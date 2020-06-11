//
// Created by yretenai on 2020-06-10.
//

#ifndef FMT_DRAGON_ACTORSUBMESH_H
#define FMT_DRAGON_ACTORSUBMESH_H

#include "AbstractEMFXChunk.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorSubmesh : public AbstractEMFXChunk {
      public:
        ActorSubmesh(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        ACTOR_SUBMESH_V1_HEADER Header;

        Array<uint32_t> Indices;
        Array<uint8_t> Polygons;
        Array<uint32_t> Bones;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // FMT_DRAGON_ACTORSUBMESH_H
