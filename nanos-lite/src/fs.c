#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);
typedef struct {
  char *name;
  size_t size;
  off_t disk_offset;
  off_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;



size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
  [FD_FB] = {"/dev/fb",0,0,0,NULL,fb_write},
  [FD_DISPINFO] = {"/proc/dispinfo",128,0,0,dispinfo_read},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
  file_table[FD_FB].size = screen_width() * screen_height() * 4;
}

inline off_t fs_disk_offset(int fd) {
  assert(fd < NR_FILES);
  return file_table[fd].disk_offset;
}

inline size_t fs_filesz(int fd) {
  assert(fd < NR_FILES);
  return file_table[fd].size;
}

int fs_open(const char *pathname, int flags, int mode) {
  for(int i = 0;i < NR_FILES;++i) {
    if(!strcmp(pathname,file_table[i].name)) {
      file_table[i].open_offset = 0;
      return i;
    }
  }
  assert(0);
  return 0;
}
ssize_t fs_write(int fd, const void *buf, size_t len) {
  switch(fd) {
    case FD_STDIN: {
      break;
    }
    case FD_STDOUT:
    case FD_STDERR: {
      file_table[fd].write(buf,0,len);
      break;
    }
    case FD_FB: {
      if(file_table[fd].open_offset >= fs_filesz(fd)) {
        return 0;
      }
      if(file_table[fd].open_offset + len > fs_filesz(fd)) {
        len = file_table[fd].size - file_table[fd].open_offset;
      }
      file_table[fd].write(buf,file_table[fd].open_offset,len);
      file_table[fd].open_offset += len;
      break;
    }
    case FD_DISPINFO: {
      break;
    }
    default: {
      if(file_table[fd].open_offset + len >= fs_filesz(fd)) {
        len = fs_filesz(fd) - file_table[fd].open_offset - 1;
      }
      size_t offset = file_table[fd].disk_offset + file_table[fd].open_offset;
      int i = 0;
      i = ramdisk_write(buf,offset,len);
      file_table[fd].open_offset += i;
      return i;
    }
  }
  return len;
}
ssize_t fs_read(int fd, void *buf, size_t len) {
  switch(fd) {
    case FD_STDIN:
    case FD_STDOUT:
    case FD_STDERR:
    case FD_FB: {
      break;
    }
    case FD_DISPINFO: {
      if(file_table[fd].open_offset >= fs_filesz(fd)) {
        return 0;
      }
      if(file_table[fd].open_offset + len > fs_filesz(fd)) {
        len = file_table[fd].size - file_table[fd].open_offset;
      }
      file_table[fd].read(buf, file_table[fd].open_offset, len);
      file_table[fd].open_offset += len;	
      break;
    }
    default: {
      if(file_table[fd].open_offset + len >= fs_filesz(fd)) {
        len = fs_filesz(fd) - file_table[fd].open_offset - 1;
      }
      size_t offset = file_table[fd].disk_offset + file_table[fd].open_offset;
      int i = 0;
      i = ramdisk_read(buf,offset,len);
      file_table[fd].open_offset += i;
      return i;
    }
  }
  return len;
}
off_t fs_lseek(int fd, off_t offset, int whence) {
  switch(whence) {
    case SEEK_SET: {
      file_table[fd].open_offset = offset;
      break;
    }
    case SEEK_CUR: {
      file_table[fd].open_offset += offset;
      break;
    }
    case SEEK_END: {
      file_table[fd].open_offset = file_table[fd].disk_offset + fs_filesz(fd) - offset;
      break;
    }
  }
  return file_table[fd].open_offset;
}
int fs_close(int fd) {
  return 0;
}