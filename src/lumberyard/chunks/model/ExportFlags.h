//
// Created by yretenai on 6/2/2020.
//

#ifndef FMT_LUMBERYARD_EXPORTFLAGS_H
#define FMT_LUMBERYARD_EXPORTFLAGS_H

#include "../../Lumberyard.h"
#include "SharedStructures.h"

namespace dragon::lumberyard::chunk::model {
    class LUMBERYARD_EXPORT ExportFlags : public AbstractModelChunk {
    public:
#pragma pack(push, 1)
        struct EXPORT_FLAGS_HEADER {
            enum FLAGS : uint32_t {
                MergeAllNodes = 0x1,
                HaveAllLods = 0x2,
                CustomNormals = 0x4,
                SingleVertex = 0x8,
                EightWeights = 0x10,
                Skinned = 0x20,
                ExportedFromXsi = 0x1001,
                ExportedFromMax = 0x1002,
                ExportedFromMaya = 0x1003
            };

            FLAGS Flags;
            VERSION_STRUCT Version;
            char VersionString[16];
            uint32_t ToolId;
            uint32_t ToolVersion;
        };
#pragma pack(pop)

        ExportFlags(vector<char> buffer, uint32_t version);

        EXPORT_FLAGS_HEADER Header;
    };
}

#endif //FMT_LUMBERYARD_EXPORTFLAGS_H
