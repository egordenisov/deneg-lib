#ifndef __TMP112_REGS_H
#define __TMP112_REGS_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TMP112_REGS_TEMP    (0x00)
#define TMP112_REGS_TLOW    (0x02)
#define TMP112_REGS_THIGH   (0x03)
#define TMP112_REGS_CONFIG  (0x01)

#define TMP112_REGS_CONFIG_DEFAULT (0x60A0)

typedef union {
    struct {
        uint16_t reserved0  : 4; // Not used in TMP112
        uint16_t em         : 1; // Extended mode (extend to 13-bits) Not used in this driver
        uint16_t al         : 1; // Alert. Read-only. 
        uint16_t cr         : 2; // Convertion rate. 0.25, 1, 4 or 8 Hz. Default 4 Hz
        uint16_t sd         : 1; // Shutdown mode
        uint16_t tm         : 1; // Thermostat mode
        uint16_t pol        : 1; // Polarity of alert pin
        uint16_t fq         : 2; // Fault queue. 1, 2, 4 or 6. Default 1
        uint16_t resolution : 2; // Resolution. Read-only
        uint16_t os         : 1; // One-shot.
    } bf;
    uint16_t word;
} tmp112_regs_config_t;

#define TMP112_UNSIG_RESOLUTION         (TMP112_RESOLUTION - 1)
#define TMP112_RESOLUTION_MASK          ((1 << TMP112_RESOLUTION) - 1)
#define TMP112_UNSIG_RESOLUTION_MASK    ((1 << (TMP112_RESOLUTION - 1)) - 1)

#define TMP112_LSB_PER_CELSIUS          (16)

#ifdef __cplusplus
}
#endif

#endif // __TMP112_REGS_H