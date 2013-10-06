#!/bin/sh
cp led-config /usr/bin/
chmod +x /usr/bin/led-config

cp leds /etc/default/
cp led.service /usr/lib/systemd/system/
echo "Done!"

