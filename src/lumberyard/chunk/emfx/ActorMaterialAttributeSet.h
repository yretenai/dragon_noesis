//
// Created by yretenai on 2020-06-10.
//

#pragma once

#ifndef FMT_LUMBERYARD_ACTORMATERIALATTRIBUTESET_H
#define FMT_LUMBERYARD_ACTORMATERIALATTRIBUTESET_H

#include "AbstractEMFXChunk.h"
#include "ActorMaterialAttribute.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorMaterialAttributeSet : public AbstractEMFXChunk {
      public:
        ActorMaterialAttributeSet(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        ACTOR_MATERIAL_ATTRIBUTE_SET_V1_HEADER Header;

        Array<std::shared_ptr<ActorMaterialAttribute>> Attributes;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif // FMT_LUMBERYARD_ACTORMATERIALATTRIBUTESET_H
