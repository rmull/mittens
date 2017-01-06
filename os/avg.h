#ifndef AVG_H_
#define AVG_H_

struct avg_descriptor {
    uint32_t ma_total;
    uint16_t nsamps;
    uint16_t init;
};

void avg_init(struct avg_descriptor *, uint16_t);
void avg_moving(struct avg_descriptor *, uint16_t);
uint16_t avg_get(struct avg_descriptor *);

#endif
