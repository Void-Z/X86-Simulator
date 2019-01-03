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

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

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
  {"stdout", 0, 0, 0, invalid_read, invalid_write},
  {"stderr", 0, 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
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
  char *p = (char *)file_table[fd].disk_offset;
  int i = 0;
  for(;(i < len) && (file_table[fd].open_offset < fs_filesz(fd));++i) {
    *(p + file_table[fd].open_offset++) = *((char *)buf + i);
  }
  return i;
}
ssize_t fs_read(int fd, void *buf, size_t len) {
  printf("read %d\n",len);
  char *p = (char *)file_table[fd].disk_offset;
  int i = 0;
  for(;(i < len) && (file_table[fd].open_offset < fs_filesz(fd));++i) {
    *((char *)buf + i) = *(p + file_table[fd].open_offset++);
    printf("%x\n",*((char*)buf + i - 1));
  }
  return i;
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