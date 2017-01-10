#include <stdint.h>
#include <stdlib.h>

#include "driver/tlc5971.h"

uint16_t hue_state[TLC_NPIXELS];
uint16_t white[TLC_NPIXELS];
uint8_t white_up[TLC_NPIXELS];
struct tlc5971_color tlcc[TLC_NPIXELS];

void hue2rgb16(struct tlc5971_color *color, uint16_t hue)
{
	switch (hue >> 8) {
		
	/* We are between R and RG */
	case 0:
		color->r = 0xFFFF;						/* Red */
		color->g = (hue & 0xFF) << 8;				/* Green */
		color->b = 0x0000;						/* Blue */
		break;
	
	/* We are between RG and G */
	case 1:
		color->r = ~(hue & 0xFF) << 8;
		color->g = 0xFFFF;
		color->b = 0x0000;
		break;
	
	/* We are between G and GB */
	case 2:
		color->r = 0x0000;
		color->g = 0xFFFF;
		color->b = (hue & 0xFF) << 8;
		break;
		
	/* We are between GB and B */
	case 3:
		color->r = 0x0000;
		color->g = ~(hue & 0xFF) << 8;
		color->b = 0xFFFF;
		break;
		
	/* We are between B and BR */
	case 4:
		color->r = (hue & 0xFF) << 8;
		color->g = 0x0000;
		color->b = 0xFFFF;
		break;
		
	/* We are between BR and R */
	case 5:
		color->r = 0xFFFF;
		color->g = 0x0000;
		color->b = ~(hue & 0xFF) << 8;
		break;
		
	/* Shouldn't get here */
	default:
		break;
	}
}

#if 0
uint8_t toggle = 0;
int
show_flicker(void)
{
    uint8_t i;

    for (i=0; i<TLC_NPIXELS; i++) {
        if (toggle) {
            tlc.color[i].r = 0xFFFF;
            tlc.color[i].g = 0xFFFF;
            tlc.color[i].b = 0xFFFF;
            tlc.color[i].w = 0xFFFF;
        } else {
            tlc.color[i].r = 0;
            tlc.color[i].g = 0;
            tlc.color[i].b = 0;
            tlc.color[i].w = 0;
        }

        toggle ^= 1;
    }

    return 1;
}
#endif

#define MASK 0x80000057
uint32_t random_number = 1;
uint16_t
random(void)
{
    uint8_t i;

    for (i=0; i<4; i++) {
        if (random_number & 0x00000001) {
            random_number = (((random_number ^ MASK) >> 1) | 0x80000000);
        } else {
            random_number >>=1;
        }
    }

    return((uint16_t)random_number);
}

void
tlc5971_show_noise(uint8_t npx)
{
    unsigned int randval;

    rand_r(&randval);

    randval %= 0x7FFF;

    for (uint8_t i=0; i<npx; i++) {
            tlcc[i].r = randval;
            tlcc[i].g = randval;
            tlcc[i].b = randval;
    }
}

void
tlc5971_color_to_buf(struct tlc5971_color *color, uint16_t *buf)
{
	buf[0] = color->b;
	buf[1] = color->g;
	buf[2] = color->r;
}

void
tlc5971_show_wash(uint8_t npx)
{
	for (uint8_t i=0; i<npx; i++) {
		hue_state[i]+=4;
		if (hue_state[i] > 0x5FF) {
			hue_state[i] = 0;
		}

		hue2rgb16(&(tlcc[i]), hue_state[i]);
	}
}

#define THROBSTEP 200
uint16_t throb = 0xFFFF;
#define THROB_LOCK      0x1FF
uint8_t up = 0;
void
tlc5971_show_throb(uint8_t npx)
{
    if (up == 0) {
        if (throb > THROBSTEP) {
                throb -= THROBSTEP;
        } else {
                throb = 0;
                up = 1;
        }
    } else if (up == 1) {
        if (throb < (0xFFFF-THROBSTEP)) {
                throb += THROBSTEP;
        } else {
                throb = 0xFFFF;
                up = 0;
        }
    } else {
        throb = THROB_LOCK;
    }

    for (uint8_t i=0; i<npx; i++) {
            tlcc[i].r = throb;
            tlcc[i].g = throb;
            tlcc[i].b = throb;
    }
}

#if 0
void
tlc5957_testpattern_allon(void)
{
    uint8_t i;

    for (i=0; i<TLC_NPIXELS; i++) {
        tlc.color[i].r = 0xFFFF;
        tlc.color[i].g = 0xFFFF;
        tlc.color[i].b = 0xFFFF;
        tlc.color[i].w = 0xFFFF;
        hue[i] = i*128;
        white[i] = 0;
    }
}

void
tlc5957_testpattern_onechannel(uint8_t ch)
{
    uint8_t i;

    for (i=0; i<TLC_NPIXELS; i++) {
        tlc.color[i].r = 0x0000;
        tlc.color[i].g = 0x0000;
        tlc.color[i].b = 0x0000;
        tlc.color[i].w = 0x0000;

        switch (ch) {
        case 0:
            tlc.color[i].r = 0xFFFF;
            break;
        case 1:
            tlc.color[i].g = 0xFFFF;
            break;
        case 2:
            tlc.color[i].b = 0xFFFF;
            break;
        case 3:
            tlc.color[i].w = 0xFFFF;
            break;

        }

        hue[i] = i*128;
        white[i] = 0;
    }
}
#endif

void
tlc5971_show_init(void)
{
	for (uint8_t i=0; i<TLC_NPIXELS; i++) {
		hue_state[i] = i*0x40;
		tlcc[i].r = 0xFFFF;
		tlcc[i].g = 0xFFFF;
		tlcc[i].b = 0xFFFF;
	}
}

void
tlc5971_show(uint16_t *dest_buf, uint8_t npx)
{
    //tlc5971_show_wash(npx);
    //tlc5971_show_throb(npx);
    tlc5971_show_noise(npx);

    for (uint8_t i=0; i<npx; i++) {
        tlc5971_color_to_buf(&tlcc[i], &dest_buf[i*3]);
    }
}

void
tlc5971_testpattern_whitelevel(struct tlc5971_color *color, uint16_t level)
{
	color->r = level;
	color->g = level;
	color->b = level;
}

void
tlc5971_testpattern_allhalfon(struct tlc5971_color *color, uint8_t npx)
{
    for (uint8_t i=0; i<npx; i++) {
		tlc5971_testpattern_whitelevel(&color[i], 0x7FFF);
    }
}

void
tlc5971_testpattern_allfullon(struct tlc5971_color *color, uint8_t npx)
{
    for (uint8_t i=0; i<npx; i++) {
        tlc5971_testpattern_whitelevel(&color[i], 0xFFFF);
    }
}

void
tlc5971_testpattern(uint16_t *dest_buf, uint8_t npx)
{
    tlc5971_testpattern_allfullon(&tlcc[0], npx);
    for (uint8_t i=0; i<npx; i++) {
        tlc5971_color_to_buf(tlcc+i, &dest_buf[i*3]);
    }
}
