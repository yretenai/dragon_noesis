//
// Created by yretenai on 6/7/2020.
//

#include "Material.h"

using namespace tinyxml2;

namespace dragon::lumberyard {
    Material::Material(XMLElement* xml) {
        if (xml == nullptr)
            return;

        const char* name = xml->Attribute("Name");
        if (name != nullptr) {
            Name = std::string(name);
        }
        Flags = (MATERIAL_FLAGS)xml->Unsigned64Attribute("MtlFlags", 0);

        if (((uint64_t)Flags & (uint64_t)MATERIAL_FLAGS::MultiSubMaterial) == (uint64_t)MATERIAL_FLAGS::MultiSubMaterial) {
            XMLElement* sub_materials = xml->FirstChildElement("SubMaterials");
            if (sub_materials != nullptr) {
                XMLElement* sub_material = sub_materials->FirstChildElement("Material");
                while (sub_material != nullptr) {
                    SubMaterials.push_back(Material(sub_material));
                    sub_material = sub_material->NextSiblingElement("Material");
                }
            }
        }

        XMLElement* textures = xml->FirstChildElement("Textures");
        if (textures != nullptr) {
            XMLElement* texture = textures->FirstChildElement("Texture");
            while (texture != nullptr) {
                const char* key_attr = texture->Attribute("Map");
                if (key_attr == nullptr)
                    continue;
                const char* value_attr = texture->Attribute("File");
                if (value_attr == nullptr)
                    continue;
                std::string key(key_attr);
                std::filesystem::path value(value_attr);
                Textures[key] = value;
                texture = texture->NextSiblingElement("Texture");
            }
        }
    }

    Material Material::from_path(std::filesystem::path path) {
        XMLDocument doc;
        if (doc.LoadFile(path.string().c_str()) == XML_SUCCESS) {
            return Material(doc.RootElement());
        }
        return Material(nullptr);
    }
}; // namespace dragon::lumberyard
