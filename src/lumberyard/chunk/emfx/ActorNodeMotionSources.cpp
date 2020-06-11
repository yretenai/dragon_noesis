//
// Created by yretenai on 2020-06-09.
//

#include "ActorNodeMotionSources.h"

namespace dragon::lumberyard::chunk::emfx {
    ActorNodeMotionSources::ActorNodeMotionSources(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 1);
        Chunk = header;
        ptr = Align(ptr, 4);
        uint32_t count = buffer->lpcast<uint32_t>(&ptr);
        Indices = buffer->lpcast<uint16_t>(&ptr, count);
        Axes = buffer->lpcast<uint8_t>(&ptr, count);
        MirrorFlags = buffer->lpcast<uint8_t>(&ptr, count);
    }
} // namespace dragon::lumberyard::chunk::emfx
