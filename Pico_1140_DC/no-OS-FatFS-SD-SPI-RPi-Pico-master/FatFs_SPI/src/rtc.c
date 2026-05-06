/* rtc.c - stub para RP2350: sin hardware RTC.
   Devuelve timestamp fijo para FatFs. */
#include <time.h>
#include "ff.h"

static time_t epochtime = 0;

time_t time(time_t *pxTime) {
    if (pxTime) *pxTime = epochtime;
    return epochtime;
}

void time_init() { /* no-op en RP2350 */ }

DWORD get_fattime(void) {
    /* Fecha fija: 2024-01-01 00:00:00
       bits 31:25 = año-1980, 24:21 = mes, 20:16 = día */
    return ((DWORD)(2024 - 1980) << 25)
         | ((DWORD)(1) << 21)
         | ((DWORD)(1) << 16);
}
