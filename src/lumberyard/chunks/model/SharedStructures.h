//
// Created by yretenai on 6/2/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_MODEL_SHAREDSTRUCTURES_H
#define FMT_LUMBERYARD_MODEL_SHAREDSTRUCTURES_H

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

    struct VECTOR4_SINGLE {
        float X;
        float Y;
        float Z;
        float W;
    };

    struct MATRIX44_SINGLE {
        float Matrix[4][4];
    };

    struct BOUNDING_BOX_SINGLE {
        VECTOR3_SINGLE Min;
        VECTOR3_SINGLE Max;
    };

    typedef int32_t CHUNK_ID;

    struct CRCH_CHUNK_HEADER {
        enum struct TYPE : uint16_t { Mesh = 0x1000, Node = 0x100B, MaterialName = 0x1014, Flags = 0x1015, DataStream = 0x1016, Submesh = 0x1017 };

        TYPE Type;
        uint16_t Version;
        CHUNK_ID Id;
        uint32_t Size;
        uint32_t Pointer;
    };

    struct CRCH_HEADER {
        uint32_t Magic;
        uint32_t Version;
        uint32_t ChunkCount;
        uint32_t ChunkTablePointer;
    };

    struct DATA_STREAM_HEADER {
        enum struct TYPE : uint32_t {
            Position,
            Normal,
            UV,
            Color,
            Color2,
            Index,
            Tangent,
            ShapeCoefficient,
            ShapeDefromation,
            BoneMap,
            FaceMap,
            VertexMaterial,
            QTangent,
            SkinData,
            Dummy,
            CombinedPositionColorUV,
            NumTypes
        };

        int32_t Flags;
        TYPE StreamType;
        int32_t StreamIndex;
        int32_t Count;
        int32_t Size;
        int32_t Reserved[2];
    };

    struct EXPORT_FLAGS_HEADER {
        enum struct FLAGS : uint32_t {
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
        CHUNK_ID ToolId;
        uint32_t ToolVersion;
    };

    struct MESH_HEADER {
        enum struct FLAGS : uint32_t { IsEmpty = 1, HasTexMappingDensity = 2, ExtraWeights = 4, FaceArea = 8 };

        FLAGS Flags;
        uint32_t Flags2;

        uint32_t Vertices;
        uint32_t Indices;
        uint32_t Submeshes;

        CHUNK_ID SubmeshChunkId;
        CHUNK_ID VertexAnimId;
        CHUNK_ID StreamChunkId[static_cast<int>(DATA_STREAM_HEADER::TYPE::NumTypes)][8];
        CHUNK_ID PhysicsDataChunkId[4];

        BOUNDING_BOX_SINGLE BoundingBox;

        uint32_t TexMappingDensity;
        uint32_t GeometricMeanFaceArea;
        uint32_t Reserved[31];
    };

    struct NODE_HEADER {
        CHUNK_ID ObjectId;
        CHUNK_ID ParentId;
        uint32_t SubNodeCount;
        CHUNK_ID MaterialId;

        uint8_t IsGroupHead;
        uint8_t IsGroupMember;
        uint16_t Padding;

        MATRIX44_SINGLE Transform;
        VECTOR3_SINGLE Position;
        VECTOR4_SINGLE Rotation;
        VECTOR3_SINGLE Scale;

        CHUNK_ID PositionControllerId;
        CHUNK_ID RotationControllerId;
        CHUNK_ID ScaleControllerId;

        uint32_t PropertyStringLength;
    };

    struct SUBMESH_HEADER {
        enum struct FLAGS : uint32_t {
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
} // namespace dragon::lumberyard::chunk::model

#endif // FMT_LUMBERYARD_MODEL_SHAREDSTRUCTURES_H
