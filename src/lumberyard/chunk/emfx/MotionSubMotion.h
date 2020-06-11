//
// Created by yretenai on 2020-06-11.
//

#ifndef FMT_DRAGON_MOTIONSUBMOTION_H
#define FMT_DRAGON_MOTIONSUBMOTION_H

#include "AbstractEMFXChunk.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT MotionSubMotion : public AbstractEMFXChunk {
      public:
        MotionSubMotion(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        MOTION_SUBMOTION_V1_HEADER Header;
        std::string Name;
        Array<MOTION_VECTOR3_KEY> Positions;
        Array<MOTION_VECTOR4_KEY> Rotations;
        Array<MOTION_VECTOR3_KEY> Scales;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // FMT_DRAGON_MOTIONSUBMOTION_H
