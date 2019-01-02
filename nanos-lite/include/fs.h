#ifndef __FS_H__
#define __FS_H__

#include "common.h"

#define ssize_t size_t
#define off_t size_t

#ifndef SEEK_SET
enum {SEEK_SET, SEEK_CUR, SEEK_END};
#endif
off_t fs_disk_offset(int fd);
size_t fs_filesz(int fd);
int fs_open(const char *pathname, int flags, int mode);
ssize_t fs_read(int fd, void *buf, size_t len);
ssize_t fs_write(int fd, const void *buf, size_t len);
off_t fs_lseek(int fd, off_t offset, int whence);
int fs_close(int fd);
#endif
