#ifndef SEMIHOSTING_BMP_H
#define SEMIHOSTING_BMP_H

/* error numbers are system and debugger dependent. */

/* gdb filei/o error numbers. used by black magic probe */

typedef enum {
  STDIN_HANDLE = 1,
  STDOUT_HANDLE = 2,
  STDERR_HANDLE = 3,
} stdio_handle_t;

typedef enum {
  TARGET_EPERM = 1,
  TARGET_ENOENT = 2,
  TARGET_EINTR = 4,
  TARGET_EIO = 5,
  TARGET_EBADF = 9,
  TARGET_EACCES = 13,
  TARGET_EFAULT = 14,
  TARGET_EBUSY = 16,
  TARGET_EEXIST = 17,
  TARGET_ENODEV = 19,
  TARGET_ENOTDIR = 20,
  TARGET_EISDIR = 21,
  TARGET_EINVAL = 22,
  TARGET_ENFILE = 23,
  TARGET_EMFILE = 24,
  TARGET_EFBIG = 27,
  TARGET_ENOSPC = 28,
  TARGET_ESPIPE = 29,
  TARGET_EROFS = 30,
  TARGET_ENOSYS = 88,
  TARGET_ENAMETOOLONG = 91,
  TARGET_EUNKNOWN = 9999,
} target_errno_t;

#endif
