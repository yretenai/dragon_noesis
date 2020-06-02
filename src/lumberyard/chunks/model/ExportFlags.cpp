//
// Created by yretenai on 6/2/2020.
//

#include "ExportFlags.h"

ExportFlags::ExportFlags(vector<char> buffer) {
    char* ptr = buffer.data();
    Flags = reinterpret_cast<ExportFlags::FLAGS*>(ptr)[0];
    ptr += sizeof(ExportFlags::FLAGS);
    Version = reinterpret_cast<VersionStruct*>(ptr)[0];
    ptr += sizeof(VersionStruct);
    VersionString = string(ptr, ptr + 16);
    ptr += 16;
    ToolId = reinterpret_cast<uint32_t*>(ptr)[0];
    ptr += sizeof(uint32_t);
    ToolVersion = reinterpret_cast<uint32_t*>(ptr)[0];
}
