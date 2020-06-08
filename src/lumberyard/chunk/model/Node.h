//
// Created by yretenai on 6/5/2020.
//

#ifndef FMT_LUMBERYARD_MODEL_NODE_H
#define FMT_LUMBERYARD_MODEL_NODE_H

#include "AbstractModelChunk.h"

namespace dragon::lumberyard::chunk::model {
    class LUMBERYARD_EXPORT Node : public AbstractModelChunk {
      public:
        Node(Array<char>* buffer, CRCH_CHUNK_HEADER chunk_header);

        std::string Name;
        std::string Property;
        NODE_HEADER Header;
    };
} // namespace dragon::lumberyard::chunk::model

#endif // FMT_LUMBERYARD_MODEL_NODE_H
