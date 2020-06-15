//
// Created by yretenai on 2020-06-09.
//

#pragma once

#ifndef DRAGON_LUMBERYARD_EMFX_ACTORMATERIALLAYER_H
#define DRAGON_LUMBERYARD_EMFX_ACTORMATERIALLAYER_H

#include "AbstractEMFXChunk.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorMaterialLayer : public AbstractEMFXChunk {
      public:
        ActorMaterialLayer(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        ACTOR_MATERIAL_LAYER_V1_HEADER Header;

        std::string Name;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // DRAGON_LUMBERYARD_EMFX_ACTORMATERIALLAYER_H
