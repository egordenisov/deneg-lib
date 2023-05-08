#ifndef __TMP112_REGS_H
#define __TMP112_REGS_H

#include "stdint.h"

#define TMP112_REGS_TEMP    (0x00)
#define TMP112_REGS_TLOW    (0x02)
#define TMP112_REGS_THIGH   (0x03)
#define TMP112_REGS_CONFIG  (0x01)

#define TMP112_REGS_CONFIG_FAUTLT_QUEUE_1   (0x00) // Default
#define TMP112_REGS_CONFIG_FAUTLT_QUEUE_2   (0x01)
#define TMP112_REGS_CONFIG_FAUTLT_QUEUE_4   (0x02)
#define TMP112_REGS_CONFIG_FAUTLT_QUEUE_6   (0x03)

#define TMP112_REGS_CONFIG_CR_025Hz         (0x00)
#define TMP112_REGS_CONFIG_CR_1Hz           (0x01)
#define TMP112_REGS_CONFIG_CR_4Hz           (0x02) // Default
#define TMP112_REGS_CONFIG_CR_8Hz           (0x03)

typedef union {
    struct {
        uint16_t reserved0  : 4;
        uint16_t em         : 1;
        uint16_t al         : 1;
        uint16_t cr         : 2;
        uint16_t sd         : 1;
        uint16_t tm         : 1;
        uint16_t pol        : 1;
        uint16_t fault      : 2;
        uint16_t resolution : 2;
        uint16_t os         : 1;
    } bf;
    uint16_t word;
} tmp112_regs_config_t;

#define TMP112_UNSIG_RESOLUTION     (TMP112_RESOLUTION - 1)
#define TMP112_RESOLUTION_MASK       ((1 << TMP112_RESOLUTION) - 1)
#define TMP112_UNSIG_RESOLUTION_MASK ((1 << (TMP112_RESOLUTION - 1)) - 1)

#endif // __TMP112_REGS_H