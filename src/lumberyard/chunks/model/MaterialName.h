//
// Created by yretenai on 6/2/2020.
//

#ifndef FMT_LUMBERYARD_MATERIALNAME_H
#define FMT_LUMBERYARD_MATERIALNAME_H

#include "../../Lumberyard.h"

namespace dragon::lumberyard::chunk::model {
    class LUMBERYARD_EXPORT MaterialName : public AbstractModelChunk {
    public:
        MaterialName(vector<char> buffer, uint32_t version);

        string Name;
        vector<int32_t> Types;
        vector<string> Materials;
    };
}

#endif //FMT_LUMBERYARD_MATERIALNAME_H
