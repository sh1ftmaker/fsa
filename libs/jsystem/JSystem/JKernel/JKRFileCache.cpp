#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRFileFinder.h"
#include "JSystem/JKernel/JKRFileCache.h"

// FSA never mounts a JKRFileCache, so the linker strips the whole file except
// the weak JKRFileFinder destructor and vtable that getFirstFile instantiates.
// Those come first in the link, so the copies in JKRFileFinder.cpp are dropped.
// Only getFirstFile is imported; the rest of the file (mount, getResource, ...,
// getDvdPathName) is not.

JKRDvdFinder * JKRFileCache::getFirstFile(const char *path) const
{
    char *dvdPathName = getDvdPathName(path);
    JKRDvdFinder * fileFinder = new (JKRGetSystemHeap(), 0) JKRDvdFinder(dvdPathName);
    JKRFreeToSysHeap(dvdPathName);
    if (fileFinder->isAvailable()) {
        return fileFinder;
    }
    delete fileFinder;
    return nullptr;
}
