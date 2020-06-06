//
// Created by yretenai on 6/5/2020.
//

#ifndef FMT_LUMBERYARD_MESH_H
#define FMT_LUMBERYARD_MESH_H

#include "AbstractModelChunk.h"

namespace dragon::lumberyard::chunk::model {
    class LUMBERYARD_EXPORT Mesh : public AbstractModelChunk {
      public:
        Mesh(Array<char>* buffer, CRCH_CHUNK_HEADER chunk_header);

        MESH_HEADER Header;
    };
} // namespace dragon::lumberyard::chunk::model

#endif // FMT_LUMBERYARD_MESH_H
