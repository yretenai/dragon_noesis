//
// Created by yretenai on 6/2/2020.
//

#ifndef FMT_DRAGON_MODELFLAGS_H
#define FMT_DRAGON_MODELFLAGS_H

#include "../../Lumberyard.h"

class LUMBERYARD_EXPORT ModelFlags : AbstractModelChunk {
public:
    ModelFlags(vector<char> data);
    ~ModelFlags();
};


#endif //FMT_DRAGON_MODELFLAGS_H
