//
// Created by yretenai on 2020-06-09.
//

#pragma once

#ifndef FMT_LUMBERYARD_EMFX_ACTORMATERIAL_H
#define FMT_LUMBERYARD_EMFX_ACTORMATERIAL_H

#include "AbstractEMFXChunk.h"
#include "ActorMaterialLayer.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorMaterial : public AbstractEMFXChunk {
      public:
        ActorMaterial(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        ACTOR_MATERIAL_V1_HEADER Header;

        std::string Name;
        Array<std::shared_ptr<ActorMaterialLayer>> Layers;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // FMT_LUMBERYARD_EMFX_ACTORMATERIAL_H
