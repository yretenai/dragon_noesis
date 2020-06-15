//
// Created by yretenai on 2020-06-11.
//

#pragma once

#ifndef DRAGON_LUMBERYARD_MOTIONINFO_H
#define DRAGON_LUMBERYARD_MOTIONINFO_H

#include "AbstractEMFXChunk.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT MotionInfo : public AbstractEMFXChunk {
      public:
        MotionInfo(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        MOTION_INFO_V3_HEADER Header;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // DRAGON_LUMBERYARD_MOTIONINFO_H
