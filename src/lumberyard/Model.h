//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_MODEL_H
#define FMT_LUMBERYARD_MODEL_H

#include "../dragon.h"
#include "chunks/model/ModelChunks.h"
#include "export.h"

#define FOURCC_CRCH (MAKEFOURCC('C', 'r', 'C', 'h'))
namespace dragon::lumberyard {
    class LUMBERYARD_EXPORT Model {
      public:
        Model(Array<char>* buffer);

        chunk::model::CRCH_HEADER Header;
        Array<chunk::model::CRCH_CHUNK_HEADER> ChunkTable;
        std::map<uint32_t, std::shared_ptr<chunk::model::AbstractModelChunk>>
            Chunks;

        bool get_chunk_header(uint32_t id,
                              chunk::model::CRCH_CHUNK_HEADER& chunk);

        static bool check(Array<char>* buffer);

#ifdef USE_NOESIS

        static noesisModel_t* noesis_load(BYTE* buffer, int length,
                                          int& num_mdl, noeRAPI_t* rapi);

        static bool noesis_check(BYTE* buffer, int length,
                                 [[maybe_unused]] noeRAPI_t* rapi);

#endif // USE_NOESIS
    };
} // namespace dragon::lumberyard

#endif // FMT_LUMBERYARD_MODEL_H
