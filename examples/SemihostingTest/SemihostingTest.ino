/*
 * Semihosting testbench - Tests all semihosting calls.
 *
 * This program uses semihosting and requires a debugger probe to run.
 * This program will hang if there is no debugger probe present.
 */

/*
 * Before running, type the following gdb commands:
 * (gdb) set args arg1 arg2 arg3
 * (gdb) set remote system-call-allowed 1
 * BlackMagicProbe only:
 * (gdb) monitor heapinfo 0x1 0x2 0x3 0x4
 * and create the file ":semihosting-features" in the directory where gdb runs:
 * $ echo -e 'SHFB\x03' > ":semihosting-features"
 * $ chmod 0444 ":semihosting-features"
 */

#include <SemihostingStream.h>
#include <semihosting_syscalls.h>

using namespace semihosting;

SemihostingStream sh;
int stdin_handle;
int stdout_handle;

void check_retcode(int rc) {
  if (rc < 0) {sh.print("FAIL return code="); sh.println(rc);}
  else sh.println("OK");
}

void check_getcmdline() {
  // (gdb) set args a b c
  sh.println("SYS_GET_CMDLINE get command line");
  char buf[256];
  sys_getcmdline(buf, sizeof(buf));
  sh.print("command line: ");
  sh.println(buf);
  if (strlen(buf) == 0)
    sh.println("Set command line using 'set args arg1 arg2 arg3' or 'run arg1 arg2 arg3'");
}

void check_clock() {
  sh.println("SYS_CLOCK time in hundredths of seconds");
  for (int i = 0; i < 5; i++) {
    int ret = sys_clock();
    sh.print(ret);
    sh.print(" ");
    check_retcode(ret);
    delay(1000);
  }
}

void check_time() {
  sh.println("SYS_TIME time in seconds");
  for (int i = 0; i < 5; i++) {
    int ret = sys_time();
    sh.print(ret);
    sh.print(" ");
    check_retcode(ret);
    delay(1000);
  }
}

void check_stdio() {
  sh.println("SYS_OPEN special filename ':tt'");
  char *stdio_fname=":tt";
  sh.print("stdin ");
  stdin_handle = sys_open(stdio_fname, semihosting::OPEN_MODE_R, strlen(stdio_fname));
  check_retcode(stdin_handle);
  sh.print("stdout ");
  stdout_handle = sys_open(stdio_fname, semihosting::OPEN_MODE_W, strlen(stdio_fname));
  check_retcode(stdout_handle);
}

void check_features() {
  const int magiclen = 4;
  unsigned char magic[magiclen];
  unsigned char c;
  int fh;
  int len;
  sh.println("SYS_OPEN special filename ':semihosting-features'");
  char *features_fname=":semihosting-features";
  fh = sys_open(features_fname, semihosting::OPEN_MODE_R, strlen(features_fname));
  if (fh < 0) {
    sh.println("error open ':semihosting-features'\n");
    return;
  }
  len = sys_flen(fh);
  if (len < magiclen) {
    sh.println("error ':semihosting-features' too short\n");
    return;
  }
  int rc = sys_read(fh, magic, magiclen);
  if (rc != 0) {
    sh.println("error reading ':semihosting-features'\n");
    return;
  }
  if ((magic[0] != 'S') || (magic[1] != 'H') || (magic[2] != 'F') || (magic[3] != 'B')) {
    sh.println("error ':semihosting-features' bad magic\n");
    return;
  }
  sh.println("':semihosting-features' good magic\n");
  if (len > magiclen)
  for(int i = magiclen; i<len-1; i++) {
    char ch;
    if (sys_seek(fh, i) != 0) {
      sh.println("error seeking ':semihosting-features'\n");
      return;
    }
    if (sys_read(fh, &ch, 1) != 0) {
      sys_close(fh);
      return;
    }
    sh.print("feature byte: 0x");
    sh.println(ch, HEX);
  }
  sys_close(fh);
  return;
}

void check_write() {
  char abc[]="abcdefghijklmnopqrstuvwxyz";
  sh.println("SYS_WRITE stdout");
  sys_write(stdout_handle, abc, sizeof(abc));
  sh.println();
  sh.println("SYS_WRITE0 null-terminated string");
  sys_write0(abc);
  sh.println();
  sh.println("SYS_WRITEC character-at-a-time");
  for (int i = 0; i < sizeof(abc); i++) sys_writec(abc[i]);
  sh.println();
}

void check_read() {
  char buf[256]="";
  sh.println("SYS_READ stdin");
  sh.println("Type a line of text. End with newline.");
  int ret = sys_read(stdin_handle, buf, sizeof(buf));
  if (ret >= 0) {
    sh.print("number of characters read, including newline: ");
    sh.println(sizeof(buf)-ret);
  }
  sh.print("text: ");
  sh.println(buf);
  check_retcode(ret);
}

