//
// Created by yretenai on 2020-06-09.
//

#ifndef FMT_LUMBERYARD_SHAREDSTRUCTURES_H
#define FMT_LUMBERYARD_SHAREDSTRUCTURES_H

struct VECTOR3_SINGLE {
    float X;
    float Y;
    float Z;
};

struct VECTOR4_SINGLE {
    float X;
    float Y;
    float Z;
    float W;
};

struct MATRIX44_SINGLE {
    float Matrix[4][4];
};

struct BOUNDING_BOX_SINGLE {
    VECTOR3_SINGLE Min;
    VECTOR3_SINGLE Max;
};

#endif // FMT_LUMBERYARD_SHAREDSTRUCTURES_H
