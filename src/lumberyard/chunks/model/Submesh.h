//
// Created by yretenai on 6/3/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_SUBMESH_H
#define FMT_LUMBERYARD_SUBMESH_H

#include "AbstractModelChunk.h"

namespace dragon::lumberyard::chunk::model {
    class LUMBERYARD_EXPORT Submesh : public AbstractModelChunk {
      public:
        Submesh(std::vector<char> buffer, CRCH_CHUNK_HEADER chunk_header);

        SUBMESH_HEADER Header;
        std::vector<SUBMESH_DATA> Submeshes;
        std::vector<SUBMESH_BONE> Bones;
        std::vector<float> TexelDensity;
    };
} // namespace dragon::lumberyard::chunk::model

#endif // FMT_LUMBERYARD_SUBMESH_H
