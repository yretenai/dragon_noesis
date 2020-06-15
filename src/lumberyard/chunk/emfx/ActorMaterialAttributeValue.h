//
// Created by yretenai on 2020-06-10.
//

#pragma once

#ifndef DRAGON_LUMBERYARD_ACTORMATERIALATTRIBUTEVALUE_H
#define DRAGON_LUMBERYARD_ACTORMATERIALATTRIBUTEVALUE_H

#include "AbstractEMFXChunk.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorMaterialAttributeValue : public AbstractEMFXChunk {
      public:
        ActorMaterialAttributeValue(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        ACTOR_MATERIAL_ATTRIBUTE_V1_HEADER Header;

        std::string Name;
        Array<char> Buffer;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // DRAGON_LUMBERYARD_ACTORMATERIALATTRIBUTEVALUE_H
