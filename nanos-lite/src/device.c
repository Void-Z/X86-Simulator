#include "common.h"
#include <amdev.h>
#include "fs.h"
size_t serial_write(const void *buf, size_t offset, size_t len) {
  for(int i = 0;i < len;++i) {
    _putc(*((char *)buf + i));
  }
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  return 0;
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  int i;
  for(i = 0;i < len && i + offset < fs_filesz(FD_DISPINFO);++i) {
    *((char *)buf + i) = dispinfo[i];
  }
  return i;
}

size_t fb_write(const void *buf, size_t offset, size_t length) {
  // int h,w,len = length;
  // h = offset / screen_width();
  // w = offset % screen_width();
  // if(w + len <= screen_width()) {
  //   draw_rect((uint32_t *)buf,w,h,len,1);
  // } else {
  //   len -= screen_width() - w;
  //   int height = len / screen_width(),width = len % screen_width();
  //   if(height) {
  //     draw_rect((uint32_t *)buf + screen_width() - w,0,h + 1,screen_width(),height);
  //   }
  //   if(width) {
  //     draw_rect((uint32_t *)buf + length - width,0,h + 1 + height,width,1);
  //   }
  // }
  return length;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  fs_read(FD_DISPINFO,dispinfo,fs_filesz(FD_DISPINFO));
}
