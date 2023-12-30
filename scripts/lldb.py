#!/usr/bin/python3
import config
import os

param = config.get_params()
if param['ARCH'] == "x86_64":
    offset = "0x1000000"
if param['ARCH'] == "aarch64":
    offset = "0x40100000"

os.system(f"lldb kernel/kernel.elf -o 'gdb-remote localhost:1234' -o 'image load -f kernel.elf'")