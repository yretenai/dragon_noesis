//
// Created by yretenai on 2020/6/8.
//

#pragma once

#ifndef FMT_LUMBERYARD_EMFX_ABSTRACTEMFXCHUNK_H
#define FMT_LUMBERYARD_EMFX_ABSTRACTEMFXCHUNK_H

#include "../../../dragon/dragon.h"
#include "../../export.h"
#include "SharedStructures.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT AbstractEMFXChunk {
      public:
        EMFX_CHUNK_HEADER Chunk;
        virtual ~AbstractEMFXChunk() noexcept {}
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // FMT_LUMBERYARD_EMFX_ABSTRACTEMFXCHUNK_H
