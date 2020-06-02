//
// Created by yretenai on 5/29/2020.
//

#pragma once

#ifndef FMT_DRAGON_MODELSTRUCTS_H
#define FMT_DRAGON_MODELSTRUCTS_H

#pragma pack(push, 1)

#define FOURCC_CRCH (MAKEFOURCC('C','r','C','h'))

typedef enum CRCH_CHUNK_TYPE : uint16_t {
    ModelChunkMesh = 0x1000,
    // ModelChunk = 0x1001,
    // ModelChunk = 0x1002,
    // ModelChunk = 0x1003,
    // ModelChunk = 0x1004,
    // ModelChunk = 0x1005,
    ModelChunkMaterialList = 0x1006,
    // ModelChunk = 0x1007,
    // ModelChunk = 0x1008,
    // ModelChunk = 0x1009,
    // ModelChunk = 0x100A,
    ModelChunkNode = 0x100B,
    ModelChunkMaterial = 0x100C,
    // ModelChunk = 0x100D,
    // ModelChunk = 0x100E,
    // ModelChunk = 0x100F,
    // ModelChunk = 0x1010,
    // ModelChunk = 0x1011,
    // ModelChunk = 0x1012,
    ModelChunkMeta = 0x1013,
    ModelChunkMaterialName = 0x1014,
    ModelChunkFlags = 0x1015,
    ModelChunkData = 0x1016,
    ModelChunkGeometry = 0x1017,
    // ModelChunk = 0x1018,
} CrChChunkType;

typedef struct CRCH_CHUNK_HEADER {
    CrChChunkType Type;
    uint16_t Version;
    uint32_t Id;
    uint32_t Size;
    uint32_t Pointer;
} CrChChunkHeader;

typedef struct CRCH_HEADER {
    uint32_t Magic;
    uint32_t Version;
    uint32_t ChunkCount;
    uint32_t ChunkTablePointer;
} CrChHeader;

#pragma pack(pop)

#endif //FMT_DRAGON_MODELSTRUCTS_H
