//
// Created by yretenai on 2020-06-09.
//

#pragma once

#ifndef DRAGON_LUMBERYARD_EMFX_ACTORMATERIALINFO_H
#define DRAGON_LUMBERYARD_EMFX_ACTORMATERIALINFO_H

#include "AbstractEMFXChunk.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorMaterialInfo : public AbstractEMFXChunk {
      public:
        ActorMaterialInfo(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        ACTOR_MATERIAL_INFO_V1_HEADER Header;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // DRAGON_LUMBERYARD_EMFX_ACTORMATERIALINFO_H
