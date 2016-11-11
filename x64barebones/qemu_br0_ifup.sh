#!/bin/sh
switch=br0
echo "$0: adding tap interface \"$1\" to bridge \"$switch\""
ifconfig $1 0.0.0.0 up
brctl addif ${switch} $1
exit 0
