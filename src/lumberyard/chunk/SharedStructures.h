//
// Created by yretenai on 2020-06-09.
//

#ifndef FMT_LUMBERYARD_SHAREDSTRUCTURES_H
#define FMT_LUMBERYARD_SHAREDSTRUCTURES_H

struct VECTOR2_SINGLE {
    float X;
    float Y;
};

struct VECTOR3_SINGLE {
    float X;
    float Y;
    float Z;
};

struct VECTOR3_USHORT {
    uint16_t X;
    uint16_t Y;
    uint16_t Z;
};

struct VECTOR3_BYTE {
    uint8_t X;
    uint8_t Y;
    uint8_t Z;
};

struct VECTOR4_BYTE {
    uint8_t X;
    uint8_t Y;
    uint8_t Z;
    uint8_t W;
};

struct VECTOR4_SHORT {
    int16_t X;
    int16_t Y;
    int16_t Z;
    int16_t W;
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
