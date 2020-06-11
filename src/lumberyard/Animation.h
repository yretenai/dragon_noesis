//
// Created by yretenai on 5/28/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_ANIMATION_H
#define FMT_LUMBERYARD_ANIMATION_H

#include "../dragon/dragon.h"
#include "chunk/emfx/EMFXChunks.h"
#include "export.h"

#define FOURCC_MOT (MAKEFOURCC('M', 'O', 'T', ' '))
#define FOURCC_MOTW (MAKEFOURCC('M', 'O', 'T', 'W'))
namespace dragon::lumberyard {
    class LUMBERYARD_EXPORT Animation {
      public:
        Animation();
        Animation(Array<char>* buffer);

        chunk::emfx::MOTION_HEADER Header;
        std::vector<std::shared_ptr<chunk::emfx::AbstractEMFXChunk>> Chunks;

        void get_chunks_of_type(chunk::emfx::MOTION_CHUNK_TYPE type, std::vector<std::shared_ptr<chunk::emfx::AbstractEMFXChunk>>* chunks);
        std::shared_ptr<chunk::emfx::AbstractEMFXChunk> get_chunk(chunk::emfx::MOTION_CHUNK_TYPE type);

        static bool check(Array<char>* buffer);

        static VECTOR4_SINGLE uncompress_quaternion(VECTOR4_SHORT compressed);
    };
} // namespace dragon::lumberyard

#endif // FMT_LUMBERYARD_ANIMATION_H
