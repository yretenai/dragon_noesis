//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_ACTOR_H
#define FMT_LUMBERYARD_ACTOR_H

#include "../dragon/dragon.h"
#include "chunks/emfx/EMFXChunks.h"
#include "export.h"

#define FOURCC_ACTR (MAKEFOURCC('A', 'C', 'T', 'R'))
namespace dragon::lumberyard {
    class LUMBERYARD_EXPORT Actor {
      public:
        Actor(Array<char>* buffer);

        chunk::emfx::ACTOR_HEADER Header;
        Array<chunk::emfx::EMFX_CHUNK_HEADER> ChunkTable;
        std::map<chunk::emfx::CHUNK_ID, std::shared_ptr<chunk::emfx::AbstractEMFXChunk>> Chunks;

        bool get_chunk_header(chunk::emfx::CHUNK_ID id, chunk::emfx::EMFX_CHUNK_HEADER& chunk);

        void get_chunks_of_type(chunk::emfx::ACTOR_CHUNK_TYPE type, std::vector<std::shared_ptr<chunk::emfx::AbstractEMFXChunk>>* chunks);

        static bool check(Array<char>* buffer);

#ifdef USE_NOESIS

        static noesisModel_t* noesis_load(BYTE* buffer, int length, int& num_mdl, noeRAPI_t* rapi);

        static bool noesis_check(BYTE* buffer, int length, [[maybe_unused]] [[maybe_unused]] noeRAPI_t* rapi);

        static int noesis_create_texture(std::filesystem::path texturePath, CArrayList<noesisTex_t*>& texList, noeRAPI_t* rapi);

#endif // USE_NOESIS
    }; // namespace dragon::lumberyard
} // namespace dragon::lumberyard
#endif // FMT_LUMBERYARD_ACTOR_H
