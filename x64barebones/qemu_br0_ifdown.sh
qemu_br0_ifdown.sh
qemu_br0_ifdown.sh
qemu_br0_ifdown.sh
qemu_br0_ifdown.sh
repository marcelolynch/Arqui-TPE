#!/bin/sh
switch=br0
echo "$0: deleting tap interface \"$1\" from bridge \"$switch\""
brctl delif $switch $1
ifconfig $1 0.0.0.0 down
exit 0
