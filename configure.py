#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import json
import os
import sys
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
VERSIONS = [
    "G4SE01",  # 0 - USA
    "G4SJ01",  # 1 - Japan
    "G4SP01",  # 2 - Europe
]
DEFAULT_VERSION = VERSIONS.index("G4SE01")

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--no-asm",
    action="store_true",
    help="don't incorporate .s files from asm directory",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--ninja",
    metavar="BINARY",
    type=Path,
    help="path to ninja binary (optional)"
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--warn",
    dest="warn",
    type=str,
    choices=["all", "off", "error"],
    help="how to handle warnings",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.ninja_path = args.ninja
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
if args.no_asm:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20251118"
config.dtk_tag = "v1.8.4"
config.objdiff_tag = "v3.8.3"
config.sjiswrap_tag = "v1.2.2"
config.wibo_tag = "1.0.3"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym version={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = 228 # The Legend of Zelda: Four Swords Adventures (DOL)

# Globs to exclude from context files
# *.mch excludes precompiled header output (which cannot be parsed)
config.context_exclude_globs = ["*.mch"]

# Macro definitions to inject into context files
config.context_defines = ["DECOMPCTX"]

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    # "-W all",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-multibyte",  # For Wii compilers, replace with `-enc SJIS`
    "-i include",
    f"-i build/{config.version}/include",
    "-i src",
    "-i src/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include",
    "-i src/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Include",
    "-i src/PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Include",
    "-i src/PowerPC_EABI_Support/MSL/MSL_C++/MSL_Common/Include",
    "-i src/PowerPC_EABI_Support/Runtime/Inc",
    f"-DVERSION={version_num}",
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
    cflags_base.extend(['-pragma "dont_inline on"'])
    cflags_base.extend(['-pragma "optimization_level 0"'])
else:
    cflags_base.append("-DNDEBUG=1")

# Warning flags
if args.warn == "all":
    cflags_base.append("-W all")
elif args.warn == "off":
    cflags_base.append("-W off")
elif args.warn == "error":
    cflags_base.append("-W error")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline deferred,auto",
    "-char signed",
]

# Runtime and MSL_C from projectPiki/pikmin2 (CC0): GC/2.6 with pikmin2's
# cflags_runtime plus "-inline deferred" (see docs/LIBMAP.md). They use the
# mkdd/pikmin2-style MSL headers in libs/sdk/msl_include, like the SDK units.
cflags_msl = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    "-O4,p",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-multibyte",
    "-i libs/sdk/include",
    "-i libs/sdk/msl_include",
    "-use_lmw_stmw on",
    "-str reuse,readonly",
    "-common off",
    "-inline auto",
    "-inline deferred",
]


# Dolphin library flags
cflags_dolphin = [
    *cflags_base,
    "-fp_contract off",
]

# Dolphin SDK imported from doldecomp/mkdd (libs/sdk). Uses its own include
# roots so its headers don't mix with the TWW-derived ones in include/.
cflags_sdk = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-use_lmw_stmw on",
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-multibyte",
    "-i libs/sdk/include",
    "-i libs/sdk/msl_include",
    "-DNDEBUG=1",
    "-lang=c",
    "-fp fmadd",
    "-fp_contract off",
    "-char signed",
    "-str reuse",
    "-common off",
    "-O4,p",
]

# GX from zeldaret/oot-gc (libs/sdk_gx), CE build of the Jul 23 2003 SDK.
cflags_sdk_gx = [
    "-Cpp_exceptions off",
    "-proc gekko",
    "-fp hardware",
    "-fp_contract on",
    "-enum int",
    "-align powerpc",
    "-nosyspath",
    "-RTTI off",
    "-str reuse",
    "-multibyte",
    "-O4,p",
    "-inline auto",
    "-nodefaults",
    "-sym on",
    "-i libs/sdk_gx/include",
    "-i libs/sdk_gx/libc",
    "-DVERSION=5",
]

# JSystem from doldecomp/mkdd (libs/jsystem), built with GC/2.6 like FSA's.
cflags_jsystem_mkdd = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-use_lmw_stmw on",
    "-common on",
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-multibyte",
    "-i libs/sdk/include",
    "-i libs/sdk/msl_include",
    "-DNDEBUG=1",
    "-lang=c++",
    "-fp fmadd",
    "-char signed",
    "-rostr",
]

