//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_MODEL_H
#define FMT_LUMBERYARD_MODEL_H

#include "Lumberyard.h"
#include "chunks/model/AbstractModelChunk.h"

#define FOURCC_CRCH (MAKEFOURCC('C', 'r', 'C', 'h'))
namespace dragon::lumberyard {
    class LUMBERYARD_EXPORT Model {
      public:
#pragma pack(push, 1)
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

        Model(std::vector<char> buffer);

        CRCH_HEADER Header;
        std::vector<CRCH_CHUNK_HEADER> ChunkTable;
        std::map<uint32_t, std::shared_ptr<chunk::model::AbstractModelChunk>>
            Chunks;

        bool get_chunk_header(uint32_t id, CRCH_CHUNK_HEADER& chunk);

        static bool check(std::vector<char> buffer);

#ifdef USE_NOESIS

        static noesisModel_t* noesis_load(BYTE* buffer, int length,
                                          int& num_mdl, noeRAPI_t* rapi);

        static bool noesis_check(BYTE* buffer, int length,
                                 [[maybe_unused]] noeRAPI_t* rapi);

#endif // USE_NOESIS
    };
} // namespace dragon::lumberyard

#endif // FMT_LUMBERYARD_MODEL_H
