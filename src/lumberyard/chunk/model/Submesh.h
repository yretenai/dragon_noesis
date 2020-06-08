//
// Created by yretenai on 6/3/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_MODEL_SUBMESH_H
#define FMT_LUMBERYARD_MODEL_SUBMESH_H

#include "AbstractModelChunk.h"

namespace dragon::lumberyard::chunk::model {
    class LUMBERYARD_EXPORT Submesh : public AbstractModelChunk {
      public:
        Submesh(Array<char>* buffer, CRCH_CHUNK_HEADER chunk_header);

        SUBMESH_HEADER Header;
        Array<SUBMESH_DATA> Submeshes;
        Array<SUBMESH_BONE> Bones;
        Array<float> TexelDensity;
    };
} // namespace dragon::lumberyard::chunk::model

#endif // FMT_LUMBERYARD_MODEL_SUBMESH_H