# Framework flags
cflags_framework = [
    *cflags_base,
    "-use_lmw_stmw off",
    "-str reuse,pool,readonly",
    "-inline noauto",
    "-O3,s",
    "-schedule off",
    "-sym on",
    "-fp_contract off",
]

# JSystem flags — same as framework but WITHOUT -schedule off
# FSA's JSystem libs were compiled without instruction scheduling
cflags_jsystem = [f for f in cflags_framework if f != "-schedule off"]

# MetroTRK v2.6 (TRK_MINNOW_DOLPHIN) from projectPiki/pikmin2, GC/2.6 with
# pikmin2's TRK flags (cflags_runtime plus -inline deferred, -sdata 0, -sdata2 0).
cflags_trk = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,readonly",
    "-common off",
    "-inline auto",
    "-inline deferred",
    "-sdata 0",
    "-sdata2 0",
]

# TWW game code flags
config.linker_version = "GC/1.3.2"



# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_dolphin,
        "progress_category": "sdk",
        "host": False,
        "objects": objects,
    }


# Helper function for the imported Dolphin SDK (libs/sdk)
def SdkLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "src_dir": "libs/sdk",
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_sdk,
        "progress_category": "sdk",
        "host": False,
        "objects": objects,
    }


# Helper function for GX from oot-gc (libs/sdk_gx)
def SdkGxLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "src_dir": "libs/sdk_gx",
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_sdk_gx,
        "progress_category": "sdk",
        "host": False,
        "objects": objects,
    }


# Helper function for JSystem from mkdd (libs/jsystem)
def MkddJSystemLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "src_dir": "libs/jsystem",
        "mw_version": "GC/2.6",
        "cflags": cflags_jsystem_mkdd,
        "progress_category": "jsystem",
        "host": False,
        "objects": objects,
    }


# Helper function for JSystem libraries
def JSystemLib(lib_name, objects, progress_category="third_party"):
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_jsystem,
        "progress_category": progress_category,
        "host": True,
        "objects": objects,
    }

Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions

def EquivalentFor(*versions):
    return False

