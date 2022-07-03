/* function definitions */
#include "type.h"

//sets memory to a value
void memset(void *dest, uint8_t val, size_t len) {
    uint8_t *temp = (uint8_t *)dest;
    while (len--) {
        *temp++ = val;
    }
}

//copies memory from src to dest
void memcpy(void *dest, void *src, size_t len) {
    uint8_t *temp1 = (uint8_t *)dest;
    uint8_t *temp2 = (uint8_t *)src;
    while (len--) {
        *temp1++ = *temp2++;
    }
}

//halts the processor
void halt() {
    asm("hlt");
}