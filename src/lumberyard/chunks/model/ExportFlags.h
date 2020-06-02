//
// Created by yretenai on 6/2/2020.
//

#ifndef FMT_DRAGON_EXPORTFLAGS_H
#define FMT_DRAGON_EXPORTFLAGS_H

#include "../../Lumberyard.h"
#include "SharedStructures.h"

class LUMBERYARD_EXPORT ExportFlags : public AbstractModelChunk {
public:
    enum FLAGS : uint32_t {
        MergeAllNodes = 0x1,
        HaveAllLods = 0x2,
        CustomNormals = 0x4,
        SingleVertex = 0x8,
        EightWeights = 0x10,
        Skinned = 0x20,
        ExportedFromXSI = 0x1001,
        ExportedFromMax = 0x1002,
        ExportedFromMaya = 0x1003
    };

    ExportFlags(vector<char> buffer);

    FLAGS Flags;
    VersionStruct Version;
    string VersionString;
    uint32_t ToolId;
    uint32_t ToolVersion;
};


#endif //FMT_DRAGON_EXPORTFLAGS_H
