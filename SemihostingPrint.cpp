#include <semihosting_syscalls.h>
#include <SemihostingPrint.h>

/* 
 * Arduino class to write to semihosting debug output (stderr, unbuffered)
 * 2020 Koen De Vleeschauwer
 */

SemihostingPrint::SemihostingPrint() {
  char *stdio_fname=":tt";
  stderr_handle = semihosting::sopen(stdio_fname, semihosting::OPEN_MODE_A, strlen(stdio_fname));
};

SemihostingPrint::~SemihostingPrint() {
  semihosting::sclose(stderr_handle);
}

/* print on debug console */

size_t SemihostingPrint::write(uint8_t ch) {
  return 1 - semihosting::swrite(stderr_handle, (void *)&ch, 1);
}

size_t SemihostingPrint::write(const uint8_t *buffer, size_t size) {
  return size - semihosting::swrite(stderr_handle, (void *)buffer, size);
}

void SemihostingPrint::flush() {
  // not buffering writes, so nothing to flush.
};

/* not truncated */
