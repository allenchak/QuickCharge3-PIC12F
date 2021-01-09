# Introduction
This repository to record the detail information for using a DIY PIC programmer to upload the HEX file to PIC12F675. 

# Concept of Quick Charge 3.0
I have a USB Fan which is capable to using DC12V to power on. But there is a traditional switch at the back side, to control On or Off. Therefore I create a project using ArduinoUNO to steal a 12V from USB [QC3.0 protocol](https://github.com/vdeconinck/QC3Control) power adaptor.

# PIC12F675
I got a [PIC12F675](datasheet/PIC12F675.pdf) couple years ago. It is a 8-bit MCU, 6 GPIOs, run as low as 2v - 5.5v. I had explorer how to program it without PICKit2 / PICKit3. Fortunately, we have [ardpicprog](https://github.com/rweather/ardpicprog) nowaday.

# Parts Requires
1. DC12V Power supply, or using 12v (up to 13.5v) battery pack, or Linear Voltage regulator (Such as LM7812) with 15V up higher power supply
2. ArduinoUNO (Or Nano, ProMini, etc)
3. 1 x NPN transistor (BC548, BC337 or equivalent)
4. 1 x 10K Ohm
5. 1 x 4.7K Ohm
6. 1 x 1K Ohm
7. 1 x 220 Ohm
8. 2 x LED (Red & Yellow)
9. 1 x Capacitor (100nF)
10. 1 x breadboard

# Details
1. Prepare the [programmer](http://rweather.github.io/ardpicprog/pic14_zif_circuit.html)
2. Upload the [Sketch](http://rweather.github.io/ardpicprog/programpic_sketch.html) to ArduinoUNO, suggest using the Sketch download from [Github](https://github.com/rweather/ardpicprog/blob/master/ProgramPIC/ProgramPIC.pde)
3. Compile the Host program, `ardpicprog` by `Make` (I am using macOS)
4. Upload the HEX file to MCU by `ardpicprog -p PORT -d pic12f675 --erase --burn -i blink.hex`

# Hex File
I am using [MPLAB X IDE](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-x-ide) to compile the source to [HEX files](https://github.com/allenchak/QuickCharge3-PIC12F/tree/main/precompiled). Moreover, listed here with following features:
1. Support 12V, no LED, no EEPROM
2. Support 12V, LED
3. Support 12V, EEPROM
4. Support 12V, LED, EEPROM
5. Support 20V, no LED, no EEPROM
6. Support 20V, LED
7. Support 20V, EEPROM
8. Support 20V, LED, EEPROM

# Program
The QC controlling program will switching the output voltage as 5v, 9v, 12v, 20v. Toggle by a press button. While the voltage was changing, the LED will turn ON. The output mode will storing to internal EEPROM. Detail process could be found [here](MPLAB%20X%20IDE/QC3_Control.X/main.c).


