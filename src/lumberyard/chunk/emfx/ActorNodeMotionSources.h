//
// Created by yretenai on 2020-06-09.
//

#pragma once

#ifndef DRAGON_LUMBERYARD_EMFX_ACTORNODEMOTIONSOURCES_H
#define DRAGON_LUMBERYARD_EMFX_ACTORNODEMOTIONSOURCES_H

#include "AbstractEMFXChunk.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorNodeMotionSources : public AbstractEMFXChunk {
      public:
        ActorNodeMotionSources(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        Array<uint16_t> Indices;
        Array<uint8_t> Axes;
        Array<uint8_t> MirrorFlags;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // DRAGON_LUMBERYARD_EMFX_ACTORNODEMOTIONSOURCES_H
