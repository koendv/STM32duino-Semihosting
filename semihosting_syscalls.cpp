#include <semihosting_syscalls.h>

/* 
 * semihosting syscalls
 * 2020 Koen De Vleeschauwer, after Liviu Ionescu and Max Asulov
 */

namespace semihosting
{

  /*
   * semihosting system call
   *
   * Depending upon architecture (32 or 64 bit) and code generated (arm or thumb),
   * different instructions are used to do a semihosting call.
   * Here we use the thumb mode semihosting call, as stm32duino compiles using "arm-none-eabi-g++ -mthumb".
   * See "Semihosting for AArch32 and AArch64, Table 3.1: Semihosting Trap Instructions and Encodings".
   */

  static inline int semihosting_call (int R0, int R1) {
    int rc;
    __asm__ volatile (
      "mov r0, %1\n"             /* move int R0 to register r0 */
      "mov r1, %2\n"             /* move int R1 to register r1 */
      "bkpt #0xAB\n"             /* thumb mode semihosting call */
      "mov %0, r0"               /* more register r0 to int rc */
      : "=r" (rc)
      : "r" (R0), "r" (R1)
      : "r0", "r1", "ip", "lr", "memory", "cc"
      );
    return rc;
  }

  int sclock() {
    return semihosting_call(SYS_CLOCK, 0);
  }

  int sclose(int handle) {
    return semihosting_call(SYS_CLOSE, (int) &handle);
  }

  int selapsed(unsigned long long* ticks) {
    return semihosting_call(SYS_ELAPSED, (int) ticks);
  }

  int serrno() {
    return semihosting_call(SYS_ERRNO, 0);
  }

  void sexit(int reason) {
    semihosting_call(SYS_EXIT, reason);
    for(;;);
    return;
  }

  void sexitextended(unsigned long long* reason) {
    semihosting_call(SYS_EXIT_EXTENDED, (int) reason);
    return;
  }

  int sflen(int handle) {
    return semihosting_call(SYS_FLEN, (int) &handle);
  }

  /* command line arguments, separated by spaces.
   * Black Magic Probe uses backslashes to escape spaces '\ '
   * and backslashes '\\' in the command line arguments themselves.
   */

  int sgetcmdline(void *buf, unsigned int size) {
    unsigned int args[] = {
      (unsigned int) buf,
      (unsigned int) size,
    };
    return semihosting_call(SYS_GET_CMDLINE, (int) args);
  }

  int sheapinfo(heapinfo_block_t *block) {
    return semihosting_call(SYS_HEAPINFO, (int) block);
  }

  int siserror(int status) {
    return semihosting_call(SYS_ISERROR, (int) &status);
  }

  int sistty(int handle) {
    return semihosting_call(SYS_ISTTY, (int) &handle);
  }

  int sopen(const char *path, open_mode_t mode, unsigned int path_len) {
    unsigned int args[] = {
      (unsigned int) path,
      (unsigned int) mode,
      (unsigned int) path_len,
    };
    return semihosting_call(SYS_OPEN, (int) args);
  }

  int sread(int handle, void *buf, unsigned int count) {
    unsigned int args[] = {
      (unsigned int) handle,
      (unsigned int) buf,
      (unsigned int) count,
    };
    return semihosting_call(SYS_READ, (int) args);
  }

  int sreadc() {
    return semihosting_call(SYS_READC, 0);
  }

  int sremove(char *path, unsigned int path_len) {
    unsigned int args[] = {
      (unsigned int) path,
      (unsigned int) path_len,
    };
    return semihosting_call(SYS_REMOVE, (int) args);
  }

  int srename(char *old_path, unsigned int old_path_len, char *new_path, unsigned int new_path_len) {
    unsigned int args[] = {
      (unsigned int) old_path,
      (unsigned int) old_path_len,
      (unsigned int) new_path,
      (unsigned int) new_path_len,
    };
    return semihosting_call(SYS_RENAME, (int) args);
  }

  int sseek(int handle, int pos) {
    unsigned int args[] = {
      (unsigned int) handle,
      (unsigned int) pos,
    };
    return semihosting_call(SYS_SEEK, (int) args);
  }

  int ssystem(const char* command, unsigned int length) {
    unsigned int args[] = {
      (unsigned int) command,
      (unsigned int) length,
    };
    return semihosting_call(SYS_SYSTEM, (int) args);
  }

  int stickfreq() {
    return semihosting_call(SYS_TICKFREQ, 0);
  }

  unsigned int stime() {
    return (unsigned int)semihosting_call(SYS_TIME, 0);
  }

  int stmpnam(void *buf, int target_id, unsigned int buf_size) {
    unsigned int args[] = {
      (unsigned int) buf,
      (unsigned int) target_id,
      (unsigned int) buf_size,
    };
    return semihosting_call(SYS_TMPNAM, (int) args);
  }

  int swrite(int handle, void *buf, unsigned int count) {
    unsigned int args[] = {
      (unsigned int) handle,
      (unsigned int) buf,
      (unsigned int) count,
    };
    return (count - semihosting_call(SYS_WRITE, (int) args));
  }

  void swritec(char symbol) {
    semihosting_call(SYS_WRITEC, (int) &symbol);
  }

  void swrite0(char *string) {
    semihosting_call(SYS_WRITE0, (int) string);
  }

}

/* not truncated */
