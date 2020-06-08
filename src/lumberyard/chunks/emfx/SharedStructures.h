//
// Created by yretenai on 2020/6/8.
//

#pragma once

#ifndef FMT_LUMBERYARD_EMFX_SHAREDSTRUCTURES_H
#define FMT_LUMBERYARD_EMFX_SHAREDSTRUCTURES_H

#include <stdint.h>

namespace dragon::lumberyard::chunk::emfx {
#pragma pack(push, 1)
    struct ACTOR_HEADER {
        uint32_t Magic;
        uint16_t Version;
        uint8_t IsBigEndian;
    };

    typedef uint32_t CHUNK_ID;

    struct EMFX_CHUNK_HEADER {
        CHUNK_ID ChunkID;
        uint32_t Size;
        uint32_t Version;
    };

    enum class ACTOR_CHUNK_TYPE : CHUNK_ID {
        Node = 0,
        Mesh = 1,
        SkinningInfo = 2,
        StandardMaterial = 3,
        StandardMaterialLayer = 4,
        EffectMaterial = 5,
        Limit = 6,
        Info = 7,
        MeshLOD = 8,
        StandardProgMorphTargets = 9,
        NodeGroups = 10,
        Nodes = 11,
        StandardMorphTargets = 12,
        MaterialInfo = 13,
        NodeMotionSources = 14,
        AttachmentSources = 15,
        MaterialAttributeSet = 16,
        GenericMaterial = 17,
        PhysicsSetup = 18,
        SimulatedObjectSetup = 19
    };
#pragma pack(pop)
} // namespace dragon::lumberyard::chunk::emfx

#endif // FMT_LUMBERYARD_EMFX_SHAREDSTRUCTURES_H
