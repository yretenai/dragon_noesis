//
// Created by yretenai on 2020-06-10.
//

#include "ActorMaterialAttribute.h"

namespace dragon::lumberyard::chunk::emfx {
    ActorMaterialAttribute::ActorMaterialAttribute(Array<char> *buffer, EMFX_CHUNK_HEADER header, int &ptr) {
        Chunk = header;

        uint8_t version = buffer->lpcast<uint8_t>(&ptr);
        if(version >= 2) {
            Flags = buffer->lpcast<uint16_t>(&ptr);
        }

        int32_t size = buffer->lpcast<int32_t>(&ptr);
        if (size > 0) {
            Array<char> stringBuffer = buffer->lpslice(&ptr, size);
            InternalName = std::string(stringBuffer.data(), size);
        }

        size = buffer->lpcast<int32_t>(&ptr);
        if (size > 0) {
            Array<char> stringBuffer = buffer->lpslice(&ptr, size);
            Name = std::string(stringBuffer.data(), size);
        }

        size = buffer->lpcast<int32_t>(&ptr);
        if (size > 0) {
            Array<char> stringBuffer = buffer->lpslice(&ptr, size);
            Description = std::string(stringBuffer.data(), size);
        }

        InterfaceType = buffer->lpcast<uint32_t>(&ptr);
        ComboValues = Array<std::string>(buffer->lpcast<uint32_t>(&ptr));
        for(size_t i = 0; i < ComboValues.size(); i++) {
            size = buffer->lpcast<int32_t>(&ptr);
            if (size > 0) {
                Array<char> stringBuffer = buffer->lpslice(&ptr, size);
                ComboValues[i] = std::string(stringBuffer.data(), size);
            }
        }

        DefaultValue = std::shared_ptr<ActorMaterialAttributeValue>(new ActorMaterialAttributeValue(buffer, header, ptr));
        MinimumValue = std::shared_ptr<ActorMaterialAttributeValue>(new ActorMaterialAttributeValue(buffer, header, ptr));
        MaximumValue = std::shared_ptr<ActorMaterialAttributeValue>(new ActorMaterialAttributeValue(buffer, header, ptr));
        Value = std::shared_ptr<ActorMaterialAttributeValue>(new ActorMaterialAttributeValue(buffer, header, ptr));
    }
}
