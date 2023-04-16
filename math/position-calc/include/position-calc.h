#ifndef __POSITION_CALC_H
#define __POSITION_CALC_H

#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Accels {
    float x;
    float y;
    float z;
} accels_t;

typedef struct Angles {
    float alpha;
    float beta;
    float gamma;
} angles_t;

bool position_accel_to_rad (accels_t accels, angles_t* angles);
bool position_accel_to_deg (accels_t accels, angles_t* angles);

#ifdef __cplusplus
}
#endif

#endif // __POSITION_CALC_H