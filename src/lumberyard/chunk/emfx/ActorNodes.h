//
// Created by yretenai on 2020-06-09.
//

#pragma once

#ifndef DRAGON_LUMBERYARD_EMFX_ACTORNODES_H
#define DRAGON_LUMBERYARD_EMFX_ACTORNODES_H

#include "AbstractEMFXChunk.h"
#include "ActorNode.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorNodes : public AbstractEMFXChunk {
      public:
        ActorNodes(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        ACTOR_NODES_V1_HEADER Header;

        Array<std::shared_ptr<ActorNode>> Nodes;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // DRAGON_LUMBERYARD_EMFX_ACTORNODES_H
