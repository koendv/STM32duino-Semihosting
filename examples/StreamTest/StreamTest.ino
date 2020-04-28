/*
 * StreamTest - demo of semihosting input and output fron the gdb console
 *
 * This program uses semihosting and requires a debugger probe to run.
 * This program will hang if there is no debugger probe present.
 */

#include <SemihostingStream.h>

SemihostingStream sh;

void setup() {
  // put your setup code here, to run once:
  sh.println("Hello world!");
}

void loop() {
  // put your main code here, to run repeatedly:
  sh.print("enter integer number: ");
  int i =  sh.parseInt();
  sh.print("\nvalue: ");
  sh.print(i);
  sh.print(" = 0x");
  sh.print(i, HEX);
  sh.println();
  
  sh.print("enter floating point number: ");
  float f = sh.parseFloat();
  sh.print("\nvalue: ");
  sh.println(f);

  
  sh.print("enter string: ");
  while (!isAlphaNumeric(sh.peek())) sh.read();
  String s = sh.readStringUntil('\n');
  sh.print("\nvalue: ");
  sh.println(s);
}
