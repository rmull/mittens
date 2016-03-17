# mittens
Handles microcontrollers comfortably.

# About
Mittens is a basic set of code templates that can be used across a number of
bare-metal embedded platforms. It is meant to speed application development
time, reduce vendor dependencies, etc. It will be optimized for 16-bit
platforms, tested on 32-bit platforms, and not officially tested on 8-bit
platforms. Is it an operating system? Sure, maybe. 

Note: Currently being developed on a TI Stellaris Launchpad (EK-TM4C123GXL).
This is an ARM Cortex-M4F. Future ports may include TI MSP430, Microchip PIC24
and PIC32, and Renesas RL78. Complete ports are a secondary goal to actual
functionality on the primary platform.

# Goals
- System parameters defined at compile-time through #defines and enums
- Optimized for 16-bit platforms
- Very little complexity, very little overhead

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

# Recommendations
- Set up your development environment so that the project's metadata all resides
together in one git repository, and check out mittens as a submodule within
that project's repo.
