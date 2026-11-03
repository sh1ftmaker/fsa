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
# Always write the link map (build/G4SE01/fsa_game_code.elf.MAP, with -mapunused
# below): tools/honest_report.py reads it to prove which weak functions' extab
# entries the link discarded (see CLAUDE.md, "Measuring progress"). --map is
# kept for compatibility and changes nothing.
config.generate_map = True
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
config.wibo_tag = "1.1.0"  # not 1.2.0: sjiswrap.exe fails to load under it (GetProcAddressFail)

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
config.ldflags.append("-mapunused")  # list discarded (UNUSED) symbols in the map
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
    # Blobs extracted from orig/ at build time (config.yml extract:), as "assets/..."
    f"-i build/{config.version}/include",
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

# FSA's own game code (docs/PLAN.md, "Game code compiler"): Metrowerks GC/2.6,
# -O4,p C++ with exception tables. Proven on five game functions
# (fn_801F856C, fn_80154248, fn_80154310, fn_80154528, fn_80105DD4): each
# matches with exactly these flags, and -O4,s (stmw for 4 saved registers),
# -use_lmw_stmw off (6 separate stw), -fp_contract off (fmuls+fadds instead of
# fmadds) or -Cpp_exceptions off (no extab) each break one. String literals
# sit unpooled in .rodata (-str reuse,readonly); .sdata/.sbss hold objects up
# to 8 bytes and .sdata2 doubles (the default -sdata 8 -sdata2 8). The game
# has no Shift-JIS string literals. -char signed and the include order follow
# the JSystem build, whose headers game files use.
cflags_game = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions on",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-use_lmw_stmw on",
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse,readonly",
    "-multibyte",
    "-char signed",
    "-lang=c++",
    "-i libs/sdk/include",
    "-i libs/sdk/msl_include",
    "-i include",
    "-i src",
    "-DNDEBUG=1",
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
        "progress_category": "gx",
        "host": False,
        "objects": objects,
    }


# Helper function for JSystem from mkdd (libs/jsystem)
def MkddJSystemLib(
    lib_name: str, objects: List[Object], extra_cflags: List[str] = []
) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "src_dir": "libs/jsystem",
        "mw_version": "GC/2.6",
        "cflags": cflags_jsystem_mkdd + extra_cflags,
        "progress_category": "jsystem",
        "host": False,
        "objects": objects,
    }


# Helper function for JSystem libraries
def JSystemLib(lib_name, objects, progress_category="jsystem"):
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_jsystem,
        "progress_category": progress_category,
        "host": True,
        "objects": objects,
    }


