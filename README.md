# mittens
Handles microcontrollers comfortably.

# About
Mittens is a basic set of code templates that can be used across a number of
bare-metal embedded platforms. It is meant to speed application development
time, reduce vendor dependencies, etc. It will probably be optimized for 16-bit
platforms, occasionally tested on 32-bit platforms, and not officially tested
on 8-bit platforms. It's not quite an operating system.

# Goals
- System parameters defined at compile-time through #defines and enums
- Optimized for 16-bit platforms
- As little complexity as possible

# Guide
## app/
Contains code that is specific to the application.

## os/
Contains code that is used for managing mittens. Nothing in here should be
hardware- or application-dependent.

## port/
Contains implementations of hardware-dependent code as required by the
abstracted calls from os/ code.

## port/config.h
Contains a number of definitions that are specific to the application's
requirements of the MCU's peripherals. The use of this file is subject to
change as the project develops.
