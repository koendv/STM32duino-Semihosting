#include <semihosting_syscalls.h>
#include <Semihosting.h>

/*
 * FileLength - determine length of file on host
 *
 * This program uses semihosting and requires a debugger probe to run.
 * This program will hang if there is no debugger probe present.
 */

using namespace semihosting;

SemihostingStream sh;

void setup() {
  // put your setup code here, to run once:
  sh.println("Hello ARM world!");
}

void loop() {
  // put your main code here, to run repeatedly:

  // length of file on host
  String fnam;
  int fhandle;

  sh.print("\nfilename? ");
  while(isWhitespace(sh.peek())) sh.read();
  fnam = sh.readStringUntil('\n');
  sh.print("file: ");
  sh.println(fnam);

  if ((fhandle = sopen(fnam.c_str(), semihosting::OPEN_MODE_R, fnam.length())) >= 0) {
    sh.print("file length: ");
    sh.println(sflen(fhandle));
    sclose(fhandle);
  } else sh.println("file open error");
}

/* not truncated */
