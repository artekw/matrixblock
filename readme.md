# matrixblock

The goal of this project is create a cape for beaglebone black to drive a [Sure Electronic 32x16 LED Display](http://www.sureelectronics.net/goods.php?id=1095).


## Hardware Connection

![matrix connection to beaglebone](https://dl.dropboxusercontent.com/u/677573/Photos/surelcd/surelcd_bbb_bb.png)

Power circuit from matrix connector, not beaglebone! Use power adapter 5V/2A.

## Compile the code

TODO

Examples:

    $ cd examples
    $ make
    $ ./test

Clock:

    $ make
    $ ./clock

### TODO

- python bindings or TCP deamon

### DONE

- print static text
- scroll text
- set brightness (in range 0-16)
- schematics and PCB design of cape
- DT code and EEEPROM firmware


### Source

Based on source code from https://github.com/dcuartielles/HT1632c
