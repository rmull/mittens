#ifndef TLC5971_SHOW_H
#define	TLC5971_SHOW_H

void tlc5971_show(uint16_t *dest_buf, uint8_t npx);
void tlc5971_show_init(void);
void tlc5971_testpattern(uint16_t *dest_buf, uint8_t npx);

#endif

