//
// Created by yretenai on 6/3/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_SUBMESH_H
#define FMT_LUMBERYARD_SUBMESH_H

#include "ModelChunks.h"

namespace dragon::lumberyard::chunk::model {
    class LUMBERYARD_EXPORT Submesh : public AbstractModelChunk {
      public:
#pragma pack(push, 1)
        struct SUBMESH_HEADER {
            enum FLAGS : uint32_t {
                HasDecompMat = 0x1,
                BoneIndices = 0x2,
                SubsetTexelDensity = 0x4,
            };

            FLAGS Flags;
            int32_t Count;
            int32_t Reserved[2];
        };

        struct SUBMESH_DATA {
            int32_t FirstIndexId;
            int32_t IndexCount;
            int32_t FirstVertId;
            int32_t VertCount;
            int32_t MaterialId;
            float Radius;
            VECTOR3_SINGLE Center;
        };

        struct SUBMESH_BONE {
            uint32_t Count;
            uint16_t IDs[0x80];
        };
#pragma pack(pop)

        Submesh(std::vector<char> buffer, uint32_t version);

        SUBMESH_HEADER Header;
        std::vector<SUBMESH_DATA> Submeshes;
        std::vector<SUBMESH_BONE> Bones;
        std::vector<float> TexelDensity;
    };
} // namespace dragon::lumberyard::chunk::model

#endif // FMT_LUMBERYARD_SUBMESH_H
