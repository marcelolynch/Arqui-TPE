#!/bin/bash
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -net nic,model=rtl8139,macaddr='DE:AD:BE:EF:00:AA' -net user -net tap,ifname=tap0