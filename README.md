# stm32duino-semihosting
Arm Arduino semihosting library

With semihosting an arduino arm system can do keyboard input, screen output, and file I/O on the pc where the debugger is running. To run a semihosting program, you need a debugger probe to connect the program on your arduino to the debugger on your pc. Semihosting only runs on systems with arm processors, sorry.

# Installation
* install the [arduino IDE](https://www.arduino.cc/)
* install [stm32duino library](https://github.com/stm32duino/Arduino_Core_STM32)
* install [stm32duino-semihosting library](https://github.com/koendv/stm32duino-semihosting) (this library)
* connect a debugger probe with semihosting support, e.g. a [segger](https://www.segger.com/products/debug-probes/j-link/) or a [black magic probe](https://github.com/blacksphere/blackmagic/wiki).
* run examples from File->Examples->STM32duino-Semihosting




