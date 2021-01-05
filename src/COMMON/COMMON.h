//
// Created by SHKim on 2020-11-12.
//

#ifndef SYSTEMC_NULLHOP_COMMON_H
#define SYSTEMC_NULLHOP_COMMON_H

#include <systemc.h>
#include "Debug.h"
//////////////// GLOBAL CONFIGURATION /////////////
constexpr size_t FIXED_POINT_PRECISION = 16;

//////////////// CCM CONFIGURATION ////////////////
constexpr size_t NUMBER_OF_MAC = 128;
constexpr size_t KERNEL_BANK_SIZE = 4608;

//////////////// IDP CONFIGURATION ////////////////
constexpr size_t PIXEL_MEM_SIZE = 524288;

//////////////// SDRAM CONFIGURATION ////////////////
constexpr size_t SDRAM_SIZE = 256;//in Megabytes
constexpr int SDRAM_tAA = 0;
constexpr int SDRAM_tAA = 0;
constexpr int SDRAM_tAA = 0;
constexpr int SDRAM_tAA = 0;


#endif //SYSTEMC_NULLHOP_COMMON_H
