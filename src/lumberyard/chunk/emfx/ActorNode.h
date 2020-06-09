//
// Created by yretenai on 2020-06-09.
//

#ifndef FMT_LUMBERYARD_EMFX_ACTORNODE_H
#define FMT_LUMBERYARD_EMFX_ACTORNODE_H

#include "AbstractEMFXChunk.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorNode : public AbstractEMFXChunk {
      public:
        ActorNode(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        ACTOR_NODE_V1_HEADER Header;
        std::string Name;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // FMT_LUMBERYARD_EMFX_ACTORNODE_H
