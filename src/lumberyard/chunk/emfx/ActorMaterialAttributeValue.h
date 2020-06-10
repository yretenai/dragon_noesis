//
// Created by yretenai on 2020-06-10.
//

#ifndef FMT_DRAGON_ACTORMATERIALATTRIBUTEVALUE_H
#define FMT_DRAGON_ACTORMATERIALATTRIBUTEVALUE_H

#include "AbstractEMFXChunk.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorMaterialAttributeValue : public AbstractEMFXChunk {
    public:
        ActorMaterialAttributeValue(Array<char>* buffer, EMFX_CHUNK_HEADER header, int & ptr);

        ACTOR_MATERIAL_ATTRIBUTE_V1_HEADER Header;

        std::string Name;
        Array<char> Buffer;
    };
}

#endif //FMT_DRAGON_ACTORMATERIALATTRIBUTEVALUE_H
