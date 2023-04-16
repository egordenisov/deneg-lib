#ifndef __POSITION_CALC_H
#define __POSITION_CALC_H

#include "stdbool.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Accels {
    float x;
    float y;
    float z;
} accels_t;

typedef struct Angles {
    float roll;
    float pitch;
    float yaw;
} angles_t;

typedef struct Gyro {
    float x;
    float y;
    float z;
} gyro_t;

typedef struct Position {
    uint64_t timestamp;

    angles_t ang;
    accels_t acc;
    gyro_t   gyr;

    bool valid;

} position_t;

bool position_init (position_t* ctx);
bool position_step (position_t* ctx, accels_t acc_raw, gyro_t gyr_raw, uint64_t timestamp);

bool position_get_rad (position_t* ctx, angles_t* angles);
bool position_get_deg (position_t* ctx, angles_t* angles);


#ifdef __cplusplus
}
#endif

#endif // __POSITION_CALC_H