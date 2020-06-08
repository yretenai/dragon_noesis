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

    typedef uint32_t CHUNK_TYPE;

    struct EMFX_CHUNK_HEADER {
        CHUNK_TYPE Type;
        uint32_t Size;
        uint32_t Version;
    };

    enum class ACTOR_CHUNK_TYPE : CHUNK_TYPE {
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

    enum struct UNIT_TYPE : uint8_t { Inches, Feet, Yards, Miles, Milimeters, Centimeters, Decimeters, Meters, Kilometers };

    struct ACTOR_INFO_V1_HEADER {
        uint32_t NumLODs;
        int32_t MotionNodeIndex;
        int32_t RetargetRootNodeIndex;
        float RetargetOffset;
        UNIT_TYPE UnitType;
        uint16_t ExporterVersion;
    };

    struct ACTOR_INFO_V2_HEADER {

        uint32_t NumLODs;
        int32_t MotionNodeIndex;
        int32_t RetargetRootNodeIndex;
        UNIT_TYPE UnitType;
        uint16_t ExporterVersion;
    };

    struct ACTOR_INFO_V3_HEADER {
        ACTOR_INFO_V3_HEADER() {}

        ACTOR_INFO_V3_HEADER(ACTOR_INFO_V2_HEADER header) {
            NumLODs = header.NumLODs;
            MotionNodeIndex = header.MotionNodeIndex;
            RetargetRootNodeIndex = header.RetargetRootNodeIndex;
            UnitType = header.UnitType;
            ExporterVersion = header.ExporterVersion;
        }

        ACTOR_INFO_V3_HEADER(ACTOR_INFO_V1_HEADER header) {
            NumLODs = header.NumLODs;
            MotionNodeIndex = header.MotionNodeIndex;
            RetargetRootNodeIndex = header.RetargetRootNodeIndex;
            UnitType = header.UnitType;
            ExporterVersion = header.ExporterVersion;
        }

        uint32_t NumLODs;
        int32_t MotionNodeIndex;
        int32_t RetargetRootNodeIndex;
        UNIT_TYPE UnitType;
        uint16_t ExporterVersion;
        uint8_t IsOptimized;
    };
#pragma pack(pop)
} // namespace dragon::lumberyard::chunk::emfx

#endif // FMT_LUMBERYARD_EMFX_SHAREDSTRUCTURES_H
