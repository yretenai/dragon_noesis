//
// Created by yretenai on 2020-06-10.
//

#ifndef FMT_DRAGON_ACTORMATERIALATTRIBUTE_H
#define FMT_DRAGON_ACTORMATERIALATTRIBUTE_H

#include "AbstractEMFXChunk.h"
#include "ActorMaterialAttributeValue.h"

namespace dragon::lumberyard::chunk::emfx {
    class LUMBERYARD_EXPORT ActorMaterialAttribute : public AbstractEMFXChunk {
    public:
        ActorMaterialAttribute(Array<char>* buffer, EMFX_CHUNK_HEADER header, int& ptr);

        uint16_t Flags;
        std::string InternalName;
        std::string Name;
        std::string Description;
        uint32_t InterfaceType;
        Array<std::string> ComboValues;
        std::shared_ptr<ActorMaterialAttributeValue> DefaultValue;
        std::shared_ptr<ActorMaterialAttributeValue> MinimumValue;
        std::shared_ptr<ActorMaterialAttributeValue> MaximumValue;
        std::shared_ptr<ActorMaterialAttributeValue> Value;
    };
} // namespace dragon::lumberyard::chunk::emfx

#endif //FMT_DRAGON_ACTORMATERIALATTRIBUTE_H
