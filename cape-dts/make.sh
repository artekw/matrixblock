#!/bin/sh
dtc -O dtb -o BB-BONE-HT1632C-00A0.dtbo -b 0 -@ BB-BONE-HT1632C-00A0.dts
mv BB-BONE-HT1632C-00A0.dtbo /lib/firmware/
