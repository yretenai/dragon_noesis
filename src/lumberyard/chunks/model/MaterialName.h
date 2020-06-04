//
// Created by yretenai on 6/2/2020.
//

#ifndef FMT_LUMBERYARD_MATERIALNAME_H
#define FMT_LUMBERYARD_MATERIALNAME_H

#include "../../Lumberyard.h"

class LUMBERYARD_EXPORT MaterialName : public AbstractModelChunk {
public:
    MaterialName(vector<char> buffer);

    string Name;
    vector<int32_t> Types;
    vector<string> Materials;
};


#endif //FMT_LUMBERYARD_MATERIALNAME_H
