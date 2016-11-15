#!/bin/bash
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -device rtl8139,netdev=net0,mac=DE:AD:BE:EF:22:22 -netdev tap,id=net0,ifname=tap2,script=no,downscript=no