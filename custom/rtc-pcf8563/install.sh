#!/bin/sh
cp -a rtc_pcf8563 /usr/share/
cp rtc-pcf8563.service /usr/lib/systemd/system/
systemctl enable rtc-pcf8563
systemctl start rtc-pcf8563
print "Done!"

