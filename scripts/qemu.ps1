#!/usr/bin/pwsh

$ErrorActionPreference = "stop"
$ARCH_TABLE = @{
    "x86" = "x86_64"
    "AMD64" = "x86_64"
    "aarch64" = "aarch64"
}
if($ARCH -eq $null){
    	$ARCH = $ARCH_TABLE[$env:PROCESSOR_ARCHITECTURE]
}

if($ARCH -eq "x86_64"){
    $EFIARCH="X64"
    $FW_BASE="OVMF"
}
if($ARCH -eq "aarch64"){
    $EFIARCH="AA64"
    $QEMUOPTS="-M virt -cpu cortex-a57 -device virtio-gpu-pci"
    $FW_BASE="QEMU_EFI"
}
$BIOS="$($FW_BASE)_$($EFIARCH).fd"
$FW_ZIP="$FW_BASE-$EFIARCH.zip"
$QEMU="qemu-system-$ARCH"

if(Test-Path -Path "image"){
    Remove-Item -Recurse -Force image
}
.\scripts\build_all.ps1
New-Item -ItemType Directory -Force image\efi\boot | Out-Null
Copy-Item -Force bootloader/*.efi image/efi/boot
Copy-Item -Force kernel/*.elf image
if(!(Test-Path -Path $BIOS)){
    Write-Host "Downloading : https://efi.akeo.ie/${FW_BASE}/${FW_ZIP}"
    curl.exe -OLk "https://efi.akeo.ie/${FW_BASE}/${FW_ZIP}"
    Expand-Archive $FW_ZIP "$FW_BASE.fd"
    Rename-Item "$FW_BASE.fd" $BIOS
    Remove-Item -Force $FW_ZIP
}
Write-Host "[$QEMU]"
Invoke-Expression "$QEMU $QEMUOPTS -m 128M -bios $BIOS -nographic -net none -drive format=raw,file=fat:rw:image"
