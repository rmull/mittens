#include <stdint.h>
#include <stdlib.h>

#include "transform.h"

/* TODO: Untested */

/*
 * Mapping of an input to an output where the output has a linear/proportional
 * relationship to the input. This version uses integer math, so scale the
 * inputs accordingly.
 *
 * This version is "proportional" in that it assumes the minimum values for both
 * input and output are zero, meaning we don't need to calculate a slope. Use
 * this for "typical" ADC/DAC functions.
 */
uint16_t
transform_int_proportional(uint16_t input, uint16_t in_x, uint16_t out_y)
{
    /* Prevent divide-by-zero */
    if (in_x == 0) {
        return 0;
    }

    /* Cast to uint32_t to preserve precision */
    return (uint16_t)(((uint32_t)out_y * input) / in_x);
}

/*
 * This function works the same as transform_int_proportional(), except it is
 * able to operate on mappings that may not both start at zero. It uses the
 * point-slope form and needs the min/max values for both input ranges in order
 * to calculate slope.
 */
uint16_t
transform_int_linear(uint16_t input, uint16_t in_min,
                    uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
    /* Prevent divide-by-zero */
    if (in_min == in_max) {
        return 0;
    }

    /* Point-slope form: y = (rise(x - x1)/run) + y1 */
    return (uint16_t)((((uint32_t)(out_max - out_min) * (input - in_min)) / (in_max - in_min)) + out_min);
}

/*
 * This function searches a lookup table that consists of inflection points that
 * draw a curve of connected linear paths. This is sometimes the best way to
 * represent highly non-linear output data. For example, a basic NTC thermistor
 * resistance changes non-linearly with respect to temperature. You might think that
 * it would be better to convert the data so that your LUT gives a fixed ADC
 * step along the x-axis, and maybe you'd be right, but you need to carefully
 * analyze your resulting data to see if you're risking throwing away precision
 * somewhere when the resistance is changing very rapidly.
 *
 * The LUT is typically constructed with a fixed step of the output unit
 * corresponding to a non-linear change in the input unit. The index of each
 * LUT member is proportional to the step size.
 *
 * Pass a pointer to the LUT, a count of how many elements are in the LUT, and
 * the input value.
 *
 * The returned pointer points to the minimum output value, and pointer+1 points
 * to the maximum output value. These two points define the linear range that
 * contains the input value.
 *
 * TODO: Consider switching to binary search of the LUT.
 */
uint16_t *
transform_interpolate(uint16_t *lut, uint16_t nmemb, uint16_t input)
{
    if (lut == NULL || nmemb < 2) {
        return NULL;
    }

    if (input <= lut[0]) {
        return lut;
    }

    for (uint16_t i=0; i<nmemb-1; i++) {
        if (input >= lut[i] && input < lut[i+1]) {
            return lut+i;
        }
    }

    return lut + nmemb - 2;
}
