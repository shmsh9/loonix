#!/usr/bin/python3
import os
import config
import build_all
import shutil
param = config.get_params()
qemu_memory = "128M"
if param['ARCH'] == "x86_64":
    qemu_flags = "-M q35"
    qemu = "qemu-system-x86_64"
    bios = "OVMF_X64.fd"
    bootloader = "bootx64.efi"

if param['ARCH'] == "aarch64":
    qemu_flags = "-M virt-2.12 -cpu cortex-a57 -device VGA"
    qemu = "qemu-system-aarch64"
    bios = "QEMU_EFI_AA64.fd"
    bootloader = "bootaa64.efi"
if not os.path.exists("image/"):
    os.makedirs("image/efi/boot")
config.clean_files_glob("image/", "*.elf")
config.clean_files_glob("image/", "*.efi")
shutil.copyfile("kernel/kernel.elf", "image/kernel.elf")
shutil.copyfile(f"bootloader/{bootloader}", f"image/efi/boot/{bootloader}")

print(f"[{qemu}]")
os.system(f"{qemu} {qemu_flags} -no-reboot -d int,cpu_reset -m {qemu_memory} -bios {bios} -serial stdio -s -net none -drive format=raw,file=fat:rw:image")