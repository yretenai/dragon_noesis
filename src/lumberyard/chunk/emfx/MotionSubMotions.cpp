//
// Created by yretenai on 2020-06-11.
//

#include "MotionSubMotions.h"

namespace dragon::lumberyard::chunk::emfx {
    MotionSubMotions::MotionSubMotions(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 1);
        uint32_t count = buffer->lpcast<uint32_t>(&ptr);
        Motions = Array<std::shared_ptr<MotionSubMotion>>(count);
        for (uint32_t i = 0; i < count; i++) {
            Motions[i] = std::shared_ptr<MotionSubMotion>(new MotionSubMotion(buffer, header, ptr));
        }
    }
} // namespace dragon::lumberyard::chunk::emfx
