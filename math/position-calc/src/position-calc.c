#include "position-calc.h"

#include <math.h>
#include <string.h>

static void calculate_roll_pitch (angles_t* angles, accels_t accels) {
    angles->roll  = atan2f ( accels.x, sqrtf(accels.y * accels.y + accels.z * accels.z));
    angles->pitch = atan2f ( accels.y, sqrtf(accels.x * accels.x + accels.z * accels.z));
} 

static void calculate_yaw (uint32_t delta_us, float* yaw, float ang_vel_z) {
    *yaw += delta_us * ang_vel_z / 1000000;
}

bool position_step (position_t* ctx, accels_t acc_raw, gyro_t gyr_raw, uint64_t timestamp) {
    memcpy(&ctx->acc, &acc_raw, sizeof(accels_t));
    memcpy(&ctx->gyr, &gyr_raw, sizeof(gyro_t));

    calculate_roll_pitch (&ctx->ang, ctx->acc);

    calculate_yaw (timestamp - ctx->timestamp, &ctx->ang.yaw, ctx->gyr.z);

    ctx->timestamp = timestamp;
    return ctx->valid;
}


bool position_get_rad (position_t* ctx, angles_t* angles) {
    memcpy(angles, &ctx->ang, sizeof(angles_t));
    return ctx->valid;
}

bool position_get_deg (position_t* ctx, angles_t* angles) {
    memcpy(angles, &ctx->ang, sizeof(angles_t));
    angles->roll  = angles->roll  * 180 / M_PI;
    angles->pitch = angles->pitch * 180 / M_PI;
    angles->yaw   = angles->yaw * 180 / M_PI;

    return ctx->valid;
}