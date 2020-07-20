//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef DRAGON_LUMBERYARD_MODEL_H
#define DRAGON_LUMBERYARD_MODEL_H

#include "standard_dragon/dragon.h"
#include "chunk/model/ModelChunks.h"
#include "export.h"

#define FOURCC_CRCH (MAKEFOURCC('C', 'r', 'C', 'h'))
namespace dragon::lumberyard {
    class LUMBERYARD_EXPORT Model {
      public:
        Model();
        Model(Array<char>* buffer);

        chunk::model::CRCH_HEADER Header;
        Array<chunk::model::CRCH_CHUNK_HEADER> ChunkTable;
        std::map<chunk::model::CHUNK_ID, std::shared_ptr<chunk::model::AbstractModelChunk>> Chunks;

        bool get_chunk_header(chunk::model::CHUNK_ID id, chunk::model::CRCH_CHUNK_HEADER& chunk);

        void get_chunks_of_type(chunk::model::CRCH_CHUNK_HEADER::TYPE type, std::vector<std::shared_ptr<chunk::model::AbstractModelChunk>>* chunks);

        static bool check(Array<char>* buffer);

#ifdef USE_NOESIS

        static noesisModel_t* noesis_load(BYTE* buffer, int length, int& num_mdl, noeRAPI_t* rapi);

        static bool noesis_check(BYTE* buffer, int length, [[maybe_unused]] noeRAPI_t* rapi);

        static int noesis_create_texture(std::filesystem::path texturePath, CArrayList<noesisTex_t*>& texList, bool alpha, noeRAPI_t* rapi);

#endif // USE_NOESIS
    };
} // namespace dragon::lumberyard

#endif // DRAGON_LUMBERYARD_MODEL_H
