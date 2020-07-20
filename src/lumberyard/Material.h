//
// Created by yretenai on 6/7/2020.
//

#pragma once

#ifndef DRAGON_LUMBERYARD_MATERIAL_H
#define DRAGON_LUMBERYARD_MATERIAL_H

#include "standard_dragon/dragon.h"
#include "export.h"
#include <tinyxml2/tinyxml2.h>

namespace dragon::lumberyard {
    class LUMBERYARD_EXPORT Material {
      public:
        enum class MATERIAL_FLAGS : uint32_t {
            Wire = 0x0001,
            TwoSided = 0x0002,
            Additive = 0x0004,
            Detail = 0x0008,
            Lightning = 0x0010,
            NoShadow = 0x0020,
            Always = 0x0040,
            Pure = 0x0080,
            MultiSubMaterial = 0x0100,
            NoPhysical = 0x0200,
            NoDraw = 0x0400,
            NoPreview = 0x0800,
            NoInstance = 0x1000,
            CollisionProxy = 0x2000,
            Scatter = 0x4000,
            ForwardRendering = 0x8000,
            NonRemovable = 0x10000,
            HideOnBreak = 0x20000,
            UI = 0x40000,
            ShaderGen = 0x80000,
            Raycast = 0x100000,
            RequireCubemap = 0x200000,
            ConsoleMat = 0x400000,
            Delete = 0x800000,
            Blend = 0x1000000,
            IsTerrain = 0x2000000,
            Issky = 0x4000000,
            Fog = 0x8000000
        };

        Material();
        Material(tinyxml2::XMLElement* xml);

        static Material from_path(std::filesystem::path path);

        std::string Name;
        MATERIAL_FLAGS Flags;

        float DiffuseColor[4] = {1, 1, 1, 1};
        float SpecularColor[4] = {1, 1, 1, 1};

        std::map<std::string, std::filesystem::path> Textures;
        std::vector<Material> SubMaterials;
    };
} // namespace dragon::lumberyard

#endif // DRAGON_LUMBERYARD_MATERIAL_H
