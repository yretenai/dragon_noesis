//
// Created by yretenai on 6/2/2020.
//

#pragma once

#ifndef FMT_LUMBERYARD_SHAREDSTRUCTURES_H
#define FMT_LUMBERYARD_SHAREDSTRUCTURES_H

#include <stdint.h>

namespace dragon::lumberyard::chunk::model {
    struct VERSION_STRUCT {
        uint32_t A;
        uint32_t B;
        uint32_t C;
        uint32_t D;
    };

    struct VECTOR3_SINGLE {
        float X;
        float Y;
        float Z;
    };
} // namespace dragon::lumberyard::chunk::model

#endif // FMT_LUMBERYARD_SHAREDSTRUCTURES_H
