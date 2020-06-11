//
// Created by yretenai on 2020-06-11.
//

#include "MotionSubMotion.h"

namespace dragon::lumberyard::chunk::emfx {
    MotionSubMotion::MotionSubMotion(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 1);
        Header = buffer->lpcast<MOTION_SUBMOTION_V1_HEADER>(&ptr);
        int32_t size = buffer->lpcast<int32_t>(&ptr);
        if (size > 0) {
            Array<char> stringBuffer = buffer->lpslice(&ptr, size);
            Name = std::string(stringBuffer.data(), size);
        }
        Positions = buffer->lpcast<MOTION_VECTOR3_KEY>(&ptr, Header.NumPositionKeys);
        Rotations = buffer->lpcast<MOTION_VECTOR4_KEY>(&ptr, Header.NumRotationKeys);
        Scales = buffer->lpcast<MOTION_VECTOR3_KEY>(&ptr, Header.NumScaleKeys);
    }
} // namespace dragon::lumberyard::chunk::emfx
