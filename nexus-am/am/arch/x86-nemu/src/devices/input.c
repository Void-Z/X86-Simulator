#include <am.h>
#include <x86.h>
#include <amdev.h>

size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _KbdReg *kbd = (_KbdReg *)buf;
      // kbd->keydown = 0;
      // kbd->keycode = _KEY_NONE;
      kbd->keycode = inl(0x60);
      if((kbd->keycode & 0x8000)) {
        kbd->keydown = 1;
      } else {
        kbd->keydown = 0;
      }
      return sizeof(_KbdReg);
    }
  }
  return 0;
}
