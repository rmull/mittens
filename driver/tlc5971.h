#ifndef TLC5971_H_
#define TLC5971_H_

#include "os/spi.h"

/*
 * Driver for TI TL5971 signage RGB LED current-sinking driver IC.
 * 12 channel 16-bit enhanced-spectrum PWM.
 * Three color groups with adjustable global brightness control.
 */

/* TODO: Support daisy chain better by having a descriptor for each device */

/* Datasheet SBVS146D (dec 2015) (downloaded 2016-11-04): http://www.ti.com/lit/ds/symlink/tlc5971.pdf */

/* Three color groups with 12 channels (4 channels per group) */
enum tlc5971_group {
	TLC_GROUP_BLUE,
	TLC_GROUP_GREEN,
	TLC_GROUP_RED,
	TLC_GROUP_TOTAL
};
#define TLC_NCHANS              12
#define TLC_NPIXELS             (TLC_NCHANS/TLC_GROUP_TOTAL)

/* Header definition: Sizes and offsets given in bits */
/* Write command */
#define TLC_WR_SZ               6
#define TLC_WR_OFF              (32-TLC_WR_SZ)
#define TLC_WR_CMD              0x25

/* Function control */
#define TLC_CTRL_SZ             5
#define TLC_CTRL_OFF            (TLC_WR_OFF-TLC_CTRL_SZ)
#define TLC_CTRL_BLANK          (1<<0)
#define TLC_CTRL_DSPRPT         (1<<1)
#define TLC_CTRL_TMGRST         (1<<2)
#define TLC_CTRL_EXTGCK         (1<<3)
#define TLC_CTRL_OUTTMG         (1<<4)

/* Global brightness (BC) */
#define TLC_BC_SZ               7
#define TLC_BC_OFF              (TLC_CTRL_OFF-TLC_BC_SZ)
#define TLC_BC_B_OFF            (TLC_BC_OFF)
#define TLC_BC_G_OFF            (TLC_BC_B_OFF-TLC_BC_SZ)
#define TLC_BC_R_OFF            (TLC_BC_G_OFF-TLC_BC_SZ)
#define TLC_BC_MAX              0x7F					/* Maximum global brightness current setting */

#define TLC_HEADER_SZ           4 /* bytes */

enum tlc5971_state {
	TLC_STATE_IDLE,
	TLC_STATE_HEADER,
	TLC_STATE_BGR,
	TLC_STATE_WAITLATCH
};

struct tlc5971_color {
    uint16_t b;
    uint16_t g;
    uint16_t r;
};

struct tlc5971_device_descriptor {
    uint32_t header;
    uint16_t *bgr;
};

struct tlc5971_descriptor {
	struct spi_descriptor *spi;
        struct serial_descriptor tx;
	struct tlc5971_device_descriptor *dev;
	uint16_t *bgr;			/* Point to 16-bit grayscale data, organized in B,G,R triplets */
	uint8_t *hdr;			/* TLC5971 packet header (write cmd, ctrl bits, BC bits) */
	uint16_t ntlc;			/* Number of TLC5971 chips in the daisy chain, which will dictate the length of data at *bgr */
	enum tlc5971_state state;
};

void tlc5971_init(struct tlc5971_descriptor *tlc, struct spi_descriptor *spi, uint8_t ntlc);
void tlc5971_poll(struct tlc5971_descriptor *tlc);
void tlc5971_set_global_brightness(struct tlc5971_descriptor *tlc, enum tlc5971_group group, uint8_t bc);
void tlc5971_set_bgr(struct tlc5971_descriptor *tlc, uint16_t *bgr);

#endif
