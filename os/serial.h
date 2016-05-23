#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdint.h>

struct serial_descriptor {
    void (*cb)(void *ctx);
    void *ctx;
    uint8_t *buf;
    uint16_t sz;
    uint8_t flags;
    uint8_t pad;
};

/* serial_buf flags */
#define FLAG_LOCK       (1<<0)
#define FLAG_INUSE      (1<<1)

/* Return values */
#define SERIAL_OK       0
#define SERIAL_BUSY     -1

void serial_init(struct serial_descriptor *sb);
void serial_set_cb(struct serial_descriptor *sb, void (*cb)(void *ctx), void *ctx);
int serial_set_buf(struct serial_descriptor *sb, uint8_t *buf, uint16_t sz);
uint8_t * serial_pop(struct serial_descriptor *sb);
uint8_t * serial_push(struct serial_descriptor *sb, uint8_t push);
uint16_t serial_get_sz(struct serial_descriptor *sb);

#endif
