#ifndef SHDATA_H
#define SHDATA_H

#include <stdint.h>

typedef struct output_data_type {
    uint32_t dig_out;       /* 32 digital outputs */
    uint32_t ana_out[10];   /* 10 analog outputs */
} out_t;

typedef struct input_data_type {
    uint32_t dig_in;        /* 32 digital inputs */
    uint32_t ana_in[10];    /* 10 analog inputs */
} in_t;

#endif /* SHDATA_H */