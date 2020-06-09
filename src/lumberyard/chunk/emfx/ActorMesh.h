//
// Created by yretenai on 2020-06-09.
//

#ifndef FMT_LUMBERYARD_EMFX_ACTORMESH_H
#define FMT_LUMBERYARD_EMFX_ACTORMESH_H

#include "AbstractEMFXChunk.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorMesh : public AbstractEMFXChunk {
    public:
        ActorMesh(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        ACTOR_MESH_V1_HEADER Header;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // FMT_LUMBERYARD_EMFX_ACTORMESH_H