void check_readc() {
  char ch;
  char buf[256]={0};
  int i = 0;
  sh.println("SYS_READC");
  sh.println("Type a line of text. End with newline.");
  int ret;
  while ((ret = sys_readc()) != '\n') {
    if (i != sizeof(buf)) buf[i++] = ret;
    if (!isPrintable(ret)) {
      sh.println();
      sh.print("non-printable char, code: ");
      sh.print(ret);
      sh.print(" (0x");
      sh.print(ret, HEX);
      sh.println(")");
    };
  }
  sh.print("text: ");
  sh.println(buf);
}

void check_fileio() {
  char fnam1[]="file1.txt";
  char fnam2[]="file2.txt";
  char abc[]="abcdefghijklmnopqrstuvwxyz";
  char scmd[]="/bin/ls -l file*.txt";
  sh.println("SYS_OPEN write");
  int fh = sys_open(fnam1, semihosting::OPEN_MODE_W, strlen(fnam1));
  check_retcode(fh);
  sh.println("SYS_WRITE file");
  int ret = sys_write(fh, abc, sizeof(abc));
  check_retcode(ret);
  sh.println("SYS_CLOSE");
  ret = sys_close(fh);
  check_retcode(ret);
  sys_system(scmd, strlen(scmd));
  sh.println("SYS_RENAME");
  ret = sys_rename(fnam1, strlen(fnam1), fnam2, strlen(fnam2));
  check_retcode(ret);
  sys_system(scmd, strlen(scmd));
  sh.println("SYS_OPEN read");
  fh = sys_open(fnam2, semihosting::OPEN_MODE_R, strlen(fnam2));
  check_retcode(fh);
  sh.println("SYS_FLEN");
  int flen = sys_flen(fh);
  check_retcode(flen);
  if (flen == 27) sh.println("flen ok");
  else sh.println("flen error - check");
  sh.println("SYS_SEEK");
  ret = sys_seek(fh, 10);
  check_retcode(ret);
  char buf[256];
  sh.println("SYS_READ file");
  ret = sys_read(fh, buf, sizeof(buf));
  check_retcode(ret);
  if (strcmp(buf, "klmnopqrstuvwxyz") == 0) sh.println("read ok");
  else sh.println("read error- check");
  sh.println("SYS_CLOSE");
  ret = sys_close(fh);
  check_retcode(ret);
  sh.println("SYS_REMOVE");
  ret = sys_remove(fnam2, strlen(fnam2));
  check_retcode(ret);
  sys_system(scmd, strlen(scmd));
}

void check_iserror() {
  sh.println("SYS_ISERROR error codes");
  for (int i = 0; i< 100; i++)
    if (sys_iserror(i)) {sh.print(i); sh.print(' '); sh.flush(); }
    sh.print(' ');
  sh.println();
}

void check_istty() {
  sh.println("SYS_ISTTY");
  int ret = sys_istty(stdin_handle);
  check_retcode(ret);
}

void check_heapinfo() {
  // (gdb) monitor heapinfo 1 2 3 4
  sh.println("SYS_HEAPINFO");
  heapinfo_block_t heapinfo = {0xdeadbeef};
  sys_heapinfo(&heapinfo);
  sh.print("heap_base: "); sh.println(heapinfo.heap_base, HEX);
  sh.print("heap_limit: "); sh.println(heapinfo.heap_limit, HEX);
  sh.print("stack_base: "); sh.println(heapinfo.stack_base, HEX);
  sh.print("stack_limit: "); sh.println(heapinfo.stack_limit, HEX);
}

void check_system() {
  // (gdb) set remote system-call-allowed 1
  sh.println("SYS_SYSTEM");
  char cmd[]="/bin/echo system call works";
  int ret = sys_system(cmd, strlen(cmd));
  if (ret != 0) sh.println("enable system calls with the following gdb command: 'set remote system-call-allowed 1'");
  check_retcode(stdin_handle);
}

void check_errno() {
  sh.println("SYS_ERRNO");
  int ret = sys_errno();
  check_retcode(ret);
}

void check_elapsed() {
  sh.println("SYS_ELAPSED");
  uint64_t cnt;
  int ret = sys_elapsed(&cnt);
  check_retcode(ret);
}

void check_tickfreq() {
  sh.println("SYS_TICKFREQ");
  int ret = sys_tickfreq();
  check_retcode(ret);
}

void check_tmpnam() {
  char buf[256];
  sh.println("SYS_TMPNAM");
  int ret = sys_tmpnam(buf, 0, sizeof(buf));
  check_retcode(ret);
  if (ret == 0) {
    sh.print("tmpnam: ");
    sh.println(buf);
  }
}

void check_exit() {
  sh.println("SYS_EXIT");
  sys_exit(0);
  //sys_exit(ADP_Stopped_ApplicationExit, 0xdeadbeef);
}

void setup() {
  // put your setup code here, to run once:
  sh.println("semihosting testbench");
  check_getcmdline();
  check_stdio();
  check_features();
  check_write();
  check_read();
  check_readc();
  check_fileio();
  check_system();
  check_iserror();
  check_istty();
  check_errno();
  check_elapsed();
  check_tickfreq();
  check_tmpnam();
  check_heapinfo();
  check_clock();
  check_time();
  sh.println("semihosting testbench normal end");
  check_exit();
}

void loop() {
  // put your main code here, to run repeatedly:
}

// not truncated
