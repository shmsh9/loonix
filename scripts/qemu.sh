#!/bin/bash
set -e
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

rm -rf bootloader/*.efi
rm -rf kernel/*.elf
rm -rf image/
bash scripts/build_all.sh
mkdir -p image/efi/boot
cp -f bootloader/*.efi image/efi/boot
cp -f kernel/*.elf image
if [[ ! -f "$BIOS" ]]
then
    wget "https://efi.akeo.ie/${FW_BASE}/${FW_ZIP}"
    unzip "$FW_ZIP" "${FW_BASE}.fd"
    mv "${FW_BASE}.fd" "$BIOS"
    rm -f "$FW_ZIP"
fi

$QEMU $QEMUOPTS -bios $BIOS -nographic -s -net none -hda fat:rw:image