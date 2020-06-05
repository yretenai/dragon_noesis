//
// Created by yretenai on 6/2/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_SHAREDSTRUCTURES_H
#define FMT_LUMBERYARD_SHAREDSTRUCTURES_H

#include <stdint.h>

namespace dragon::lumberyard::chunk::model {
#pragma pack(push, 1)
    struct VERSION_STRUCT {
        uint32_t A;
        uint32_t B;
        uint32_t C;
        uint32_t D;
    };

    struct VECTOR3_SINGLE {
        float X;
        float Y;
        float Z;
    };

    struct CRCH_CHUNK_HEADER {
        enum CRCH_CHUNK_TYPE : uint16_t {
            ModelChunkMesh = 0x1000,
            ModelChunkMaterialList = 0x1006,
            ModelChunkNode = 0x100B,
            ModelChunkMaterial = 0x100C,
            ModelChunkMeta = 0x1013,
            ModelChunkMaterialName = 0x1014,
            ModelChunkFlags = 0x1015,
            ModelChunkData = 0x1016,
            ModelChunkSubmesh = 0x1017
        };

        CRCH_CHUNK_TYPE Type;
        uint16_t Version;
        uint32_t Id;
        uint32_t Size;
        uint32_t Pointer;
    };

    struct CRCH_HEADER {
        uint32_t Magic;
        uint32_t Version;
        uint32_t ChunkCount;
        uint32_t ChunkTablePointer;
    };
#pragma pack(pop)
} // namespace dragon::lumberyard::chunk::model

#endif // FMT_LUMBERYARD_SHAREDSTRUCTURES_H
