//
// Created by lando on 2023-03-21.
//

#include "int_conversion.h"

uint32_t buildInt32(const uint8_t *buf) {
    return (buf[0] << 16) | (buf[1] << 8) | buf[2];
}

void splitInt32(uint32_t number, uint8_t *buf) {
    buf[0] = (number >> 16) & 0xff;
    buf[1] = (number >> 8) & 0xff;
    buf[2] = number & 0xff;
}