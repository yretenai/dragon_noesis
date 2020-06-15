//
// Created by yretenai on 2020-06-09.
//

#pragma once

#ifndef FMT_LUMBERYARD_EMFX_ACTORMESH_H
#define FMT_LUMBERYARD_EMFX_ACTORMESH_H

#include "AbstractEMFXChunk.h"
#include "ActorSubmesh.h"
#include "ActorVertexBuffer.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorMesh : public AbstractEMFXChunk {
      public:
        ActorMesh(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        ACTOR_MESH_V1_HEADER Header;

        Array<std::shared_ptr<ActorVertexBuffer>> VBOs;
        Array<std::shared_ptr<ActorSubmesh>> Submeshes;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // FMT_LUMBERYARD_EMFX_ACTORMESH_H
