//
// Created by yretenai on 2020-06-11.
//

#include "MotionSubMotions.h"

namespace dragon::lumberyard::chunk::emfx {
    MotionSubMotions::MotionSubMotions(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 1);
        Chunk = header;
        uint32_t count = buffer->lpcast<uint32_t>(&ptr);
        Motions = Array<std::shared_ptr<MotionSubMotion>>(count, nullptr);
        EMFX_CHUNK_HEADER nodeHeader = {static_cast<CHUNK_TYPE>(MOTION_CHUNK_TYPE::SubMotion), 0, header.Version};
        for (uint32_t i = 0; i < count; i++) {
            Motions[i] = std::shared_ptr<MotionSubMotion>(new MotionSubMotion(buffer, nodeHeader, ptr));
        }
    }
} // namespace dragon::lumberyard::chunk::emfx
