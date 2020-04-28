/*
 * Blink - Blink led
 * To set the number of times the led will blink from the gdb command prompt:
 * set args 10
 *
 * This program uses semihosting and requires a debugger probe to run.
 * This program will hang if there is no debugger probe present.
 */

#include <SemihostingStream.h>
#include <semihosting_syscalls.h>

using namespace std;
using namespace semihosting;

int sec = 0;
int n;

SemihostingStream sh;

void setup() {
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);
  sh.println("Hello ARM world!");
  char buf[256];
  sgetcmdline(buf, sizeof(buf)); // set command line arguments with "set args" or "run".
  sh.print("command line: ");
  sh.println(buf);
  n = atoi(buf);
  if (n <= 0) n = 5;
  sh.print("will blink led ");
  sh.print(n);
  sh.println(" times");
}


void loop() {
  // put your main code here, to run repeatedly:

  // blinking led test
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  if (sec < n) digitalWrite(LED_BUILTIN, LOW);
  sh.print("Second ");
  sh.println(++sec);
}

/* not truncated */
