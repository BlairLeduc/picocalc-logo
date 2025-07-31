# picocalc-logo

This project implements a dialect of the computer language Logo for the PicoCalc. The dialect is inspired by Berkley Logo, FMSLogo and Tarrapin Logo, with modifications to fit the constraints of the PicoCalc, and with enhanced features to leverage the capabilities of the PicoCalc.

![screenshot](assets/screenshot-1.bmp)

Logo is a programming language known for its use in education, particularly in teaching programming concepts through turtle graphics. The PicoCalc Logo interpreter allows users to write Logo programs that can control the turtle graphics on the PicoCalc device.

## Features

- 320×320 resolution with double-buffered 65K colour turtle graphics
- Three simultaneous display modes: full screen text for programs without graphics, full screen graphics for running graphical programs, and split screen for interactive use
- Full line editing and history
- Full support for the Logo language, including procedures, recursion, and control structures
- All mathematical operations are single-precision floating-point arithmetic
- Saving and loading of programs to and from a SD card formatted as FAT32

## Recommended Requirements

- Raspberry Pi Pico 2 or compatible board (RP2350) for required floating-point unit
- PicoCalc device with the latest firmware

> [!NOTE]
> UF2 files for the original PicoCalc are available and can be used to run PicoCalc Logo, however, without a floating-point unit, Logo will run about 5x slower.

## Getting Started

For more detailed information, tutorials, and guides, please visit the [PicoCalc Logo Wiki](https://github.com/BlairLeduc/picocalc-logo/wiki).
