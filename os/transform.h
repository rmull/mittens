#ifndef TRANSFORM_H_
#define TRANSFORM_H_

uint16_t transform_int_proportional(uint16_t input, uint16_t in_x, uint16_t out_y);
uint16_t transform_int_linear(uint16_t input, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
uint16_t * transform_interpolate(uint16_t *lut, uint16_t nmemb, uint16_t input);

#endif
