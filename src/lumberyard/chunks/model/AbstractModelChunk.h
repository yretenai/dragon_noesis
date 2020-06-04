//
// Created by yretenai on 6/2/2020.
//

#ifndef FMT_DRAGON_ABSTRACTMODELCHUNK_H
#define FMT_DRAGON_ABSTRACTMODELCHUNK_H

#include "../../export.h"

namespace dragon::lumberyard::chunk::model {
    class LUMBERYARD_EXPORT AbstractModelChunk {
    public:
        virtual ~AbstractModelChunk() noexcept {
        }
    };
}

#endif //FMT_DRAGON_ABSTRACTMODELCHUNK_H
