#include <stdlib.h>

#include "serial.h"

/*
 * This is an implementation of serial communication buffers usable by SPI,
 * UART, I2C, etc.
 */

int serial_lock(struct serial_descriptor *sb);
void serial_unlock(struct serial_descriptor *sb);

void
serial_init(struct serial_descriptor *sb)
{
    sb->cb = NULL;
    sb->buf = NULL;
    sb->sz = 0;
    sb->ctx = NULL;
    sb->flags = 0;
}

int
serial_lock(struct serial_descriptor *sb)
{
    if (sb->flags & FLAG_LOCK) {
        return SERIAL_BUSY;
    }

    sb->flags |= FLAG_LOCK;
    return SERIAL_OK;
}

void
serial_unlock(struct serial_descriptor *sb)
{
    sb->flags &= ~FLAG_LOCK;
}

void
serial_set_cb(struct serial_descriptor *sb, void (*cb)(void *ctx), void *ctx)
{
    while (serial_lock(sb) == SERIAL_BUSY);

    sb->ctx = ctx;
    sb->cb = cb;

    serial_unlock(sb);
}

int
serial_set_buf(struct serial_descriptor *sb, uint8_t *buf, uint16_t sz)
{
    int status;

    while (serial_lock(sb) == SERIAL_BUSY);

    if (!(sb->flags & FLAG_INUSE)) {
        sb->flags |= FLAG_INUSE;

        sb->buf = buf;
        sb->sz = sz;

        status = SERIAL_OK;

    } else {

        /* Serial buffer is in use, caller should cancel it first */
        status = SERIAL_BUSY;
    }

    serial_unlock(sb);

    return status;
}

/*
 * When reading/removing bytes from a serial buffer, use this. It will execute a
 * callback if it is called after the buffer has been entirely drained.
 *
 * NOTE: No locking performed.
 *
 * Returns: Pointer to a new byte, or NULL if there are no bytes remaining.
 */
uint8_t *
serial_pop(struct serial_descriptor *sb)
{
    uint8_t *pop;

    if (sb->sz > 0) {
        pop = sb->buf++;
        sb->sz--;

        if (sb->sz == 0) {
            sb->flags &= ~SERIAL_BUSY;

            if (sb->cb != NULL) {
                sb->cb(sb->ctx);
            }
        }
    } else {
        pop = NULL;
    }

    return pop;
}

/*
 * When adding bytes to a serial buffer, use this.
 *
 * NOTE: No locking performed.
 *
 * Returns: Pointer to the next insertion address, or NULL if there is no room
 * left in the serial buffer.
 */
uint8_t *
serial_push(struct serial_descriptor *sb, uint8_t push)
{
    if (sb->sz > 0) {
        *(sb->buf) = push;
        sb->buf++;
        sb->sz--;

        if (sb->sz == 0) {
            sb->flags &= ~SERIAL_BUSY;

            if (sb->cb != NULL) {
                sb->cb(sb->ctx);
            }

            return NULL;
        }
    }

    return sb->buf;
}

uint16_t
serial_get_sz(struct serial_descriptor *sb)
{
    return sb->sz;
}
