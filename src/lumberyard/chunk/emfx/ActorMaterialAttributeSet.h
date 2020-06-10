//
// Created by yretenai on 2020-06-10.
//

#ifndef FMT_DRAGON_ACTORMATERIALATTRIBUTESET_H
#define FMT_DRAGON_ACTORMATERIALATTRIBUTESET_H

#include "AbstractEMFXChunk.h"
#include "ActorMaterialAttribute.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorMaterialAttributeSet : public AbstractEMFXChunk {
        public:
            ActorMaterialAttributeSet(Array < char > *buffer, EMFX_CHUNK_HEADER header, int & ptr);

            ACTOR_MATERIAL_ATTRIBUTE_SET_V1_HEADER Header;

            Array<std::shared_ptr<ActorMaterialAttribute>> Attributes;
    };
}

#endif //FMT_DRAGON_ACTORMATERIALATTRIBUTESET_H
