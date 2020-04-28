#include <semihosting_syscalls.h>
#include <Semihosting.h>

/* 
 * Arduino class to write to and read from semihosting console 
 * requires the following semihosting calls: SYS_OPEN, SYS_READC, SYS_WRITE, SYS_CLOSE.
 * 2020 Koen De Vleeschauwer
 */

SemihostingStream::SemihostingStream() {
  char *stdio_fname=":tt";
  stdin_handle = semihosting::sopen(stdio_fname, semihosting::OPEN_MODE_R, strlen(stdio_fname));
  stdout_handle = semihosting::sopen(stdio_fname, semihosting::OPEN_MODE_W, strlen(stdio_fname));
  peekbuf = '?';
  peekbuf_valid = false;
};

SemihostingStream::~SemihostingStream() {
  semihosting::sclose(stdin_handle);
  semihosting::sclose(stdout_handle);
}

/* print on gdb console */

size_t SemihostingStream::write(uint8_t ch) {
  return 1 - semihosting::swrite(stdout_handle, (void *)&ch, 1);
}

size_t SemihostingStream::write(const uint8_t *buffer, size_t size) {
  return size - semihosting::swrite(stdout_handle, (void *)buffer, size);
}

void SemihostingStream::flush() {
  // not buffering writes, so nothing to flush.
};

/* read from gdb console */

int SemihostingStream::read() {
  if (peekbuf_valid) {
    peekbuf_valid = false;
    return peekbuf;
  }
  return semihosting::sreadc();
}

size_t SemihostingStream::readBytes(char *buf, size_t length) {
  int i = 0;
  while(i < length) {
    int rc = read();
    if (rc < 0) break;
    buf[i++] = rc;
    if (rc == '\n') break;
  }
  return i;
}

int SemihostingStream::peek() {
  if (!peekbuf_valid) {
    int rc = semihosting::sreadc();
    peekbuf_valid = rc >= 0;
    if (peekbuf_valid) peekbuf = rc;
    else peekbuf = '?';
  }
  if (peekbuf_valid) return peekbuf;
  else return -1;
}

int SemihostingStream::available() {
  peek();
  if (peekbuf_valid) return 1;
  else return 0;
}

/* not truncated */
