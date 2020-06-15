//
// Created by yretenai on 6/5/2020.
//

#pragma once

#ifndef DRAGON_LUMBERYARD_MODEL_MESH_H
#define DRAGON_LUMBERYARD_MODEL_MESH_H

#include "AbstractModelChunk.h"

namespace dragon::lumberyard::chunk::model {
    class LUMBERYARD_EXPORT Mesh : public AbstractModelChunk {
      public:
        Mesh(Array<char>* buffer, CRCH_CHUNK_HEADER chunk_header);

        MESH_HEADER Header;
    };
} // namespace dragon::lumberyard::chunk::model

#endif // DRAGON_LUMBERYARD_MODEL_MESH_H
