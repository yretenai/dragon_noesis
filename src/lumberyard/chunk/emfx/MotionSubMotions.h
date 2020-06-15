//
// Created by yretenai on 2020-06-11.
//

#pragma once

#ifndef FMT_LUMBERYARD_MOTIONSUBMOTIONS_H
#define FMT_LUMBERYARD_MOTIONSUBMOTIONS_H

#include "AbstractEMFXChunk.h"
#include "MotionSubMotion.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT MotionSubMotions : public AbstractEMFXChunk {
      public:
        MotionSubMotions(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        Array<std::shared_ptr<MotionSubMotion>> Motions;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // FMT_LUMBERYARD_MOTIONSUBMOTIONS_H
