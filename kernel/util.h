/* rewrites of needed cstdlib functions, as well as misc functions */

#ifndef UTIL_H
#define UTIL_H

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

void memset(void *dest, uint8_t val, size_t len);
void memcpy(void *dest, void *src, size_t len);
void halt();

#endif