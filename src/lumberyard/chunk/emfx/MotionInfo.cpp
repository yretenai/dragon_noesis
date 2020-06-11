//
// Created by yretenai on 2020-06-11.
//

#include "MotionInfo.h"

namespace dragon::lumberyard::chunk::emfx {
    MotionInfo::MotionInfo(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 3);
        ptr = Align(ptr, 4);
        Header = buffer->lpcast<MOTION_INFO_V3_HEADER>(&ptr);
        ptr = Align(ptr, 4);
    }
} // namespace dragon::lumberyard::chunk::emfx
