#include <stdlib.h>

#include "serial.h"

/*
 * This is an implementation of serial communication buffers usable by SPI,
 * UART, I2C, etc.
 */

int serial_lock(struct serial_descriptor *s);
void serial_unlock(struct serial_descriptor *s);

void
serial_init(struct serial_descriptor *s)
{
    s->cb = NULL;
    s->buf = NULL;
    s->sz = 0;
    s->ctx = NULL;
    s->flags = 0;
}

int
serial_lock(struct serial_descriptor *s)
{
    if (s->flags & FLAG_LOCK) {
        return SERIAL_BUSY;
    }

    s->flags |= FLAG_LOCK;
    return SERIAL_OK;
}

void
serial_unlock(struct serial_descriptor *s)
{
    s->flags &= ~FLAG_LOCK;
}

void
serial_set_cb(struct serial_descriptor *s, void (*cb)(void *ctx), void *ctx)
{
    while (serial_lock(s) == SERIAL_BUSY);

    s->ctx = ctx;
    s->cb = cb;

    serial_unlock(s);
}

int
serial_set_buf(struct serial_descriptor *s, uint8_t *buf, uint16_t sz)
{
    int status;

    while (serial_lock(s) == SERIAL_BUSY);

    if (!(s->flags & FLAG_INUSE)) {
        s->flags |= FLAG_INUSE;

        s->buf = buf;
        s->sz = sz;

        status = SERIAL_OK;

    } else {

        /* Serial buffer is in use, caller should cancel it first */
        status = SERIAL_BUSY;
    }

    serial_unlock(s);

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
serial_pop(struct serial_descriptor *s)
{
    uint8_t *pop;

    if (s == NULL) {
        pop = NULL;

    } else {
        if (s->sz > 0) {
            pop = s->buf++;
            s->sz--;

        } else {
            pop = NULL;

            if (s->flags & SERIAL_BUSY) {
                s->flags &= ~SERIAL_BUSY;

                if (s->cb != NULL) {
                    s->cb(s->ctx);
                }
            }
        }
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
serial_push(struct serial_descriptor *s, uint8_t push)
{
    if (s == NULL) {
        return NULL;

    } else {

        if (s->sz > 0) {
            *(s->buf) = push;
            s->buf++;
            s->sz--;

            if (s->sz == 0) {

                s->flags &= ~SERIAL_BUSY;

                if (s->cb != NULL) {
                    s->cb(s->ctx);
                }

                return NULL;
            }

            return s->buf;

        } else {
            return NULL;
        }
    }
}

uint8_t *
serial_peek(struct serial_descriptor *s)
{
    if (s->sz > 0) {
        return s->buf;
    }

    return NULL;
}

uint16_t
serial_get_sz(struct serial_descriptor *s)
{
    if (s == NULL) {
        return 0;
    }
    return s->sz;
}
