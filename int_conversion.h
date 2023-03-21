//
// Created by lando on 2023-03-21.
//

#ifndef clox_int_conversion_h
#define clox_int_conversion_h

#include "common.h"

void splitInt32(uint32_t number, uint8_t *buf);
uint32_t buildInt32(const uint8_t *buf);

#endif
