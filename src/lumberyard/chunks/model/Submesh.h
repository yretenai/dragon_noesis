//
// Created by yretenai on 6/3/2020.
//

#ifndef FMT_DRAGON_SUBMESH_H
#define FMT_DRAGON_SUBMESH_H

#include "../../Lumberyard.h"
#include "SharedStructures.h"

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

    Submesh(vector<char> buffer);

    SUBMESH_HEADER Header;
    vector<SUBMESH_DATA> Submeshes;
    vector<SUBMESH_BONE> Bones;
    vector<float> TexelDensity;
};


#endif //FMT_DRAGON_SUBMESH_H
