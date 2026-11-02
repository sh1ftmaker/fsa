#ifndef JAUDIO_JASTRACKPORT_H
#define JAUDIO_JASTRACKPORT_H

#include "types.h"

class JASTrackPort
{
public:
    static const u32 MAX_PORTS = 16;

    void init();
    u16 readImport(u32 port_num);
    u16 readExport(u32 port_num);
    void writeImport(u32 port_num, u16);
    void writeExport(u32 port_num, u16);
    bool checkImport(u32 port_num) const;
    bool checkExport(u32 port_num) const;

    u16 get(u32 port_num) { return mPortData[port_num]; }
    void set(u32 port_num, u16 data) { mPortData[port_num] = data; }

    u16 mImportFlags;
    u16 mExportFlags;
    u16 mPortData[MAX_PORTS];
};

#endif // JAUDIO_JASTRACKPORT_H
