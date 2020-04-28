#ifndef SEMIHOSTING_SYSCALLS_H
#define SEMIHOSTING_SYSCALLS_H

/* 2020 Koen De Vleeschauwer, after Liviu Ionescu and Max Asulov */

#include <cstdint>
#include <cstddef>

namespace semihosting
{
  enum {
    SYS_CLOCK = 0x10,
    SYS_CLOSE = 0x02,
    SYS_ELAPSED = 0x30,
    SYS_ERRNO = 0x13,
    SYS_EXIT = 0x18,
    SYS_EXIT_EXTENDED = 0x20,
    SYS_FLEN = 0x0C,
    SYS_GET_CMDLINE = 0x15,
    SYS_HEAPINFO = 0x16,
    SYS_ISERROR = 0x08,
    SYS_ISTTY = 0x09,
    SYS_OPEN = 0x01,
    SYS_READ = 0x06,
    SYS_READC = 0x07,
    SYS_REMOVE = 0x0E,
    SYS_RENAME = 0x0F,
    SYS_SEEK = 0x0A,
    SYS_SYSTEM = 0x12,
    SYS_TICKFREQ = 0x31,
    SYS_TIME = 0x11,
    SYS_TMPNAM = 0x0D,
    SYS_WRITE = 0x05,
    SYS_WRITEC = 0x03,
    SYS_WRITE0 = 0x04,
  };

  /* SYS_OPEN open modes */

  typedef enum {
    OPEN_MODE_R   = 0,           /* read */
    OPEN_MODE_RB  = 1,           /* read binary */
    OPEN_MODE_RP  = 2,           /* read plus */
    OPEN_MODE_RPB = 3,           /* read plus binary */
    OPEN_MODE_W   = 4,           /* write */
    OPEN_MODE_WB  = 5,           /* write binary */
    OPEN_MODE_WP  = 6,           /* write plus */
    OPEN_MODE_WPB = 7,           /* write plus binary */
    OPEN_MODE_A   = 8,           /* append */
    OPEN_MODE_AB  = 9,           /* append binary */
    OPEN_MODE_AP  = 10,          /* append plus */
    OPEN_MODE_APB = 11,          /* append plus binary */
  } open_mode_t;

  /* SYS_EXIT reason codes */

  typedef enum {
    /* hardware exceptions: */
    ADP_Stopped_BranchThroughZero = 0x20000,
    ADP_Stopped_UndefinedInstr = 0x20001,
    ADP_Stopped_SoftwareInterrupt = 0x20002,
    ADP_Stopped_PrefetchAbort = 0x20003,
    ADP_Stopped_DataAbort = 0x20004,
    ADP_Stopped_AddressException = 0x20005,
    ADP_Stopped_IRQ = 0x20006,
    ADP_Stopped_FIQ = 0x20007,
    /* software exceptions: */
    ADP_Stopped_BreakPoint = 0x20020,
    ADP_Stopped_WatchPoint = 0x20021,
    ADP_Stopped_StepComplete = 0x20022,
    ADP_Stopped_RunTimeErrorUnknown = 0x20023,
    ADP_Stopped_InternalError = 0x20024,
    ADP_Stopped_UserInterruption = 0x20025,
    ADP_Stopped_ApplicationExit = 0x20026,
    ADP_Stopped_StackOverflow = 0x20027,
    ADP_Stopped_DivisionByZero = 0x20028,
    ADP_Stopped_OSSpecific = 0x20029,
  } exit_reason_t;

  typedef struct heapinfo_block
  {
    int heap_base;
    int heap_limit;
    int stack_base;
    int stack_limit;
  } heapinfo_block_t;

  static inline int semihosting_call (int R0, int R1);
  int sclock();
  int sclose(int handle);
  int selapsed(unsigned long long*);
  int serrno();
  void sexit(int reason);
  void sexitextended(unsigned long long*);
  int sflen(int handle);
  int sgetcmdline(void *buf, unsigned int size);
  int sheapinfo(heapinfo_block_t *block);
  int siserror(int status);
  int sistty(int handle);
  int sopen(const char *path, open_mode_t mode, unsigned int path_len);
  int sread(int handle, void *buf, unsigned int count);
  int sreadc();
  int sremove(char *path, unsigned int path_len);
  int srename(char *old_path, unsigned int old_path_len, char *new_path, unsigned int new_path_len);
  int sseek(int handle, int pos);
  int ssystem(const char* command, unsigned int length);
  int stickfreq();
  unsigned int stime();
  int stmpnam(void *buf, int target_id, unsigned int buf_size);
  int swrite(int handle, void *buf, unsigned int count);
  void swritec(char symbol);
  void swrite0(char *string);

}
#endif

/* not truncated */
