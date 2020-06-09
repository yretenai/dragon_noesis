//
// Created by yretenai on 2020-06-09.
//

#include "ActorNodeMotionSources.h"

namespace dragon::lumberyard::chunk::emfx {
    ActorNodeMotionSources::ActorNodeMotionSources(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr) {
        assert(header.Version <= 1);
        Chunk = header;
        Header = buffer->lpcast<ACTOR_NODE_MOTION_SOURCES_V1_HEADER>(&ptr);
        Indices = buffer->lpcast<uint16_t>(&ptr, Header.NumNodes);
        Axes = buffer->lpcast<uint8_t>(&ptr, Header.NumNodes);
        MirrorFlags = buffer->lpcast<uint8_t>(&ptr, Header.NumNodes);
    }
} // namespace dragon::lumberyard::chunk::emfx
