#include <stdint.h>

#include "avg.h"

/*
 * Use this file if you want a reusable way to keep an average of some periodic
 * samples
 */

void
avg_init(struct avg_descriptor *avg, uint16_t nsamps)
{
    avg->ma_total = 0;
    avg->nsamps = nsamps;
    avg->init = 0;
}

void
avg_moving(struct avg_descriptor *avg, uint16_t val)
{
    if (avg->init < avg->nsamps) {
        avg->init++;
        avg->ma_total += val;
    } else {
        avg->ma_total = avg->ma_total + val - (avg->ma_total / avg->nsamps);
    }
}

uint16_t
avg_get(struct avg_descriptor *avg)
{
    if (avg->init < avg->nsamps) {
        if (avg->init != 0) {
            return avg->ma_total / avg->init;
        } else {
            return 0;
        }
    } else {
        return avg->ma_total / avg->nsamps;
    }
}