# Game translation units (config/G4SE01/game_units.tsv, from tu_map.tsv by
# tools/tu_map/to_splits.py). Every unit is listed from the start, with or
# without a source file: until src/game/<name>.cpp exists it counts as its
# target only (0 %), so the `game` progress category covers the whole game
# region, and a unit agent only adds its source and flips its own flag.
def GameLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "src_dir": "src",
        "mw_version": "GC/2.6",
        "cflags": cflags_game,
        "progress_category": "game",
        "host": False,
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
        Object(Matching, "dolphin/dsp/dsp_debug.c"),
        Object(Matching, "dolphin/dsp/dsp_task.c"),
    ]),
    SdkLib("dvd", [
        Object(Matching, "dolphin/dvd/dvd.c"),
        Object(Matching, "dolphin/dvd/dvderror.c"),
        Object(Matching, "dolphin/dvd/dvdFatal.c"),
        Object(Matching, "dolphin/dvd/dvdfs.c"),
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
        Object(Matching, "dolphin/gba/GBAKey.c"),  # .data: build-extracted microcode
    ]),
    SdkLib("gd", [
        Object(Matching, "dolphin/gd/GDBase.c"),
        Object(Matching, "dolphin/gd/GDGeometry.c"),
    ]),
    SdkLib("mtx", [
        Object(Matching, "dolphin/mtx/mtx.c"),
        Object(Matching, "dolphin/mtx/mtx44.c"),
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
        Object(Matching, "JSystem/J2DGraph/J2DPicture.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J2DGraph/J2DPictureEx.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DPrint.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DScreen.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DTevs.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DTextBox.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DTextBoxEx.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J2DGraph/J2DWindow.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J2DGraph/J2DWindowEx.cpp", extra_cflags=["-O4,p"]),
    ]),
    # JAudio/Interface and JAudio/Utility without -sym on. PROVISIONAL: drop
    # if any Interface/Utility unit is later found to need -sym on.
    # JAISeq, JAISeMgr and JAUSectionHeap link only with it off (their weak
    # copies stay in place, as in FSA); JAUAudioMgr links neither way (its
    # .data vtable order differs). Every other unit of the two directories
    # compiles identically either way. JAudio/System and Task need it on;
    # which unit needs it is still to be found.
    MkddJSystemLib("mkdd_JAudio_Interface", [
        Object(Matching, "JSystem/JAudio/Interface/JAIAudible.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAIAudience.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAISe.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAISeMgr.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAISeq.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAISeqDataMgr.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAISeqMgr.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAISound.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAISoundChild.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAISoundHandles.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAISoundInfo.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAISoundParams.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAISoundStarter.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAIStream.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAIStreamDataMgr.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Interface/JAIStreamMgr.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JAudio", [
        Object(Matching, "JSystem/JAudio/System/JASAiCtrl.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASAramStream.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASAudioReseter.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASAudioThread.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASBNKParser.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASBank.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASBasicBank.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASBasicInst.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASBasicWaveBank.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASCalc.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASCallback.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASChannel.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASCmdStack.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASDSPChannel.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASDSPInterface.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASDriverIF.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASDrumSet.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASDvdThread.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASHeapCtrl.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASInstRand.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASInstSense.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASLfo.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASOscillator.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASProbe.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASRegisterParam.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASReport.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASResArcLoader.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASSeqCtrl.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASSeqParser.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASSeqReader.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASSimpleWaveBank.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASSoundParams.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASTaskThread.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/System/JASTrack.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASTrackPort.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASVoiceBank.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASWSParser.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/System/JASWaveArcLoader.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Task/dspproc.c", extra_cflags=["-O4,p", "-O4,s", "-inline", "noauto", "-use_lmw_stmw", "off", "-func_align", "32"]),
        Object(Matching, "JSystem/JAudio/Task/dsptask.c", extra_cflags=["-O4,p", "-O4,s", "-inline", "noauto", "-use_lmw_stmw", "off", "-func_align", "32"]),
        Object(Matching, "JSystem/JAudio/Task/osdsp.c", extra_cflags=["-O4,p", "-O4,s", "-inline", "noauto", "-use_lmw_stmw", "off", "-func_align", "32"]),
        Object(Matching, "JSystem/JAudio/Task/osdsp_task.c", extra_cflags=["-O4,p", "-O4,s", "-inline", "noauto", "-use_lmw_stmw", "off", "-func_align", "32"]),
    # FSA's JAudio/System and Task were built with -sym on, as tp's JAudio2 and Petari's
    # cflags_jsys_jaudio are: it puts weak and inline copies at the end of
    # each object, which is FSA's layout. Not validated for other libraries.
    ], extra_cflags=["-sym on"]),
    MkddJSystemLib("mkdd_JAudio_Utility", [
        Object(NonMatching, "JSystem/JAudio/Utility/JAUAudience.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUAudioArcInterpreter.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUAudioArcLoader.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JAudio/Utility/JAUAudioMgr.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUBankTable.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUClusterSound.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUInitializer.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUSectionHeap.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUSeqCollection.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUSeqDataBlockMgr.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUSoundMgr.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUSoundObject.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUSoundTable.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JAudio/Utility/JAUStreamFileTable.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JFramework", [
        Object(Matching, "JSystem/JFramework/JFWDisplay.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JFramework/JFWSystem.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JGadget", [
        Object(Matching, "JSystem/JGadget/binary.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JGadget/linklist.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JKernel", [
        Object(Matching, "JSystem/JKernel/JKRAram.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRAramArchive.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRAramBlock.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRAramHeap.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRAramPiece.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRAramStream.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRArchivePri.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRArchivePub.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRCompArchive.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRDecomp.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRDisposer.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRDvdAramRipper.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRDvdArchive.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRDvdFile.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRDvdRipper.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRExpHeap.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRFileCache.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRFileFinder.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRFileLoader.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRHeap.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRMemArchive.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRSolidHeap.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JKernel/JKRThread.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JKernel/JKRUnitHeap.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JMath", [
        Object(Matching, "JSystem/JMath/JMATrigonometric.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JMath/JMath.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JMath/random.cpp", extra_cflags=["-O4,p"]),
    ]),
    # Linked just before JMessage, as in pikmin2. FSA uses no JStudio, so the
    # link dead-stripped all of it except the first (kept) weak copies of
    # JPAEmitterCallBack's vtable and empty virtuals (pikmin2 ends this file's
    # .text with the same four, in the same order).
    MkddJSystemLib("mkdd_JStudio_JParticle", [
        Object(NonMatching, "JSystem/JStudio_JParticle/object-particle.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JMessage", [
        Object(Matching, "JSystem/JMessage/resource.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JMessage/processor.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JMessage/control.cpp", extra_cflags=["-O4,p"]),
    ]),
    # J3DU from projectPiki/pikmin2 (CC0); linked between JMessage and JKernel.
    # Only weak copies the link kept survive from it.
    MkddJSystemLib("J3DU", [
        Object(NonMatching, "JSystem/J3DU/J3DUDL.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DU/J3DUMotion.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JParticle", [
        Object(NonMatching, "JSystem/JParticle/JPABaseShape.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAChildShape.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPADynamicsBlock.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAEmitter.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAEmitterManager.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAExTexShape.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAExtraShape.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAFieldBlock.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAKeyBlock.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JParticle/JPAMath.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAParticle.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAResource.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAResourceLoader.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPAResourceManager.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JParticle/JPATexture.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JSupport", [
        Object(Matching, "JSystem/JSupport/JSUFileStream.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JSupport/JSUInputStream.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JSupport/JSUList.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JSupport/JSUMemoryStream.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JSupport/JSUOutputStream.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("mkdd_JUtility", [
        Object(Matching, "JSystem/JUtility/JUTAssert.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTConsole.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/JUtility/JUTDbPrint.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTDirectFile.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTDirectPrint.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTException.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTFont.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTGamePad.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTGraphFifo.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTNameTab.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTPalette.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/JUtility/JUTProcBar.cpp", extra_cflags=["-O4,p"]),
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
        "progress_category": "runtime",
        "host": False,
        "objects": [
            Object(Matching, "PowerPC_EABI_Support/Runtime/Src/__mem.c"),
        ],
    },
    SdkGxLib("gx", [
        Object(Matching, "dolphin/gx/GXAttr.c"),
        Object(Matching, "dolphin/gx/GXBump.c"),
        Object(Matching, "dolphin/gx/GXDraw.c", extra_cflags=["-fp_contract off"]),
        Object(Matching, "dolphin/gx/GXDisplayList.c"),
        Object(Matching, "dolphin/gx/GXFifo.c"),
        Object(Matching, "dolphin/gx/GXFrameBuf.c"),
        Object(Matching, "dolphin/gx/GXGeometry.c"),
        Object(Matching, "dolphin/gx/GXInit.c"),
        Object(Matching, "dolphin/gx/GXLight.c"),
        Object(Matching, "dolphin/gx/GXMisc.c"),
        Object(Matching, "dolphin/gx/GXPerf.c"),
        Object(Matching, "dolphin/gx/GXPixel.c", extra_cflags=["-fp_contract off"]),
        Object(Matching, "dolphin/gx/GXTev.c"),
        Object(Matching, "dolphin/gx/GXTexture.c"),
        Object(Matching, "dolphin/gx/GXTransform.c", extra_cflags=["-fp_contract off"]),
    ]),
    {
        "lib": "TRK_MINNOW_DOLPHIN",
        "mw_version": "GC/2.6",
        "cflags": cflags_trk,
        "progress_category": "runtime",
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
        Object(Matching, "JSystem/J3DGraphBase/J3DSys.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphBase/J3DVertex.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphBase/J3DTransform.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphBase/J3DPacket.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphBase/J3DShape.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphBase/J3DMaterial.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphBase/J3DTevs.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphBase/J3DDrawBuffer.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphBase/J3DGD.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphBase/J3DMatBlock.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphBase/J3DShapeMtx.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphBase/J3DShapeDraw.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphBase/J3DStruct.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("J3DGraphAnimator", [
        Object(Matching, "JSystem/J3DGraphAnimator/J3DModel.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphAnimator/J3DAnimation.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphAnimator/J3DMaterialAnm.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphAnimator/J3DCluster.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphAnimator/J3DJoint.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphAnimator/J3DModelData.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphAnimator/J3DMaterialAttach.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphAnimator/J3DJointTree.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphAnimator/J3DSkinDeform.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphAnimator/J3DMtxBuffer.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphAnimator/J3DShapeTable.cpp", extra_cflags=["-O4,p"]),
    ]),
    MkddJSystemLib("J3DGraphLoader", [
        Object(NonMatching, "JSystem/J3DGraphLoader/J3DMaterialFactory.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphLoader/J3DBinaryFormat.cpp", extra_cflags=["-O4,p"]),  # FIXME: pikmin2 emits the weak J3DShapeMtx::getType through a fabricated function
        Object(Matching, "JSystem/J3DGraphLoader/J3DModelLoader.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphLoader/J3DJointFactory.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphLoader/J3DShapeFactory.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphLoader/J3DAnmLoader.cpp", extra_cflags=["-O4,p"]),
        Object(NonMatching, "JSystem/J3DGraphLoader/J3DModelSaver.cpp", extra_cflags=["-O4,p"]),  # FIXME: pikmin2 emits the weak getUseMtxIndex through a fabricated function
        Object(Matching, "JSystem/J3DGraphLoader/J3DMaterialFactory_v21.cpp", extra_cflags=["-O4,p"]),
        Object(Matching, "JSystem/J3DGraphLoader/J3DModelLoaderCalcSize.cpp", extra_cflags=["-O4,p"]),
    ]),
    JSystemLib("JGadget", [
        Object(Matching, "JSystem/JGadget/std-vector.cpp"),
    ]),
    JSystemLib("JUtility", [
        Object(Matching, "JSystem/JUtility/JUTFader.cpp"),
    ]),
    # Runtime and MSL_C (pikmin2, GC/2.6; see cflags_msl).
    {
        "lib": "Runtime",
        "mw_version": "GC/2.6",
        "cflags": cflags_msl,
        "progress_category": "runtime",
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
        "progress_category": "runtime",
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
    GameLib("game", [
        Object(NonMatching, "game/tu_80021840.cpp"),
        Object(NonMatching, "game/tu_800F39EC.cpp"),
        Object(NonMatching, "game/act_rat.cpp"),
        Object(NonMatching, "game/tu_800F46C0.cpp"),
        Object(NonMatching, "game/act_force_like.cpp"),
        Object(NonMatching, "game/tu_800F7FCC.cpp"),
        Object(NonMatching, "game/RailBase.cpp"),
        Object(NonMatching, "game/tu_800FE8C0.cpp"),
        Object(Matching, "game/act_boss_display_test_npc.cpp"),
        Object(NonMatching, "game/tu_800FF060.cpp"),
        Object(NonMatching, "game/tu_8010089C.cpp"),
        Object(NonMatching, "game/act_digging_game.cpp"),
        Object(Matching, "game/act_debug_warp_settings.cpp"),
        Object(NonMatching, "game/act_tower_collapse_effect.cpp"),
        Object(NonMatching, "game/act_temple_bti_display.cpp"),
        Object(NonMatching, "game/tu_80104C3C.cpp"),
        Object(NonMatching, "game/act_cloud.cpp"),
        Object(NonMatching, "game/act_room_transfer_lift.cpp"),
        Object(NonMatching, "game/act_wind_environment.cpp"),
        Object(NonMatching, "game/act_cannon.cpp"),
        Object(NonMatching, "game/act_merchant.cpp"),
        Object(NonMatching, "game/act_shop_item.cpp"),
        Object(NonMatching, "game/act_princess_zelda.cpp"),
        Object(NonMatching, "game/act_icicle.cpp"),
        Object(NonMatching, "game/act_snowball.cpp"),
        Object(NonMatching, "game/act_message_check_npc.cpp"),
        Object(NonMatching, "game/tu_8010DF44.cpp"),
        Object(NonMatching, "game/act_princess_zelda_demo.cpp"),
        Object(NonMatching, "game/act_maiden_demo.cpp"),
        Object(NonMatching, "game/tu_80113788.cpp"),
        Object(NonMatching, "game/act_vaati_demo.cpp"),
        Object(NonMatching, "game/act_four_sword_demo.cpp"),
        Object(NonMatching, "game/tu_80118384.cpp"),
        Object(NonMatching, "game/tu_8011A0F8.cpp"),
        Object(NonMatching, "game/act_opening_goal.cpp"),
        Object(NonMatching, "game/act_shadow_link_demo.cpp"),
        Object(NonMatching, "game/tu_8011CEC8.cpp"),
        Object(NonMatching, "game/act_ice_wall.cpp"),
        Object(NonMatching, "game/tu_8011E96C.cpp"),
        Object(NonMatching, "game/act_hyrule_castle_barrier.cpp"),
        Object(NonMatching, "game/act_zora_escort.cpp"),
        Object(NonMatching, "game/tu_801213EC.cpp"),
        Object(NonMatching, "game/act_escort_woman.cpp"),
        Object(NonMatching, "game/tu_80125B58.cpp"),
        Object(Matching, "game/act_vaati_buster.cpp"),
        Object(NonMatching, "game/act_zora_escort_goal.cpp"),
        Object(NonMatching, "game/tu_80127E20.cpp"),
        Object(NonMatching, "game/tu_801286C0.cpp"),
        Object(NonMatching, "game/tu_801291A0.cpp"),
        Object(NonMatching, "game/act_rainbow_bridge.cpp"),
        Object(NonMatching, "game/tu_8012A5EC.cpp"),
        Object(NonMatching, "game/act_blizzard_battle.cpp"),
        Object(NonMatching, "game/tu_8012DAC0.cpp"),
        Object(NonMatching, "game/act_snow_wall.cpp"),
        Object(NonMatching, "game/tu_8012EA00.cpp"),
        Object(NonMatching, "game/act_ice_floor_check_npc.cpp"),
        Object(NonMatching, "game/tu_8012F3E4.cpp"),
        Object(Matching, "game/tu_8012F8AC.cpp"),
        Object(NonMatching, "game/tu_8012FBCC.cpp"),
        Object(NonMatching, "game/tu_8012FF9C.cpp"),
        Object(NonMatching, "game/tu_80130310.cpp"),
        Object(NonMatching, "game/act_warp_cannon.cpp"),
        Object(NonMatching, "game/tu_80131D98.cpp"),
        Object(NonMatching, "game/act_great_fairy_spfr.cpp"),
        Object(NonMatching, "game/tu_8013394C.cpp"),
        Object(NonMatching, "game/tu_801344C0.cpp"),
        Object(NonMatching, "game/tu_80134C04.cpp"),
        Object(NonMatching, "game/tu_80139BE8.cpp"),
        Object(NonMatching, "game/tu_8013AAD4.cpp"),
        Object(NonMatching, "game/tu_8013C5C8.cpp"),
        Object(NonMatching, "game/act_octorok.cpp"),
        Object(NonMatching, "game/act_armos.cpp"),
        Object(NonMatching, "game/act_poe.cpp"),
        Object(NonMatching, "game/act_sand_crab.cpp"),
        Object(NonMatching, "game/act_buzz_blob.cpp"),
        Object(NonMatching, "game/act_deadrock.cpp"),
        Object(NonMatching, "game/act_recovery_heart.cpp"),
        Object(NonMatching, "game/act_octorok_rock.cpp"),
        Object(NonMatching, "game/tu_80147B50.cpp"),
        Object(NonMatching, "game/act_bomb.cpp"),
        Object(NonMatching, "game/act_hinox.cpp"),
        Object(NonMatching, "game/act_sword_soldier.cpp"),
        Object(NonMatching, "game/act_tektite.cpp"),
        Object(NonMatching, "game/tu_801555D8.cpp"),
        Object(NonMatching, "game/tu_80158624.cpp"),
        Object(NonMatching, "game/act_liftable_item.cpp"),
        Object(NonMatching, "game/act_bumper.cpp"),
        Object(NonMatching, "game/act_keese.cpp"),
        Object(NonMatching, "game/act_flame.cpp"),
        Object(NonMatching, "game/tu_8015F454.cpp"),
        Object(NonMatching, "game/act_fairy.cpp"),
        Object(NonMatching, "game/tu_801618D0.cpp"),
        Object(NonMatching, "game/act_blade_trap.cpp"),
        Object(NonMatching, "game/act_arrow.cpp"),
        Object(NonMatching, "game/tu_80166BD0.cpp"),
        Object(NonMatching, "game/act_hardhat_beetle.cpp"),
        Object(NonMatching, "game/tu_801688AC.cpp"),
        Object(NonMatching, "game/tu_8016A2B4.cpp"),
        Object(NonMatching, "game/tu_80170008.cpp"),
        Object(NonMatching, "game/act_popo.cpp"),
        Object(NonMatching, "game/act_chief_soldier.cpp"),
        Object(NonMatching, "game/act_stalfos.cpp"),
        Object(NonMatching, "game/act_wizzrobe.cpp"),
        Object(NonMatching, "game/act_searchlight.cpp"),
        Object(NonMatching, "game/tu_8017A44C.cpp"),
        Object(NonMatching, "game/act_cheep_cheep.cpp"),
        Object(NonMatching, "game/tu_8017C01C.cpp"),
        Object(NonMatching, "game/tu_8017EF40.cpp"),
        Object(NonMatching, "game/tu_80182914.cpp"),
        Object(NonMatching, "game/tu_80183544.cpp"),
        Object(NonMatching, "game/act_pengator.cpp"),
        Object(NonMatching, "game/tu_80184D64.cpp"),
        Object(NonMatching, "game/act_phantom_ganon_2.cpp"),
        Object(NonMatching, "game/tu_8018A8D8.cpp"),
        Object(NonMatching, "game/tu_8018EB94.cpp"),
        Object(NonMatching, "game/tu_801902C0.cpp"),
        Object(NonMatching, "game/act_ganon.cpp"),
        Object(NonMatching, "game/tu_80197070.cpp"),
        Object(NonMatching, "game/act_all_enemies_defeated.cpp"),
        Object(NonMatching, "game/act_shooting_gallery.cpp"),
        Object(NonMatching, "game/tu_801A0710.cpp"),
        Object(NonMatching, "game/act_enemy_spawn_area.cpp"),
        Object(NonMatching, "game/tu_801A3C50.cpp"),
        Object(NonMatching, "game/act_zora_fish_herder.cpp"),
        Object(NonMatching, "game/tu_801A8194.cpp"),
        Object(NonMatching, "game/tu_801A908C.cpp"),
        Object(NonMatching, "game/act_gohma.cpp"),
        Object(NonMatching, "game/act_dodongo.cpp"),
        Object(NonMatching, "game/tu_801AE9C4.cpp"),
        Object(NonMatching, "game/act_eyegore.cpp"),
        Object(NonMatching, "game/tu_801B0A00.cpp"),
        Object(NonMatching, "game/tu_801B27DC.cpp"),
        Object(NonMatching, "game/act_princess_zelda_motion_spawner.cpp"),
        Object(NonMatching, "game/tu_801B4D0C.cpp"),
        Object(NonMatching, "game/act_tingle_wallet.cpp"),
        Object(NonMatching, "game/tu_801B6C50.cpp"),
        Object(NonMatching, "game/act_rope.cpp"),
        Object(NonMatching, "game/act_terrorpin.cpp"),
        Object(NonMatching, "game/act_gibdo.cpp"),
        Object(NonMatching, "game/act_zora.cpp"),
        Object(NonMatching, "game/act_wallmaster.cpp"),
        Object(NonMatching, "game/act_fire_bar.cpp"),
        Object(NonMatching, "game/tu_801BD8C0.cpp"),
        Object(NonMatching, "game/act_beamos.cpp"),
        Object(NonMatching, "game/tu_801BEDBC.cpp"),
        Object(NonMatching, "game/act_vulture.cpp"),
        Object(NonMatching, "game/tu_801C0D70.cpp"),
        Object(NonMatching, "game/act_shadow_link_generator.cpp"),
        Object(NonMatching, "game/act_shadow_link_2.cpp"),
        Object(NonMatching, "game/tu_801C5B08.cpp"),
        Object(NonMatching, "game/tu_801C6A4C.cpp"),
        Object(NonMatching, "game/tu_801C71BC.cpp"),
        Object(NonMatching, "game/tu_801C9AF8.cpp"),
        Object(NonMatching, "game/tu_801CB270.cpp"),
        Object(NonMatching, "game/tu_801CDC8C.cpp"),
        Object(NonMatching, "game/tu_801D11FC.cpp"),
        Object(NonMatching, "game/tu_801D1C18.cpp"),
        Object(NonMatching, "game/tu_801D2918.cpp"),
        Object(NonMatching, "game/tu_801D2C60.cpp"),
        Object(NonMatching, "game/tu_801D3320.cpp"),
        Object(NonMatching, "game/tu_801D3CBC.cpp"),
        Object(NonMatching, "game/act_vaati_boss.cpp"),
        Object(NonMatching, "game/tu_801D6CEC.cpp"),
        Object(NonMatching, "game/tu_801D7AFC.cpp"),
        Object(NonMatching, "game/tu_801D8400.cpp"),
        Object(NonMatching, "game/tu_801DA7C0.cpp"),
        Object(NonMatching, "game/tu_801DAC6C.cpp"),
        Object(NonMatching, "game/tu_801DB18C.cpp"),
        Object(NonMatching, "game/act_frostare_tail.cpp"),
        Object(NonMatching, "game/tu_801DCC98.cpp"),
        Object(NonMatching, "game/tu_801DD1AC.cpp"),
        Object(NonMatching, "game/tu_801DE714.cpp"),
        Object(NonMatching, "game/tu_801E2540.cpp"),
        Object(NonMatching, "game/act_tingle_buster.cpp"),
        Object(NonMatching, "game/tu_801E3618.cpp"),
        Object(NonMatching, "game/tu_801E4A18.cpp"),
        Object(NonMatching, "game/tu_801E4D54.cpp"),
        Object(NonMatching, "game/tu_801E5FC8.cpp"),
        Object(NonMatching, "game/tu_801E667C.cpp"),
        Object(NonMatching, "game/tu_801E6D48.cpp"),
        Object(NonMatching, "game/act_fuse_point.cpp"),
        Object(NonMatching, "game/tu_801E801C.cpp"),
        Object(NonMatching, "game/tu_801E9AFC.cpp"),
        Object(NonMatching, "game/tu_801EAB0C.cpp"),
        Object(NonMatching, "game/tu_801EB00C.cpp"),
        Object(NonMatching, "game/act_horse_race_man.cpp"),
        Object(NonMatching, "game/tu_801ED3DC.cpp"),
        Object(NonMatching, "game/tu_801EDBA4.cpp"),
        Object(NonMatching, "game/tu_801EE24C.cpp"),
        Object(NonMatching, "game/tu_801F6058.cpp"),
        Object(NonMatching, "game/tu_801F9484.cpp"),
        Object(NonMatching, "game/tu_80204638.cpp"),
        Object(NonMatching, "game/act_thief.cpp"),
        Object(NonMatching, "game/tu_80206B30.cpp"),
        Object(NonMatching, "game/tu_80207548.cpp"),
        Object(NonMatching, "game/act_slime_manager.cpp"),
        Object(NonMatching, "game/act_bird_manager.cpp"),
        Object(NonMatching, "game/tu_80209C14.cpp"),
        Object(NonMatching, "game/tu_8020B354.cpp"),
        Object(NonMatching, "game/tu_8020C0A8.cpp"),
        Object(NonMatching, "game/tu_8020CCA8.cpp"),
        Object(NonMatching, "game/tu_8020D58C.cpp"),
        Object(NonMatching, "game/act_floormaster.cpp"),
        Object(NonMatching, "game/tu_8020F798.cpp"),
        Object(NonMatching, "game/tu_8020FE50.cpp"),
        Object(NonMatching, "game/tu_80210630.cpp"),
        Object(NonMatching, "game/act_quicksand.cpp"),
        Object(NonMatching, "game/tu_80212AC0.cpp"),
        Object(NonMatching, "game/tu_80212E3C.cpp"),
        Object(NonMatching, "game/tu_80213E00.cpp"),
        Object(Matching, "game/tu_80214244.cpp"),
        Object(NonMatching, "game/tu_802144F0.cpp"),
        Object(NonMatching, "game/tu_802153A8.cpp"),
        Object(Matching, "game/tu_802157B8.cpp"),
        Object(NonMatching, "game/tu_80215A94.cpp"),
        Object(NonMatching, "game/tu_8021693C.cpp"),
        Object(NonMatching, "game/tu_80217374.cpp"),
        Object(NonMatching, "game/tu_8021775C.cpp"),
        Object(NonMatching, "game/tu_8021827C.cpp"),
        Object(NonMatching, "game/tu_802188B0.cpp"),
        Object(NonMatching, "game/act_kaepora_gaebora.cpp"),
        Object(NonMatching, "game/tu_8021A570.cpp"),
        Object(NonMatching, "game/tu_8021AF28.cpp"),
        Object(NonMatching, "game/tu_8021B904.cpp"),
        Object(Matching, "game/tu_8021BCBC.cpp"),
        Object(Matching, "game/tu_8021BFA8.cpp"),
        Object(NonMatching, "game/tu_8021C2B4.cpp"),
        Object(NonMatching, "game/tu_8021C848.cpp"),
        Object(NonMatching, "game/tu_8021CDE4.cpp"),
        Object(NonMatching, "game/act_fireball_generator.cpp"),
        Object(NonMatching, "game/act_navi.cpp"),
        Object(NonMatching, "game/tu_8021F064.cpp"),
        Object(NonMatching, "game/tu_8021F4B0.cpp"),
        Object(NonMatching, "game/tu_8021FDB8.cpp"),
        Object(NonMatching, "game/tu_802201D8.cpp"),
        Object(NonMatching, "game/act_shadow_link_jump.cpp"),
        Object(NonMatching, "game/tu_80220F54.cpp"),
        Object(NonMatching, "game/tu_80221B44.cpp"),
        Object(NonMatching, "game/act_shadow_link_bow.cpp"),
        Object(NonMatching, "game/tu_80222CC4.cpp"),
        Object(NonMatching, "game/tu_802233A4.cpp"),
        Object(NonMatching, "game/tu_802242F4.cpp"),
        Object(NonMatching, "game/tu_8022461C.cpp"),
        Object(NonMatching, "game/act_warp.cpp"),
        Object(NonMatching, "game/act_player_effect.cpp"),
        Object(NonMatching, "game/tu_8022AD64.cpp"),
        Object(NonMatching, "game/tu_8023FBF0.cpp"),
        Object(NonMatching, "game/act_lift.cpp"),
        Object(NonMatching, "game/tu_80256C74.cpp"),
        Object(NonMatching, "game/tu_8025EB18.cpp"),
        Object(NonMatching, "game/tu_80264998.cpp"),
        Object(NonMatching, "game/tu_802684A4.cpp"),
        Object(NonMatching, "game/tu_8026FC20.cpp"),
        Object(NonMatching, "game/tu_8027E5BC.cpp"),
        Object(NonMatching, "game/tu_80285E58.cpp"),
        Object(NonMatching, "game/act_extending_lift.cpp"),
        Object(NonMatching, "game/act_pull_switch.cpp"),
        Object(NonMatching, "game/act_magic_circle.cpp"),
        Object(NonMatching, "game/act_block.cpp"),
        Object(NonMatching, "game/tu_8028DD78.cpp"),
        Object(NonMatching, "game/tu_8028FBA0.cpp"),
        Object(NonMatching, "game/tu_802957CC.cpp"),
        Object(NonMatching, "game/act_push_switch.cpp"),
        Object(NonMatching, "game/act_burning_block.cpp"),
        Object(NonMatching, "game/tu_8029C584.cpp"),
        Object(NonMatching, "game/act_attack_hitbox_when_player_is_thrown.cpp"),
        Object(NonMatching, "game/tu_8029D898.cpp"),
        Object(NonMatching, "game/act_heart_container.cpp"),
        Object(NonMatching, "game/tu_802A4654.cpp"),
        Object(NonMatching, "game/act_switch.cpp"),
        Object(NonMatching, "game/act_explosion.cpp"),
        Object(NonMatching, "game/act_force_gem_rain.cpp"),
        Object(NonMatching, "game/act_four_switch_monitor.cpp"),
        Object(NonMatching, "game/act_shutter.cpp"),
        Object(NonMatching, "game/act_spike_floor_check.cpp"),
        Object(NonMatching, "game/act_point_check_npc.cpp"),
        Object(NonMatching, "game/act_enemy_count_wipe_out_check.cpp"),
        Object(NonMatching, "game/act_item_pedestal.cpp"),
        Object(Matching, "game/act_player_start_position.cpp"),
        Object(NonMatching, "game/tu_802AC8AC.cpp"),
        Object(NonMatching, "game/act_trick_floor.cpp"),
        Object(NonMatching, "game/act_pot_smoke.cpp"),
        Object(NonMatching, "game/act_arrow_marker.cpp"),
        Object(NonMatching, "game/act_spotlight.cpp"),
        Object(NonMatching, "game/act_torch.cpp"),
        Object(NonMatching, "game/tu_802B1558.cpp"),
        Object(NonMatching, "game/tu_802B214C.cpp"),
        Object(Matching, "game/act_spotlight_se.cpp"),
        Object(NonMatching, "game/tu_802B3674.cpp"),
        Object(NonMatching, "game/act_stone_disguised_npc.cpp"),
        Object(NonMatching, "game/act_liftable_wood.cpp"),
        Object(NonMatching, "game/act_switch_or.cpp"),
        Object(NonMatching, "game/tu_802B7C78.cpp"),
        Object(NonMatching, "game/act_hammer_bulge.cpp"),
        Object(NonMatching, "game/act_thwomp_switch.cpp"),
        Object(Matching, "game/act_bg_rewrite_check_switch.cpp"),
        Object(NonMatching, "game/tu_802BA334.cpp"),
        Object(NonMatching, "game/act_fire_rod_fire.cpp"),
        Object(NonMatching, "game/act_spreading_fire.cpp"),
        Object(NonMatching, "game/act_all_enemies_defeated_check_switch.cpp"),
        Object(NonMatching, "game/act_wait_switch.cpp"),
        Object(NonMatching, "game/act_candlestick_switch.cpp"),
        Object(NonMatching, "game/act_check_switch.cpp"),
        Object(NonMatching, "game/act_switch_related_npc.cpp"),
        Object(NonMatching, "game/act_dropped_item.cpp"),
        Object(NonMatching, "game/act_small_key.cpp"),
        Object(NonMatching, "game/act_door.cpp"),
        Object(NonMatching, "game/act_panel_rewrite_npc.cpp"),
        Object(NonMatching, "game/act_focus_npc.cpp"),
        Object(NonMatching, "game/act_blindfold.cpp"),
        Object(NonMatching, "game/act_rank_display.cpp"),
        Object(NonMatching, "game/act_on_off_switch.cpp"),
        Object(NonMatching, "game/act_red_blue_switch.cpp"),
        Object(NonMatching, "game/act_falling_floor.cpp"),
        Object(NonMatching, "game/act_bti_npc.cpp"),
        Object(NonMatching, "game/act_grass.cpp"),
        Object(NonMatching, "game/act_spawner.cpp"),
        Object(NonMatching, "game/act_stone.cpp"),
        Object(NonMatching, "game/act_pot.cpp"),
        Object(NonMatching, "game/act_rock.cpp"),
        Object(NonMatching, "game/act_sign.cpp"),
        Object(NonMatching, "game/tu_802D39B8.cpp"),
        Object(NonMatching, "game/act_coconut.cpp"),
        Object(NonMatching, "game/act_gravestone.cpp"),
        Object(NonMatching, "game/act_view_mode.cpp"),
        Object(Matching, "game/act_ura_colour.cpp"),
        Object(NonMatching, "game/act_particle_npc.cpp"),
        Object(NonMatching, "game/act_off_switch.cpp"),
        Object(NonMatching, "game/tu_802E2074.cpp"),
        Object(NonMatching, "game/tu_802E2990.cpp"),
        Object(Matching, "game/act_advance_notice_se.cpp"),
        Object(NonMatching, "game/act_environment_control_2.cpp"),
        Object(NonMatching, "game/act_inextinguishable_flame.cpp"),
        Object(NonMatching, "game/act_bgm_change_npc.cpp"),
        Object(NonMatching, "game/act_zoom.cpp"),
        Object(NonMatching, "game/tu_802E6EC4.cpp"),
        Object(NonMatching, "game/act_wave_se.cpp"),
        Object(Matching, "game/act_play_specified_se.cpp"),
        Object(NonMatching, "game/act_panel_rewrite_npc_2.cpp"),
        Object(NonMatching, "game/act_magic_circle_generator.cpp"),
        Object(Matching, "game/act_local_switch.cpp"),
        Object(Matching, "game/act_global_switch.cpp"),
        Object(NonMatching, "game/tu_802E9428.cpp"),
        Object(NonMatching, "game/act_treasure_chest.cpp"),
        Object(NonMatching, "game/act_enemy_generator_3.cpp"),
        Object(Matching, "game/act_zoom_out_npc.cpp"),
        Object(NonMatching, "game/act_sandstorm.cpp"),
        Object(NonMatching, "game/act_stop_npc.cpp"),
        Object(NonMatching, "game/tu_802ED538.cpp"),
        Object(NonMatching, "game/act_chain_switch.cpp"),
        Object(Matching, "game/act_gba_window.cpp"),
        Object(NonMatching, "game/act_ending_map_skipper.cpp"),
        Object(NonMatching, "game/tu_802EF76C.cpp"),
        Object(NonMatching, "game/act_player_sword_level_check_npc.cpp"),
        Object(NonMatching, "game/act_trick_floor_2.cpp"),
        Object(NonMatching, "game/tu_802F3B74.cpp"),
        Object(NonMatching, "game/act_pot_2_generator.cpp"),
        Object(NonMatching, "game/act_pot_2.cpp"),
        Object(Matching, "game/act_stage_info_npc.cpp"),
        Object(NonMatching, "game/tu_802F4AEC.cpp"),
        Object(NonMatching, "game/tu_802F5040.cpp"),
        Object(NonMatching, "game/tu_803006E8.cpp"),
        Object(NonMatching, "game/tu_80301CD0.cpp"),
        Object(NonMatching, "game/tu_803099FC.cpp"),
        Object(NonMatching, "game/tu_8030AE84.cpp"),
        Object(NonMatching, "game/tu_8030C058.cpp"),
        Object(NonMatching, "game/act_somaria_block.cpp"),
        Object(NonMatching, "game/act_gambling_shop.cpp"),
        Object(NonMatching, "game/act_gambling_chest.cpp"),
        Object(NonMatching, "game/act_shovel_old_timer.cpp"),
        Object(NonMatching, "game/act_knight_of_hyrule.cpp"),
        Object(NonMatching, "game/act_tingle_tower.cpp"),
        Object(NonMatching, "game/tu_80316E34.cpp"),
        Object(NonMatching, "game/act_tingle_first.cpp"),
        Object(NonMatching, "game/act_special_conversation_npc.cpp"),
        Object(NonMatching, "game/act_force_gem_winged.cpp"),
        Object(NonMatching, "game/act_force_gem_fleeing.cpp"),
        Object(NonMatching, "game/act_force_gem_wooden.cpp"),
        Object(NonMatching, "game/act_force_gem_buried.cpp"),
        Object(NonMatching, "game/act_fire_manager.cpp"),
        Object(NonMatching, "game/act_crystal_ball.cpp"),
        Object(NonMatching, "game/act_slingshot_seed.cpp"),
        Object(NonMatching, "game/act_bombchu.cpp"),
        Object(NonMatching, "game/act_fairy_invincible.cpp"),
        Object(NonMatching, "game/tu_803276C4.cpp"),
        Object(NonMatching, "game/act_cactus.cpp"),
        Object(NonMatching, "game/act_thwomp.cpp"),
        Object(NonMatching, "game/act_force_gem_spawner.cpp"),
        Object(NonMatching, "game/act_force_gem.cpp"),
        Object(NonMatching, "game/act_force_gem_2.cpp"),
        Object(NonMatching, "game/act_cucco.cpp"),
        Object(NonMatching, "game/tu_80333958.cpp"),
        Object(NonMatching, "game/tu_80334318.cpp"),
        Object(NonMatching, "game/act_iron_ball_manager.cpp"),
        Object(NonMatching, "game/act_two_sword_motion.cpp"),
        Object(Matching, "game/act_treasure_mark.cpp"),
        Object(NonMatching, "game/act_thwomp_launch_npc.cpp"),
        Object(NonMatching, "game/act_warp_pillar.cpp"),
        Object(NonMatching, "game/act_flying_tile.cpp"),
        Object(NonMatching, "game/act_post_warp_pillar_handling.cpp"),
        Object(NonMatching, "game/act_fairy_revival.cpp"),
        Object(NonMatching, "game/act_strange_block.cpp"),
        Object(NonMatching, "game/act_lotus.cpp"),
        Object(NonMatching, "game/tu_8033F448.cpp"),
        Object(NonMatching, "game/act_zora_lotus.cpp"),
        Object(NonMatching, "game/act_shadow_link_giant_bomb.cpp"),
        Object(NonMatching, "game/act_giant_bomb.cpp"),
        Object(NonMatching, "game/tu_80343A70.cpp"),
        Object(NonMatching, "game/act_fish.cpp"),
        Object(NonMatching, "game/act_carrot.cpp"),
        Object(Matching, "game/act_zelda_scroll_lock.cpp"),
        Object(NonMatching, "game/act_special_bg_draw_object.cpp"),
        Object(NonMatching, "game/tu_803489A4.cpp"),
        Object(NonMatching, "game/act_bee_manager.cpp"),
        Object(NonMatching, "game/tu_8034A6B0.cpp"),
        Object(NonMatching, "game/tu_8034AA40.cpp"),
        Object(NonMatching, "game/tu_8034C950.cpp"),
        Object(NonMatching, "game/tu_8034DFF0.cpp"),
        Object(NonMatching, "game/act_obj_lift.cpp"),
        Object(NonMatching, "game/act_bandit_herding_npc.cpp"),
        Object(Matching, "game/act_side_view_gate_display_npc.cpp"),
        Object(NonMatching, "game/act_tingle_time_up_manager.cpp"),
        Object(NonMatching, "game/tu_803536C4.cpp"),
        Object(NonMatching, "game/act_vaati_time_up.cpp"),
        Object(Matching, "game/act_force_gem_shadow.cpp"),
        Object(NonMatching, "game/tu_80354D64.cpp"),
        Object(NonMatching, "game/act_thwomp_launch_npc_2.cpp"),
        Object(NonMatching, "game/act_npc_ride_switch.cpp"),
        Object(NonMatching, "game/act_log_monitor_npc.cpp"),
        Object(NonMatching, "game/act_seesaw_lift.cpp"),
        Object(NonMatching, "game/act_enemy_for_ending_control.cpp"),
        Object(NonMatching, "game/act_maiden_ending.cpp"),
        Object(NonMatching, "game/act_princess_zelda_ending.cpp"),
        Object(NonMatching, "game/act_link_ending.cpp"),
        Object(NonMatching, "game/act_kaepora_gaebora_ending.cpp"),
        Object(NonMatching, "game/tu_80362634.cpp"),
        Object(NonMatching, "game/act_navi_maiden_crystal.cpp"),
        Object(NonMatching, "game/act_crystal_demo_2.cpp"),
        Object(NonMatching, "game/act_four_person_pull_switch.cpp"),
        Object(NonMatching, "game/act_leaf_effect_npc_generator.cpp"),
        Object(NonMatching, "game/act_leaf_effect_npc.cpp"),
        Object(NonMatching, "game/act_link_ending_2.cpp"),
        Object(NonMatching, "game/tu_8036FB60.cpp"),
        Object(Matching, "game/act_tutorial_old_man.cpp"),
        Object(NonMatching, "game/act_four_person_pull_switch_pswb.cpp"),
        Object(NonMatching, "game/act_four_person_pull_switch_pswk.cpp"),
        Object(NonMatching, "game/act_colour_block.cpp"),
        Object(NonMatching, "game/act_eyeball_switch.cpp"),
        Object(NonMatching, "game/act_invisible_npc_wall_tomi.cpp"),
        Object(Matching, "game/act_block_for_2_3_players.cpp"),
        Object(NonMatching, "game/act_invisible_npc_wall.cpp"),
        Object(NonMatching, "game/tu_80376FA0.cpp"),
        Object(NonMatching, "game/act_horse_race_end_area.cpp"),
        Object(NonMatching, "game/act_ice_melted_by_four_players.cpp"),
        Object(NonMatching, "game/act_push_switch_in_pyramid.cpp"),
        Object(NonMatching, "game/act_warp_entrance_wall.cpp"),
        Object(NonMatching, "game/tu_80379F84.cpp"),
        Object(NonMatching, "game/tu_8037ACC4.cpp"),
        Object(NonMatching, "game/act_old_man.cpp"),
        Object(NonMatching, "game/act_magic_jar.cpp"),
        Object(NonMatching, "game/tu_8037DF68.cpp"),
        Object(NonMatching, "game/act_great_fairy_bfry.cpp"),
        Object(NonMatching, "game/act_talking_sign.cpp"),
        Object(NonMatching, "game/tu_8039003C.cpp"),
        Object(NonMatching, "game/tu_80390818.cpp"),
        Object(NonMatching, "game/act_score.cpp"),
        Object(NonMatching, "game/act_talking_npc.cpp"),
        Object(NonMatching, "game/tu_803944C8.cpp"),
        Object(NonMatching, "game/tu_803A3484.cpp"),
        Object(NonMatching, "game/tu_803A60E4.cpp"),
        Object(NonMatching, "game/tu_803A85E4.cpp"),
        Object(NonMatching, "game/tu_803A8DFC.cpp"),
        Object(NonMatching, "game/tu_803B7DCC.cpp"),
        Object(Matching, "game/act_talking_npc_2.cpp"),
        Object(NonMatching, "game/tu_803B8C74.cpp"),
        Object(NonMatching, "game/tu_803B9940.cpp"),
        Object(NonMatching, "game/tu_803BACF8.cpp"),
        Object(NonMatching, "game/tu_803BC6F0.cpp"),
        Object(NonMatching, "game/tu_803D2C34.cpp"),
        Object(NonMatching, "game/act_battle_royale_end_check.cpp"),
        Object(NonMatching, "game/tu_803D7098.cpp"),
        Object(NonMatching, "game/tu_803D9944.cpp"),
        Object(NonMatching, "game/tu_803DA6B4.cpp"),
        Object(NonMatching, "game/tu_803DAC3C.cpp"),
        Object(NonMatching, "game/tu_803E1668.cpp"),
        Object(NonMatching, "game/tu_803F0A54.cpp"),
        Object(NonMatching, "game/tu_803FF2C4.cpp"),
        Object(NonMatching, "game/tu_80400DC4.cpp"),
        Object(NonMatching, "game/tu_80401848.cpp"),
        Object(NonMatching, "game/tu_804023D0.cpp"),
        Object(NonMatching, "game/tu_80403D84.cpp"),
        Object(NonMatching, "game/tu_80414F6C.cpp"),
        Object(NonMatching, "game/tu_80418B64.cpp"),
        Object(NonMatching, "game/tu_8042165C.cpp"),
        # THP player with sms THPDec.c / THPAudio.c inside (docs/LIBMAP.md):
        # deferred inlining, reversed function and data order.
        Object(NonMatching, "game/tu_80427D2C.cpp", extra_cflags=["-inline off,deferred"]),
        Object(NonMatching, "game/tu_8042E964.cpp"),
        Object(NonMatching, "game/tu_80432638.cpp"),
        Object(NonMatching, "game/tu_80434370.cpp"),
        Object(NonMatching, "game/act_gba_appear.cpp"),
        Object(NonMatching, "game/act_life_meter.cpp"),
        Object(NonMatching, "game/tu_80438C68.cpp"),
        Object(NonMatching, "game/tu_8043A0E8.cpp"),
        Object(NonMatching, "game/tu_80440BF4.cpp"),
        Object(NonMatching, "game/tu_80441318.cpp"),
        Object(NonMatching, "game/tu_80441A1C.cpp"),
        Object(NonMatching, "game/tu_80442810.cpp"),
        Object(NonMatching, "game/act_dummy_conversation.cpp"),
        Object(NonMatching, "game/tu_804432DC.cpp"),
        Object(NonMatching, "game/tu_80443B0C.cpp"),
        Object(NonMatching, "game/tu_804455E0.cpp"),
        Object(NonMatching, "game/tu_804465B8.cpp"),
        Object(NonMatching, "game/tu_80447618.cpp"),
        Object(NonMatching, "game/tu_80447FAC.cpp"),
        Object(NonMatching, "game/tu_80448940.cpp"),
        Object(NonMatching, "game/tu_8044998C.cpp"),
        Object(NonMatching, "game/tu_8044A828.cpp"),
        Object(NonMatching, "game/tu_8044B80C.cpp"),
        Object(NonMatching, "game/tu_8044C028.cpp"),
        Object(NonMatching, "game/tu_8044C7F0.cpp"),
        Object(NonMatching, "game/tu_80453AF8.cpp"),
        Object(NonMatching, "game/tu_8045560C.cpp"),
        Object(NonMatching, "game/tu_80455CA4.cpp"),
        Object(NonMatching, "game/tu_80457C08.cpp"),
        Object(NonMatching, "game/tu_80457DFC.cpp"),
        Object(NonMatching, "game/tu_8045A640.cpp"),
        Object(NonMatching, "game/tu_8045B010.cpp"),
        Object(Matching, "game/tu_8045B6F0.cpp"),
        Object(Matching, "game/tu_8045BA84.cpp"),
    ]),
]


config.custom_build_rules = []
config.custom_build_steps = {}

# Grab the specific GameID so we can format our strings properly
version = VERSIONS[version_num]
out_dir = config.build_dir / version

# report.json counts a Matching (complete) unit with no compiled object as
# 100 % unseen and honors objdiff.json symbol_mappings. Our tools
# (progress.py, gate.py, coord.py) read report_honest.json, where no unit is
# complete and no mapping applies; see tools/honest_report.py.
honest_objdiff = (config.objdiff_path if config.objdiff_path and config.objdiff_path.is_file()
                  else config.build_dir / "tools" / "objdiff-cli")
config.custom_build_rules.append({
    "name": "honest_report",
    "command": f"$python tools/honest_report.py {honest_objdiff} $out $objdiff_report_args",
    "description": "REPORT $out",
})
# tools/link_check.py --unit records trial-link evidence here; adding or
# replacing a record (by rename) changes the directory's mtime, so the report
# reruns with it.
config.custom_build_rules.append({
    "name": "mkdir",
    "command": "mkdir -p $out",
    "description": "MKDIR $out",
})
config.custom_build_steps["post-build"] = [{
    "rule": "mkdir",
    "outputs": out_dir / "weakdedup_evidence",
}, {
    "rule": "honest_report",
    "outputs": out_dir / "report_honest.json",
    # The link map and DOL: weak-duplicate stripping is proven only from the
    # current link, and only while its DOL matches build.sha1.
    "implicit": ["objdiff.json", "all_source", "tools/honest_report.py", honest_objdiff,
                 out_dir / "fsa_game_code.elf.MAP", out_dir / "fsa_game_code.dol",
                 out_dir / "weakdedup_evidence"],
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


# Progress categories: the same regions as the progress site
# (tools/site_data.py REGIONS).
config.progress_categories = [
    ProgressCategory("game", "Game code"),
    ProgressCategory("jsystem", "JSystem"),
    ProgressCategory("sdk", "Dolphin SDK"),
    ProgressCategory("gx", "GX"),
    ProgressCategory("runtime", "MSL, Runtime, TRK"),
]

# dtk's auto units (the unsplit code, most of it the game's) have no Object,
# so they get their category from the address of their code, the way
# tools/site_data.py places unsplit functions: config/G4SE01/lib_map.tsv's
# library ranges, then the game region (and lib_map's "game" row), then the
# library just before the address.
GAME_TEXT = (0x800F39EC, 0x8045BD00)


def _lib_map():
    rows = []
    path = Path("config") / config.version / "lib_map.tsv"
    if path.is_file():
        import csv
        with open(path, encoding="utf-8") as f:
            for row in csv.DictReader(f, delimiter="\t"):
                if row["text_start"].startswith("0x"):
                    rows.append((int(row["text_start"], 16), int(row["text_end"], 16), row["library"]))
    return rows


def _library_category(library: str) -> Optional[str]:
    if library == "game":
        return "game"
    if library == "SDK:gx":
        return "gx"
    if library.startswith("SDK:"):
        return "sdk"
    if library.startswith("JSystem:") or library == "J3D":
        return "jsystem"
    if library in ("MSL", "Runtime", "TRK"):
        return "runtime"
    return None


def _first_function_name(obj_path: Optional[str]) -> Optional[str]:
    """First function symbol of a (big-endian ELF32) dtk split object."""
    import struct
    try:
        data = Path(obj_path).read_bytes() if obj_path else b""
    except OSError:
        return None
    if data[:4] != b"\x7fELF":
        return None
    shoff, = struct.unpack_from(">I", data, 0x20)
    shentsize, shnum = struct.unpack_from(">HH", data, 0x2E)
    sections = [struct.unpack_from(">IIIIIIIIII", data, shoff + i * shentsize) for i in range(shnum)]
    for sec in sections:
        if sec[1] != 2:  # SHT_SYMTAB
            continue
        strtab = sections[sec[6]]
        for off in range(sec[4] + 16, sec[4] + sec[5], 16):
            name, _, _, info = struct.unpack_from(">IIIB", data, off)
            if info & 0xF == 2:  # STT_FUNC
                start = strtab[4] + name
                return data[start:data.index(b"\0", start)].decode()
    return None


def _text_symbols() -> Dict[str, int]:
    out: Dict[str, int] = {}
    path = Path("config") / config.version / "symbols.txt"
    if path.is_file():
        for line in path.read_text(encoding="utf-8").splitlines():
            name, _, rest = line.partition(" = .text:0x")
            if rest:
                out.setdefault(name, int(rest[:8], 16))
    return out


_category_tables: Dict[str, Any] = {}


def auto_progress_category(unit_name: str, obj_path: Optional[str]) -> Optional[str]:
    import re
    m = re.search(r"_([0-9A-F]{8})_(text|init)$", unit_name)
    if m:
        addr = int(m.group(1), 16)
    elif unit_name.endswith("_text"):
        if "symbols" not in _category_tables:
            _category_tables["symbols"] = _text_symbols()
        addr = _category_tables["symbols"].get(_first_function_name(obj_path) or "")
        if addr is None:
            return None
    else:
        return None  # data-only auto units: no code to count
    if "lib_map" not in _category_tables:
        _category_tables["lib_map"] = _lib_map()
    rows = _category_tables["lib_map"]
    hit = next((lib for start, end, lib in rows if start <= addr < end), None)
    if hit is not None and hit != "game":
        return _library_category(hit)
    if hit == "game" or GAME_TEXT[0] <= addr < GAME_TEXT[1]:
        return "game"
    # A gap between lib_map ranges joins the library before it (as on the site).
    before = [lib for start, _, lib in rows if start <= addr]
    return _library_category(before[-1]) if before else None


config.auto_progress_category = auto_progress_category
config.progress_each_module = args.verbose
# Relocation diffing: "all" compares the target symbol's name (or address)
# and, for data targets, its value. objdiff's default "name_address" skips the
# value; "data_value" ignores names entirely, so a call or load of a
# differently named symbol would still count as matched. The same setting goes
# into objdiff.json, so the GUI and `objdiff-cli diff` agree with the report.
config.objdiff_options = {"functionRelocDiffs": "all"}
# Optional extra arguments to `objdiff-cli report generate`
config.progress_report_args = [
    "--config functionRelocDiffs=all",
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
