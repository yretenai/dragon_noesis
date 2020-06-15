//
// Created by yretenai on 2020/6/8.
//

#pragma once

#ifndef DRAGON_LUMBERYARD_EMFX_SHAREDSTRUCTURES_H
#define DRAGON_LUMBERYARD_EMFX_SHAREDSTRUCTURES_H

#include "../SharedStructures.h"
#include <stdint.h>

namespace dragon::lumberyard::chunk::emfx {
#pragma pack(push, 1)
    struct ACTOR_HEADER {
        uint32_t Magic;
        uint16_t Version;
        uint8_t IsBigEndian;
    };

    struct MOTION_HEADER {
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
        Material = 3,
        MaterialLayer = 4,
        EffectMaterial = 5,
        Limit = 6,
        Info = 7,
        MeshLOD = 8,
        MorphTarget = 9,
        NodeGroups = 10,
        Nodes = 11,
        MorphTargets = 12,
        MaterialInfo = 13,
        NodeMotionSources = 14,
        AttachmentSources = 15,
        MaterialAttributeSet = 16,
        GenericMaterial = 17,
        PhysicsSetup = 18,
        SimulatedObjectSetup = 19,
        END
    };

    enum class MOTION_CHUNK_TYPE : CHUNK_TYPE {
        MotionEventTable = 50,
        SubMotion = 200,
        Info = 201,
        SubMotions = 202,
        WaveletInfo = 203,
        MorphSubMotions = 204,
        END
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

    struct ACTOR_NODE_V1_HEADER {
        enum struct FLAGS { IncludeInBoundsCalculation = 1, AttachmentNode = 2, Critical = 4 };

        VECTOR4_SINGLE Rotation;
        VECTOR3_SINGLE Position;
        VECTOR3_SINGLE Scale;
        uint32_t SkeletalLOD;
        int32_t ParentIndex;
        int32_t NumChild;
        FLAGS Flags;
        MATRIX44_SINGLE OBB;
    };

    struct ACTOR_NODES_V1_HEADER {
        uint32_t NumNodes;
        uint32_t NumRootNodes;
        BOUNDING_BOX_SINGLE BoundingBox;
    };

    struct ACTOR_MATERIAL_INFO_V1_HEADER {
        uint32_t LOD;
        uint32_t NumTotalMaterials;
        uint32_t NumMaterials;
        uint32_t NumEffectMaterials;
        uint32_t NumGenericMaterials;
    };

    struct ACTOR_MATERIAL_V1_HEADER {
        uint32_t LOD;
        VECTOR4_SINGLE AmbientColor;
        VECTOR4_SINGLE DiffuseColor;
        VECTOR4_SINGLE SpecularColor;
        VECTOR4_SINGLE EmissiveColor;
        float Shine;
        float ShineStrength;
        float Opacity;
        float IOR;
        uint8_t IsDoubleSided;
        uint8_t RenderWireFrame;
        uint8_t TransparencyType;
        uint8_t NumLayers;
    };

    struct ACTOR_MATERIAL_LAYER_V1_HEADER {
        float Amount;
        VECTOR2_SINGLE Offset;
        VECTOR2_SINGLE Tiling;
        float Rotation;
        uint16_t MaterialNumber;
        uint8_t MapType;
        uint8_t BlendMode;
    };

    struct ACTOR_MESH_V1_HEADER {
        uint32_t MeshId;
        uint32_t NodeIndex;
        uint32_t LOD;
        uint32_t NumOriginalVerts;
        uint32_t NumPolygons;
        uint32_t TotalVerts;
        uint32_t TotalIndices;
        uint32_t NumSubMeshes;
        uint32_t NumVBOs;
        uint8_t IsCollisionMesh;
        uint8_t IsTriangleMesh;
    };

    struct ACTOR_VBO_V1_HEADER {
        enum class TYPE : uint32_t { POSITIONS, NORMALS, TANGENTS, UV, COLORS32, VERTEXID, COLORS128, BITANGENTS, CLOTH };

        TYPE LayerType;
        uint32_t AttribSizeInBytes;
        uint8_t EnableDeformations;
        uint8_t ShouldScale;
    };

    struct ACTOR_SUBMESH_V1_HEADER {
        uint32_t NumIndices;
        uint32_t NumVertices;
        uint32_t NumPolygons;
        uint32_t MaterialId;
        uint32_t NumBones;
    };

    struct ACTOR_SKINNING_INFO_v1_HEADER {
        uint32_t NodeIndex;
        uint32_t LOD;
        uint32_t NumBones;
        uint32_t NumTotalInfluences;
        uint32_t IsForCollisionMesh;
    };

    struct ACTOR_SKINNING_INFO_v1_INFLUENCE {
        float Weight;
        uint32_t NodeIndex;
    };

    struct ACTOR_SKINNING_INFO_v1_ENTRY {
        uint32_t StartIndex;
        uint32_t NumElements;
    };

    struct ACTOR_MATERIAL_ATTRIBUTE_SET_V1_HEADER {
        uint32_t MaterialIndex;
        uint32_t LOD;
        uint8_t Version;
        uint32_t NumAttributes;
    };

    struct ACTOR_MATERIAL_ATTRIBUTE_V1_HEADER {
        enum class TYPE : uint32_t {
            FloatSpinner,
            FloatSlider,
            IntSpinner,
            IntSlider,
            ComboBox,
            CheckBox,
            Vector2,
            Gizmo,
            Vector4,
            Quaternion,
            Color,
            String,
            Tag = 26,
            Vector3 = 113212,
            PropertySet = 113213,
            Default = 0xFFFFFFFF
        };

        TYPE Type;
        uint32_t Size;
    };

    struct MOTION_INFO_V3_HEADER {
        uint32_t Flags;
        uint32_t Index;
        UNIT_TYPE UnitType;
        uint8_t IsAdditive;
    };

    struct MOTION_SUBMOTION_V1_HEADER {
        VECTOR4_SHORT RefRotation;
        VECTOR4_SHORT BindRotation;
        VECTOR3_SINGLE RefPosition;
        VECTOR3_SINGLE RefScale;
        VECTOR3_SINGLE BindPosition;
        VECTOR3_SINGLE BindScale;
        uint32_t NumPositionKeys;
        uint32_t NumRotationKeys;
        uint32_t NumScaleKeys;
    };

    struct MOTION_VECTOR3_KEY {
        VECTOR3_SINGLE Value;
        float Time;
    };

    struct MOTION_VECTOR4_KEY {
        VECTOR4_SHORT Value;
        float Time;
    };
#pragma pack(pop)
} // namespace dragon::lumberyard::chunk::emfx

#endif // DRAGON_LUMBERYARD_EMFX_SHAREDSTRUCTURES_H
