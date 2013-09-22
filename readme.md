# matrixblock

The goal of this project is create a cape for beaglebone black to drive a [Sure Electronic 32x16 LED Display](http://www.sureelectronics.net/goods.php?id=1095).

The plan is to put on PCB RTC clock based on NXP PCF8563 chip.

## Hardware Connection

![matrix connection to beaglebone](https://dl.dropboxusercontent.com/u/677573/Photos/surelcd/surelcd_bbb_bb.png)

Power circuit from matrix connector, not beaglebone! Use power adapter 5V/2A.

## Compile the code

TODO

Examples:

    $ cd examples
    $ make
    $ ./test

Daemon:

    $ pacman -Sy json-c
    $ make
    $ ./daemon

### TODO

- python bindings or TCP deamon
- cape schematics and PCB


### Source

Based on source code from https://github.com/dcuartielles/HT1632c
