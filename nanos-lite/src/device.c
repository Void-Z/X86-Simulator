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
  int key = read_key();
  bool down = false;
  if(key & 0x8000) {
    key ^= 0x8000;
    down = true;
  }
  if(key == _KEY_NONE) {
    uint32_t t = uptime();
    sprintf(buf,"t %u\n",t);
  } else {
    sprintf(buf,"%s %s\n",down ? "kd" : "ku",keyname[key]);
    // printf("%s\n",buf);
  }
  return strlen(buf);
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  strncpy(buf, dispinfo + offset, len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  draw_rect((uint32_t *)buf,(offset / 4) % screen_width(),(offset / 4) / screen_width(),len / 4,1);
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", screen_width(), screen_height());
}
