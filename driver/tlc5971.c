#include <stdint.h>
#include <string.h>

#include "tlc5971.h"

void tlc5971_spi_cb(void *ctx);
uint16_t htons(uint16_t x);
uint16_t ntohs(uint16_t x);

/* Options for control:
 * 1. TLCs are all controlled identically and BC is hardcoded
 *  - tlc_hdr is const. BC is hardcoded in its declaration.
 * 2. TLCs are all controlled identically and BC may vary at runtime
 *  - tlc_hdr is not const. BC (x3) needs to be added to the descriptor struct.
 * 3. TLCs are all uniquely controlled - different BC per TLC
 *  - Entire tlc_hdr needs to be moved into the descriptor struct.
 */

/* Default TLC5971 header: Internal oscillator, display repeat, max global brightness (BC) */
#define TLC_HDR_DEFAULT														\
	((TLC_WR_CMD << TLC_WR_OFF) 		| /* Write command */ 				\
	(TLC_CTRL_DSPRPT << TLC_CTRL_OFF)	| /* Enable display repeat mode */	\
	(TLC_BC_MAX << TLC_BC_B_OFF) 		| /* Blue BC at max */ 				\
	(TLC_BC_MAX << TLC_BC_G_OFF) 		| /* Green BC at max */ 			\
	(TLC_BC_MAX << TLC_BC_R_OFF))	  	  /* Red BC at max */

/* A buffer for transmitting the header. Decide if const is correct for your application. */
const uint8_t tlc_hdr[4] = {
        (TLC_HDR_DEFAULT >> 24),
        (TLC_HDR_DEFAULT >> 16) & 0xFF,
        (TLC_HDR_DEFAULT >> 8) & 0xFF,
        TLC_HDR_DEFAULT & 0xFF
};

void
tlc5971_spi_cb(void *ctx)
{
    struct tlc5971_descriptor *tlc = (struct tlc5971_descriptor *)ctx;

    if (tlc == NULL) {
        return;
    }

    switch (tlc->state) {
    case TLC_STATE_HEADER:
        tlc->state = TLC_STATE_BGR;
        spi_write(tlc->spi, (uint8_t *)(tlc->bgr), 2*TLC_NCHANS, tlc5971_spi_cb, (void *)tlc);
        break;

    case TLC_STATE_BGR:
        tlc->state = TLC_STATE_WAITLATCH;
        break;

    default:
        break;
    }
}

void
tlc5971_init(struct tlc5971_descriptor *tlc, struct spi_descriptor *spi, uint8_t ntlc)
{
    if (tlc == NULL || spi == NULL) {
        return;
    }

    memset(tlc, 0x00, sizeof(struct tlc5971_descriptor));

    tlc->spi = spi;
    tlc->hdr = (uint8_t *)&tlc_hdr[0];
    tlc->state = TLC_STATE_HEADER;
    tlc->ntlc = ntlc;

    /* TODO: Improve this */
    tlc->spi->tx = &(tlc->tx);
}

void
tlc5971_set_global_brightness(struct tlc5971_descriptor *tlc, enum tlc5971_group group, uint8_t bc)
{
    if (tlc == NULL || bc > TLC_BC_MAX || group >= TLC_GROUP_TOTAL) {
        return;
    }
}

/* These byteswap functions can maybe be done more efficiently with asm: rev16 */
uint16_t ntohs(uint16_t x)
{
    return ((x & 0xFF) << 8) | ((x >> 8) & 0xFF);
}
uint16_t htons(uint16_t x)
{
    return ((x & 0xFF) << 8) | ((x >> 8) & 0xFF);
}

void
tlc5971_set_bgr(struct tlc5971_descriptor *tlc, uint16_t *bgr)
{
    if (tlc == NULL || bgr == NULL) {
        return;
    }

    /* Attach color data to the TLC5971 descriptor */
    tlc->bgr = bgr;

    tlc->state = TLC_STATE_HEADER;

    /* Convert from host to network byte order in place */
    for (uint8_t i=0; i<12; i++) {
        tlc->bgr[i] = htons(tlc->bgr[i]);
    }

    spi_write(tlc->spi, tlc->hdr, TLC_HEADER_SZ, tlc5971_spi_cb, (void *)tlc);

    /* Convert to from network to host byte order in place */
    //for (uint8_t i=0; i<12; i++) {
    //    tlc->bgr[i] = ntohs(tlc->bgr[i]);
    //}
}

void
tlc5971_poll(struct tlc5971_descriptor *tlc)
{
    if (tlc == NULL) {
            return;
    }

    switch (tlc->state) {
    case TLC_STATE_WAITLATCH:
        tlc->state = TLC_STATE_IDLE;
        break;

    default:
        break;
    }
}
