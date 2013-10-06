#!/bin/sh
echo pcf8563 0x51 > /sys/class/i2c-adapter/i2c-1/new_device
hwclock -s -f /dev/rtc0
#hwclock -w
