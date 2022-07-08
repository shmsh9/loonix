#!/bin/bash
set -e
if [[ ! -f "$1" ]]
then
    echo "please provide kernel as argument"
    exit -1
fi
if [[ ${ARCH} == "" ]]
then
	ARCH="$(uname -m)"
fi
if [[ ${ARCH} == "x86_64" ]]
then
    EFIARCH="X64"
    FW_BASE="OVMF"
fi
if [[ ${ARCH} == "aarch64" ]]
then
    EFIARCH="AA64"
    QEMUOPTS="-M virt -cpu cortex-a57"
    FW_BASE="QEMU_EFI"
fi
BIOS="${FW_BASE}_${EFIARCH}.fd"
FW_ZIP="${FW_BASE}-${EFIARCH}.zip"
QEMU="qemu-system-${ARCH/-/_}"
rm -rf image/
mkdir -p image/efi/boot
cp -f bootloader/*.efi image/efi/boot

if [[ ! -f "$BIOS" ]]
then
    wget "https://efi.akeo.ie/${FW_BASE}/${FW_ZIP}"
    unzip "$FW_ZIP" "${FW_BASE}.fd"
    mv "${FW_BASE}.fd" "$BIOS"
    rm -f "$FW_ZIP"
fi
cp $1 image/kernel.elf
echo [$QEMU]
$QEMU $QEMUOPTS -m 256M -bios $BIOS -nographic -s -net none -drive format=raw,file=fat:rw:image