config.warn_missing_config = True
config.warn_missing_source = False
# Precompiled headers, as in TWW. JSystem uses cflags_jsystem (no -schedule off).
config.precompiled_headers = [
    {
        "source": "JSystem/JSystem.pch",
        "mw_version": "GC/1.3.2",
        "cflags": ["-lang=c++", *cflags_jsystem],
    },
]
config.libs = [
    SdkLib("amcstubs", [
        Object(Matching, "dolphin/amcstubs/AmcExi2Stubs.c"),
    ]),
    SdkLib("ai", [
        Object(Matching, "dolphin/ai/ai.c"),
    ]),
    SdkLib("ar", [
        Object(Matching, "dolphin/ar/ar.c"),
        Object(Matching, "dolphin/ar/arq.c"),
    ]),
    SdkLib("base", [
        Object(Matching, "dolphin/base/PPCArch.c"),
    ]),
    SdkLib("card", [
        Object(Matching, "dolphin/card/CARDBios.c"),
        Object(Matching, "dolphin/card/CARDBlock.c"),
        Object(Matching, "dolphin/card/CARDCheck.c"),
        Object(Matching, "dolphin/card/CARDCreate.c"),
        Object(Matching, "dolphin/card/CARDDir.c"),
        Object(Matching, "dolphin/card/CARDFormat.c"),
        Object(Matching, "dolphin/card/CARDMount.c"),
        Object(Matching, "dolphin/card/CARDNet.c"),
        Object(Matching, "dolphin/card/CARDOpen.c"),
        Object(Matching, "dolphin/card/CARDRdwr.c"),
        Object(Matching, "dolphin/card/CARDRead.c"),
        Object(Matching, "dolphin/card/CARDStat.c"),
        Object(Matching, "dolphin/card/CARDUnlock.c"),
        Object(Matching, "dolphin/card/CARDWrite.c"),
    ]),
    SdkLib("db", [
        Object(Matching, "dolphin/db/db.c"),
    ]),
    SdkLib("dsp", [
        Object(Matching, "dolphin/dsp/dsp.c"),
        Object(Matching, "dolphin/dsp/dsp_task.c"),
    ]),
    SdkLib("dvd", [
        Object(Matching, "dolphin/dvd/dvd.c"),
        Object(Matching, "dolphin/dvd/dvderror.c"),
        # FIXME: .text matches; symbols.txt has FatalFunc as 8 bytes (dtk guess), the static is 4 (coordinator: size 0x4, scope:local).
        Object(Matching, "dolphin/dvd/dvdFatal.c"),
        Object(NonMatching, "dolphin/dvd/dvdfs.c"),
        Object(Matching, "dolphin/dvd/dvdidutils.c"),
        Object(Matching, "dolphin/dvd/dvdlow.c"),
        Object(Matching, "dolphin/dvd/dvdqueue.c"),
        Object(Matching, "dolphin/dvd/fstload.c"),
    ]),
    SdkLib("exi", [
        Object(Matching, "dolphin/exi/EXIBios.c"),
        Object(Matching, "dolphin/exi/EXIUart.c"),
    ]),
    SdkLib("gba", [
        Object(Matching, "dolphin/gba/GBA.c"),
        Object(Matching, "dolphin/gba/GBAGetProcessStatus.c"),
        Object(Matching, "dolphin/gba/GBAJoyBoot.c"),
        Object(Matching, "dolphin/gba/GBARead.c"),
        Object(Matching, "dolphin/gba/GBAWrite.c"),
        Object(Matching, "dolphin/gba/GBAXfer.c"),
        # FIXME: the DSP microcode in .data has no licensed source; see GBAKey.c.
        Object(NonMatching, "dolphin/gba/GBAKey.c"),
    ]),
    SdkLib("gd", [
        Object(Matching, "dolphin/gd/GDBase.c"),
        Object(Matching, "dolphin/gd/GDGeometry.c"),
    ]),
    SdkLib("mtx", [
        Object(NonMatching, "dolphin/mtx/mtx.c"),
        Object(NonMatching, "dolphin/mtx/mtx44.c"),
        Object(Matching, "dolphin/mtx/mtxvec.c"),
        Object(Matching, "dolphin/mtx/vec.c"),
    ]),
    SdkLib("os", [
        Object(Matching, "dolphin/os/OS.c"),
        Object(Matching, "dolphin/os/OSAlarm.c"),
        Object(Matching, "dolphin/os/OSAlloc.c"),
        Object(Matching, "dolphin/os/OSArena.c"),
        Object(Matching, "dolphin/os/OSAudioSystem.c"),
        Object(Matching, "dolphin/os/OSCache.c"),
        Object(Matching, "dolphin/os/OSContext.c"),
        Object(Matching, "dolphin/os/OSError.c"),
        Object(Matching, "dolphin/os/OSFont.c"),
        Object(Matching, "dolphin/os/OSInterrupt.c"),
        Object(Matching, "dolphin/os/OSLink.c"),
        Object(Matching, "dolphin/os/OSMemory.c"),
        Object(Matching, "dolphin/os/OSMessage.c"),
        Object(Matching, "dolphin/os/OSMutex.c"),
        Object(Matching, "dolphin/os/OSReboot.c"),
        Object(Matching, "dolphin/os/OSReset.c"),
        Object(Matching, "dolphin/os/OSResetSW.c"),
        Object(Matching, "dolphin/os/OSRtc.c"),
        Object(Matching, "dolphin/os/OSSync.c"),
        Object(Matching, "dolphin/os/OSThread.c"),
        Object(Matching, "dolphin/os/OSTime.c"),
        Object(Matching, "dolphin/os/init/__start.c"),
        Object(Matching, "dolphin/os/init/__ppc_eabi_init.cpp"),
    ]),
    SdkLib("odenotstub", [
        Object(Matching, "dolphin/odenotstub/odenotstub.c"),
    ]),
    SdkLib("OdemuExi2", [
        Object(Matching, "dolphin/OdemuExi2/DebuggerDriver.c", extra_cflags=["-inline deferred"]),
    ]),
    SdkLib("pad", [
        Object(Matching, "dolphin/pad/Pad.c"),
        Object(Matching, "dolphin/pad/Padclamp.c"),
    ]),
    SdkLib("si", [
        Object(Matching, "dolphin/si/SIBios.c"),
        Object(Matching, "dolphin/si/SISamplingRate.c"),
    ]),
    SdkLib("vi", [
        Object(Matching, "dolphin/vi/vi.c"),
    ]),
    MkddJSystemLib("mkdd_J2DGraph", [
        Object(Matching, "JSystem/J2DGraph/J2DAnimation.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DAnmLoader.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J2DGraph/J2DBloSaver.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DGrafContext.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DManage.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J2DGraph/J2DMatBlock.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DMaterial.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J2DGraph/J2DMaterialFactory.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DOrthoGraph.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J2DGraph/J2DPane.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J2DGraph/J2DPicture.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J2DGraph/J2DPictureEx.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DPrint.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DScreen.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J2DGraph/J2DTevs.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J2DGraph/J2DTextBox.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DTextBoxEx.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J2DGraph/J2DWindow.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J2DGraph/J2DWindowEx.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JAudio", [
        Object(NonMatching, "JSystem/JAudio/Interface/JAIAudible.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Interface/JAISe.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Interface/JAISeMgr.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Interface/JAISeq.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Interface/JAISeqMgr.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Interface/JAISound.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Interface/JAISoundChild.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Interface/JAISoundHandles.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAISoundParams.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Interface/JAIStream.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Interface/JAIStreamMgr.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASAiCtrl.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASAramStream.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASAudioReseter.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASBNKParser.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASBank.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASBasicBank.cpp", extra_cflags=["-O4,p"]),  # FIXME(link): undefined JASGenericMemPool::JASGenericMemPool
        Object(NonMatching, "JSystem/JAudio/System/JASBasicInst.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASBasicWaveBank.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASCalc.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASCallback.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASChannel.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASCmdStack.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASDSPChannel.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASDSPInterface.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASDriverIF.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASDrumSet.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASDvdThread.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASHeapCtrl.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASInstRand.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASInstSense.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASLfo.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASOscillator.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASProbe.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASRegisterParam.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASReport.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASResArcLoader.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASSeqCtrl.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASSeqParser.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASSeqReader.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASSimpleWaveBank.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASSoundParams.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASTaskThread.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASTrack.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASTrackPort.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASWSParser.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASWaveArcLoader.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Task/dspproc.c", extra_cflags=["-O4,p", "-O4,s", "-inline", "noauto", "-use_lmw_stmw", "off", "-func_align", "32"]),
        Object(Matching, "JSystem/JAudio/Task/dsptask.c", extra_cflags=["-O4,p", "-O4,s", "-inline", "noauto", "-use_lmw_stmw", "off", "-func_align", "32"]),
        Object(Matching, "JSystem/JAudio/Task/osdsp.c", extra_cflags=["-O4,p", "-O4,s", "-inline", "noauto", "-use_lmw_stmw", "off", "-func_align", "32"]),
        Object(Matching, "JSystem/JAudio/Task/osdsp_task.c", extra_cflags=["-O4,p", "-O4,s", "-inline", "noauto", "-use_lmw_stmw", "off", "-func_align", "32"]),
        Object(NonMatching, "JSystem/JAudio/Utility/JAUAudience.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Utility/JAUAudioArcInterpreter.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Utility/JAUAudioArcLoader.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Utility/JAUAudioMgr.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Utility/JAUBankTable.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Utility/JAUInitializer.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Utility/JAUSectionHeap.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUSeqCollection.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUSeqDataBlockMgr.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Utility/JAUSoundMgr.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Utility/JAUSoundObject.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Utility/JAUSoundTable.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUStreamFileTable.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JFramework", [
        Object(Matching, "JSystem/JFramework/JFWDisplay.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JFramework/JFWSystem.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JGadget", [
        Object(NonMatching, "JSystem/JGadget/binary.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JGadget/linklist.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JKernel", [
        Object(NonMatching, "JSystem/JKernel/JKRAram.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRAramArchive.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRAramBlock.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRAramHeap.cpp", extra_cflags=["-O4,p"]),  # FIXME(link): undefined operator delete
        Object(NonMatching, "JSystem/JKernel/JKRAramPiece.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRAramStream.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRArchivePri.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRArchivePub.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRCompArchive.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRDecomp.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRDisposer.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRDvdAramRipper.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRDvdArchive.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRDvdFile.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRDvdRipper.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRExpHeap.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRFileFinder.cpp", extra_cflags=["-O4,p"]),  # FIXME(link): weak __dt__13JKRFileFinderFv comes first in FSA (0x80082F44); ours emits it after the class methods
        Object(Matching, "JSystem/JKernel/JKRFileLoader.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRHeap.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRMemArchive.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRSolidHeap.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRThread.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JMath", [
        Object(Matching, "JSystem/JMath/JMATrigonometric.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JMath/JMath.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JMessage", [
        Object(NonMatching, "JSystem/JMessage/resource.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JMessage/processor.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JMessage/control.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JParticle", [
        Object(NonMatching, "JSystem/JParticle/JPABaseShape.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAChildShape.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JParticle/JPADynamicsBlock.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JParticle/JPAEmitter.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JParticle/JPAEmitterManager.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAExTexShape.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JParticle/JPAExtraShape.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAFieldBlock.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAKeyBlock.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JParticle/JPAMath.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JParticle/JPAParticle.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAResource.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JParticle/JPAResourceLoader.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JParticle/JPAResourceManager.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPATexture.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JSupport", [
        Object(NonMatching, "JSystem/JSupport/JSUFileStream.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JSupport/JSUInputStream.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JSupport/JSUList.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JSupport/JSUMemoryStream.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JUtility", [
        Object(Matching, "JSystem/JUtility/JUTAssert.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTConsole.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JUtility/JUTDbPrint.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTDirectFile.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTDirectPrint.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JUtility/JUTFont.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JUtility/JUTGamePad.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTGraphFifo.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JUtility/JUTNameTab.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTPalette.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JUtility/JUTProcBar.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTResFont.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTResource.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTTexture.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTVideo.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTXfb.cpp", extra_cflags=["-O4,p"]),
    ]),
    # Runtime's __mem.c (.init memset/__fill_mem/memcpy) from pikmin2, with
    # pikmin2's Runtime flags (cflags_runtime + -inline deferred).
    {
        "lib": "Runtime.PPCEABI.H (__mem)",
        "mw_version": "GC/2.6",
        "cflags": [
            "-nodefaults", "-proc gekko", "-align powerpc", "-enum int",
            "-fp hardware", "-Cpp_exceptions off", "-O4,p",
            '-pragma "cats off"', '-pragma "warn_notinlined off"',
            "-maxerrors 1", "-nosyspath", "-RTTI off", "-fp_contract on",
            "-multibyte", "-i libs/sdk/include", "-i libs/sdk/msl_include",
            "-use_lmw_stmw on", "-str reuse,readonly", "-common off",
            "-inline auto", "-inline deferred",
        ],
        "progress_category": "sdk",
        "host": False,
        "objects": [
            Object(Matching, "PowerPC_EABI_Support/Runtime/Src/__mem.c"),
        ],
    },
    SdkGxLib("gx", [
        Object(Matching, "dolphin/gx/GXAttr.c"),
        Object(Matching, "dolphin/gx/GXBump.c"),
        Object(Matching, "dolphin/gx/GXDisplayList.c"),
        Object(Matching, "dolphin/gx/GXFifo.c"),
        Object(Matching, "dolphin/gx/GXFrameBuf.c"),
        Object(Matching, "dolphin/gx/GXGeometry.c"),
        Object(Matching, "dolphin/gx/GXInit.c"),
        Object(Matching, "dolphin/gx/GXLight.c"),
        Object(Matching, "dolphin/gx/GXMisc.c"),
        Object(NonMatching, "dolphin/gx/GXPerf.c"),
        Object(NonMatching, "dolphin/gx/GXPixel.c", extra_cflags=["-fp_contract off"]),
        Object(Matching, "dolphin/gx/GXTev.c"),
        Object(Matching, "dolphin/gx/GXTexture.c"),
        Object(Matching, "dolphin/gx/GXTransform.c", extra_cflags=["-fp_contract off"]),
    ]),
    {
        "lib": "TRK_MINNOW_DOLPHIN",
        "mw_version": "GC/2.6",
        "cflags": cflags_trk,
        "progress_category": "sdk",
        "host": False,
        "objects": [
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/mainloop.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/nubevent.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/nubinit.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msg.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msgbuf.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/serpoll.c", extra_cflags=["-sdata 8"]),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Os/dolphin/usr_put.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/dispatch.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msghndlr.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/support.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/mutex_TRK.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/notify.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Processor/ppc/Generic/flush_cache.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/mem_TRK.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Processor/ppc/Generic/targimpl.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Export/targsupp.s"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Processor/ppc/Generic/mpc_7xx_603e.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Processor/ppc/Generic/__exception.s"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Os/dolphin/dolphin_trk.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/main_TRK.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Os/dolphin/dolphin_trk_glue.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Os/dolphin/targcont.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Os/dolphin/target_options.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Export/mslsupp.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/MetroTRK/Os/dolphin/UDP_Stubs.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/gamedev/cust_connection/cc/exi2/GCN/EXI2_DDH_GCN/main.c", extra_cflags=["-sdata 8"]),
        Object(Matching, "TRK_MINNOW_DOLPHIN/gamedev/cust_connection/utils/common/CircleBuffer.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/gamedev/cust_connection/cc/exi2/GCN/EXI2_GDEV_GCN/main.c", extra_cflags=["-sdata 8"]),
        Object(Matching, "TRK_MINNOW_DOLPHIN/gamedev/cust_connection/utils/common/MWTrace.c"),
        Object(Matching, "TRK_MINNOW_DOLPHIN/gamedev/cust_connection/utils/gc/MWCriticalSection_gc.c"),
        ],
    },
    # J3D from projectPiki/pikmin2 (CC0), in its link order; TP path layout.
    MkddJSystemLib("J3DGraphBase", [
        Object(NonMatching, "JSystem/J3DGraphBase/J3DSys.cpp", extra_cflags=["-O4,p"]),  # FIXME: pikmin2's fakeFunc (a fakematch) must go before this can be Matching
        Object(Matching, "JSystem/J3DGraphBase/J3DVertex.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphBase/J3DTransform.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphBase/J3DPacket.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphBase/J3DShape.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphBase/J3DMaterial.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphBase/J3DTevs.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphBase/J3DDrawBuffer.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphBase/J3DGD.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphBase/J3DMatBlock.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphBase/J3DShapeMtx.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphBase/J3DShapeDraw.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("J3DGraphAnimator", [
        Object(NonMatching, "JSystem/J3DGraphAnimator/J3DModel.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphAnimator/J3DAnimation.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphAnimator/J3DMaterialAnm.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphAnimator/J3DCluster.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphAnimator/J3DJoint.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphAnimator/J3DModelData.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphAnimator/J3DMaterialAttach.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphAnimator/J3DJointTree.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphAnimator/J3DSkinDeform.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphAnimator/J3DMtxBuffer.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphAnimator/J3DShapeTable.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("J3DGraphLoader", [
        Object(NonMatching, "JSystem/J3DGraphLoader/J3DMaterialFactory.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphLoader/J3DBinaryFormat.cpp", extra_cflags=["-O4,p"]),  # FIXME: pikmin2 emits the weak J3DShapeMtx::getType through a fabricated function
        Object(NonMatching, "JSystem/J3DGraphLoader/J3DModelLoader.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphLoader/J3DJointFactory.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphLoader/J3DShapeFactory.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphLoader/J3DAnmLoader.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphLoader/J3DModelSaver.cpp", extra_cflags=["-O4,p"]),  # FIXME: pikmin2 emits the weak getUseMtxIndex through a fabricated function
        Object(NonMatching, "JSystem/J3DGraphLoader/J3DMaterialFactory_v21.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphLoader/J3DModelLoaderCalcSize.cpp", extra_cflags=["-O4,p"]),
    ]),
    JSystemLib("JGadget", [
        Object(NonMatching, "JSystem/JGadget/std-vector.cpp"),  # FIXME(link): undefined fn_80087AB4
    ]),
    JSystemLib("JUtility", [
        Object(NonMatching, "JSystem/JUtility/JUTException.cpp"),
        Object(NonMatching, "JSystem/JUtility/JUTFader.cpp"),  # FIXME(link): undefined fn_8008CB40
    ]),
    # Runtime and MSL_C (pikmin2, GC/2.6; see cflags_msl).
    {
        "lib": "Runtime",
        "mw_version": "GC/2.6",
        "cflags": cflags_msl,
        "progress_category": "sdk",
        "host": False,
        "objects": [
        Object(Matching, "PowerPC_EABI_Support/Runtime/Src/__va_arg.c"),
        Object(Matching, "PowerPC_EABI_Support/Runtime/Src/global_destructor_chain.c"),
        Object(Matching, "PowerPC_EABI_Support/Runtime/Src/NMWException.cp", extra_cflags=["-Cpp_exceptions on"]),
        Object(Matching, "PowerPC_EABI_Support/Runtime/Src/ptmf.c"),
        Object(Matching, "PowerPC_EABI_Support/Runtime/Src/runtime.c"),
        Object(Matching, "PowerPC_EABI_Support/Runtime/Src/__init_cpp_exceptions.cpp"),
        Object(Matching, "PowerPC_EABI_Support/Runtime/Src/Gecko_ExceptionPPC.cp", extra_cflags=["-Cpp_exceptions on"]),
        Object(Matching, "PowerPC_EABI_Support/Runtime/Src/GCN_mem_alloc.c"),
        ],
    },
    {
        "lib": "MSL_C",
        "mw_version": "GC/2.6",
        "cflags": cflags_msl,
        "progress_category": "sdk",
        "host": False,
        "objects": [
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/abort_exit.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/alloc.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/errno.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/ansi_files.c"),
        Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Src/ansi_fp.c", extra_cflags=["-str pool"]),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/arith.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/buffer_io.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Src/critical_regions.gamecube.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/ctype.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/direct_io.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/file_io.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/FILE_POS.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/mbstring.c", extra_cflags=["-inline noauto,deferred"]),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/mem.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/mem_funcs.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/misc_io.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/printf.c", extra_cflags=["-str pool"]),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/float.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/scanf.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/signal.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/string.c"),
        Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/strtoul.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/wchar_io.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Src/uart_console_io_gcn.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_asin.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_atan2.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_rem_pio2.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_cos.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_rem_pio2.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_sin.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_atan.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_ceil.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_copysign.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_cos.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_floor.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_frexp.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_ldexp.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_modf.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_sin.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_asin.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_atan2.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_sqrt.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Src/math_ppc.c"),
        Object(Matching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_sqrt.c"),
        ],
    },
]


config.custom_build_rules = []
config.custom_build_steps = {}

# Grab the specific GameID so we can format our strings properly
version = VERSIONS[version_num]
out_dir = config.build_dir / version

# report.json counts every Matching (complete) unit as 100 % without diffing
# it. Our tools (progress.py, gate.py, coord.py) read report_honest.json,
# where every unit is diffed; see tools/honest_report.py.
honest_objdiff = (config.objdiff_path if config.objdiff_path and config.objdiff_path.is_file()
                  else config.build_dir / "tools" / "objdiff-cli")
config.custom_build_rules.append({
    "name": "honest_report",
    "command": f"$python tools/honest_report.py {honest_objdiff} $out $objdiff_report_args",
    "description": "REPORT $out",
})
config.custom_build_steps["post-build"] = [{
    "rule": "honest_report",
    "outputs": out_dir / "report_honest.json",
    "implicit": ["objdiff.json", "all_source", "tools/honest_report.py", honest_objdiff],
}]


# Optional callback to adjust link order. This can be used to add, remove, or reorder objects.
# This is called once per module, with the module ID and the current link order.
#
# For example, this adds "dummy.c" to the end of the DOL link order if configured with --non-matching.
# "dummy.c" *must* be configured as a Matching (or Equivalent) object in order to be linked.
def link_order_callback(module_id: int, objects: List[str]) -> List[str]:
    # Don't modify the link order for matching builds
    if not config.non_matching:
        return objects
    if module_id == 0:  # DOL
        return objects + ["dummy.c"]
    return objects

# Uncomment to enable the link order callback.
# config.link_order_callback = link_order_callback


# Optional extra categories for progress tracking
config.progress_categories = [
    ProgressCategory("game", "FSA Game Code"),
    ProgressCategory("sdk", "SDK"),
    ProgressCategory("jsystem", "JSystem"),
    ProgressCategory("third_party", "Third Party"),
]
config.progress_each_module = args.verbose
# Optional extra arguments to `objdiff-cli report generate`
config.progress_report_args = [
    # Marks relocations as mismatching if the target value is different
    # Default is "functionRelocDiffs=none", which is most lenient
    "--config functionRelocDiffs=data_value",
]

# Disable missing return type warnings for incomplete objects
for lib in config.libs:
    for obj in lib["objects"]:
        if not obj.completed:
            obj.options["extra_clang_flags"].append("-Wno-return-type")

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress information
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
