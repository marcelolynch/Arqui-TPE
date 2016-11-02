#!/bin/bash
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -net nic,model=rtl8139 -net user -redir tcp:5555::80 -redir tcp:5556::445