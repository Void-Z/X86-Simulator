#include <am.h>
#include <x86.h>
#include <amdev.h>
int printf(const char *fmt, ...);
size_t timer_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_TIMER_UPTIME: {
      // _UptimeReg *uptime = (_UptimeReg *)buf;
      // uint32_t a = inl(0x48);
      // _putc((char)a);
      // uptime->hi = inl(0x48);
      // uptime->lo = inl(0x49);
      return inl(0x48);
    }
    case _DEVREG_TIMER_DATE: {
      _RTCReg *rtc = (_RTCReg *)buf;
      rtc->second = 0;
      rtc->minute = 0;
      rtc->hour   = 0;
      rtc->day    = 0;
      rtc->month  = 0;
      rtc->year   = 2018;
      return sizeof(_RTCReg);
    }
  }
  return 0;
}

void timer_init() {
}
