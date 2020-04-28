/*
 * Semihosting testbench - Tests all semihosting calls.
 *
 * This program uses semihosting and requires a debugger probe to run.
 * This program will hang if there is no debugger probe present.
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
  sgetcmdline(buf, sizeof(buf));
  sh.print("command line: ");
  sh.println(buf);
  if (strlen(buf) == 0)
    sh.println("Set command line using 'set args arg1 arg2 arg3' or 'run arg1 arg2 arg3'");
}

void check_clock() {
  sh.println("SYS_CLOCK time in hundredths of seconds");
  for (int i = 0; i < 5; i++) {
    int ret = sclock();
    sh.print(ret);
    sh.print(" ");
    check_retcode(ret);
    delay(1000);
  }
}

void check_time() {
  sh.println("SYS_TIME time in seconds");
  for (int i = 0; i < 5; i++) {
    int ret = stime();
    sh.print(ret);
    sh.print(" ");
    check_retcode(ret);
    delay(1000);
  }
}

void check_stdio() {
  sh.println("SYS_FOPEN special filename 'tt:'");
  char *stdio_fname=":tt";
  sh.print("stdin ");
  stdin_handle = sopen(stdio_fname, semihosting::OPEN_MODE_R, strlen(stdio_fname));
  check_retcode(stdin_handle);
  sh.print("stdout ");
  stdout_handle = sopen(stdio_fname, semihosting::OPEN_MODE_W, strlen(stdio_fname));
  check_retcode(stdout_handle);
}

void check_write() {
  char abc[]="abcdefghijklmnopqrstuvwxyz";
  sh.println("SYS_WRITE stdout");
  swrite(stdout_handle, abc, sizeof(abc));
  sh.println();
  sh.println("SYS_WRITE0 null-terminated string");
  swrite0(abc);
  sh.println();
  sh.println("SYS_WRITEC character-at-a-time");
  for (int i = 0; i < sizeof(abc); i++) swritec(abc[i]);
  sh.println();
}

void check_read() {
  char buf[256]="";
  sh.println("SYS_READ stdin");
  sh.println("Type a line of text. End with newline.");
  int ret = sread(stdin_handle, buf, sizeof(buf));
    if (ret > 0) {
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
  while ((ret = sreadc()) != '\n') {
    if (i != sizeof(buf)) buf[i++] = ret;
    if (!isalnum(ret)) {
      sh.println();
      sh.print("non-alphanumerical char, return code: ");
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
  int fh = sopen(fnam1, semihosting::OPEN_MODE_W, strlen(fnam1));
  check_retcode(fh);
  sh.println("SYS_WRITE file");
  int ret = swrite(fh, abc, sizeof(abc));
  check_retcode(ret);
  sh.println("SYS_CLOSE");
  ret = sclose(fh);
  check_retcode(ret);
  ssystem(scmd, strlen(scmd));
  sh.println("SYS_RENAME");
  ret = srename(fnam1, strlen(fnam1), fnam2, strlen(fnam2));
  check_retcode(ret);
  ssystem(scmd, strlen(scmd));
  sh.println("SYS_OPEN read");
  fh = sopen(fnam2, semihosting::OPEN_MODE_R, strlen(fnam2));
  check_retcode(fh);
  sh.println("SYS_FLEN");
  int flen = sflen(fh);
  ssystem(scmd, strlen(scmd));
  check_retcode(flen);
  if (flen == 27) sh.println("flen ok"); 
  else sh.println("flen error - check");
  sh.println("SYS_SEEK");
  ret = sseek(fh, 10);
  check_retcode(ret);
  char buf[256];
  sh.println("SYS_READ file");
  ret = sread(fh, buf, sizeof(buf));
  check_retcode(ret);
  if (strcmp(buf, "klmnopqrstuvwxyz") == 0) sh.println("read ok"); 
  else sh.println("read error- check");
  sh.println("SYS_CLOSE");
  ret = sclose(fh);
  check_retcode(ret);
  sh.println("SYS_REMOVE");
  ret = sremove(fnam2, strlen(fnam2));
  check_retcode(ret);
  ssystem(scmd, strlen(scmd));
} 

void check_iserror() {
  sh.println("SYS_ISERROR error codes");
  for (int i = 0; i< 100; i++)
    if (siserror(i)) {sh.print(i); sh.print(' '); sh.flush(); }
  sh.print(' ');
  sh.println();
}

void check_istty() {
  sh.println("SYS_ISTTY");
  int ret = sistty(stdin_handle);
  check_retcode(ret);
}

void check_heapinfo() {
  // (gdb) monitor heapinfo 1 2 3 4
  sh.println("SYS_HEAPINFO");
  heapinfo_block_t heapinfo = {0xdeadbeef};
  sheapinfo(&heapinfo);
  sh.print("heap_base: "); sh.println(heapinfo.heap_base, HEX);
  sh.print("heap_limit: "); sh.println(heapinfo.heap_limit, HEX);
  sh.print("stack_base: "); sh.println(heapinfo.stack_base, HEX);
  sh.print("stack_limit: "); sh.println(heapinfo.stack_limit, HEX);
}

void check_system() {
  // (gdb) set remote system-call-allowed 1
  sh.println("SYS_SYSTEM");
  char cmd[]="/bin/echo system call works";
  int ret = ssystem(cmd, strlen(cmd));
  if (ret != 0) sh.println("enable system calls with the following gdb command: 'set remote system-call-allowed 1'");
  check_retcode(stdin_handle);
}

void check_errno() {
  sh.println("SYS_ERRNO");
  int ret = serrno();
  check_retcode(ret);
}

void check_elapsed() {
  sh.println("SYS_ELAPSED");
  uint64_t cnt;
  int ret = selapsed(&cnt);
  check_retcode(ret);
}

void check_tickfreq() {
  sh.println("SYS_TICKFREQ");
  int ret = stickfreq();
  check_retcode(ret);
}

void check_tmpnam() {
  char buf[256];
  sh.println("SYS_TMPNAM");
  int ret = stmpnam(buf, 0, sizeof(buf));
  check_retcode(ret);
}

void check_exit() {
  sh.println("SYS_EXIT");
  sexit(0);
}

void setup() {
  // put your setup code here, to run once:
  sh.println("semihosting testbench");
  check_getcmdline();
  check_stdio();
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
