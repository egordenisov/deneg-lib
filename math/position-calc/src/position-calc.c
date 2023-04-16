#include "position-calc.h"

#include <math.h>

bool position_accel_to_rad (accels_t accels, angles_t* angles) {
    angles->alpha = atan2f ( accels.x, sqrtf(accels.y  * accels.y + accels.z  * accels.z));
    angles->beta  = atan2f ( accels.y, sqrtf(accels.x  * accels.x + accels.z  * accels.z));
    angles->gamma = atan2f ( accels.z, sqrtf(accels.x  * accels.x + accels.y  * accels.y));
    
    return true;
}

bool position_accel_to_deg (accels_t accels, angles_t* angles) {
    angles->alpha = atan2f ( accels.x, sqrtf(accels.y  * accels.y + accels.z  * accels.z)) * 180 / M_PI;
    angles->beta  = atan2f ( accels.y, sqrtf(accels.x  * accels.x + accels.z  * accels.z)) * 180 / M_PI;
    angles->gamma = atan2f ( accels.z, sqrtf(accels.x  * accels.x + accels.y  * accels.y)) * 180 / M_PI;
    
    return true;
}