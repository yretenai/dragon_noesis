//
// Created by yretenai on 2020/6/8.
//

#pragma once

#ifndef DRAGON_LUMBERYARD_EMFX_ABSTRACTEMFXCHUNK_H
#define DRAGON_LUMBERYARD_EMFX_ABSTRACTEMFXCHUNK_H

#include "standard_dragon/dragon.h"
#include "../../export.h"
#include "SharedStructures.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT AbstractEMFXChunk {
      public:
        EMFX_CHUNK_HEADER Chunk;
        virtual ~AbstractEMFXChunk() noexcept {}
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // DRAGON_LUMBERYARD_EMFX_ABSTRACTEMFXCHUNK_H
