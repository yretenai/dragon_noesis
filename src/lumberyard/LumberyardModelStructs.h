//
// Created by yretenai on 5/29/2020.
//

#pragma once

#ifndef FMT_DRAGON_LUMBERYARDMODELSTRUCTS_H
#define FMT_DRAGON_LUMBERYARDMODELSTRUCTS_H

#define FOURCC_CRCH (MAKEFOURCC('C','r','C','h'))

typedef enum LUMBERYARD_CRCH_CHUNK_TYPE : uint16_t {
    LumberyardModelChunkMesh = 0x1000,
    // LumberyardModelChunk = 0x1001,
    // LumberyardModelChunk = 0x1002,
    // LumberyardModelChunk = 0x1003,
    // LumberyardModelChunk = 0x1004,
    // LumberyardModelChunk = 0x1005,
    LumberyardModelChunkMaterialList = 0x1006,
    // LumberyardModelChunk = 0x1007,
    // LumberyardModelChunk = 0x1008,
    // LumberyardModelChunk = 0x1009,
    // LumberyardModelChunk = 0x100A,
    LumberyardModelChunkNode = 0x100B,
    LumberyardModelChunkMaterial = 0x100C,
    // LUMBERYARD_MODEL_CHUNK = 0x100D,
    // LumberyardModelChunk = 0x100E,
    // LumberyardModelChunk = 0x100F,
    // LumberyardModelChunk = 0x1010,
    // LumberyardModelChunk = 0x1011,
    // LumberyardModelChunk = 0x1012,
    LumberyardModelChunkMeta = 0x1013,
    LumberyardModelChunkMaterialName = 0x1014,
    LumberyardModelChunkFlags = 0x1015,
    LumberyardModelChunkData = 0x1016,
    LumberyardModelChunkGeometry = 0x1017,
    // LumberyardModelChunk = 0x1018,
} CrChChunkType;

typedef struct LUMBERYARD_CRCH_CHUNK_HEADER {
    CrChChunkType Type;
    uint16_t Version;
    uint32_t Id;
    uint32_t Size;
    uint32_t Pointer;
} CrChChunkHeader;

typedef struct LUMBERYARD_CRCH_HEADER {
    uint32_t Magic;
    uint32_t Version;
    uint32_t ChunkCount;
    uint32_t ChunkTablePointer;
} CrChHeader;

#endif //FMT_DRAGON_LUMBERYARDMODELSTRUCTS_H
