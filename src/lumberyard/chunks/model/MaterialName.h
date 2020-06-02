//
// Created by yretenai on 6/2/2020.
//

#ifndef FMT_DRAGON_MATERIALNAME_H
#define FMT_DRAGON_MATERIALNAME_H

#include "../../Lumberyard.h"

class LUMBERYARD_EXPORT MaterialName : AbstractModelChunk {
public:
    MaterialName(vector<char> buffer);

    ~MaterialName();

    string Name;
    vector<string> Materials;
};


#endif //FMT_DRAGON_MATERIALNAME_H
