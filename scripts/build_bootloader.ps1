#!/usr/bin/pwsh
$ErrorActionPreference = "stop"
$TARGET = "bootloader"
$OBJ = "bootx64.efi"
$CC = "clang"
$LD = "lld"
$ARCH_TABLE = @{
    "AMD64" = "x86_64"
    "aarch64" = "aarch64"
}
if($ARCH -eq $null){
    	$ARCH = $ARCH_TABLE[$env:PROCESSOR_ARCHITECTURE]
}
if($ARCH -eq "aarch64"){
    $OBJ="bootaa64.efi"
}

.\scripts\clean.ps1 $TARGET

Write-Host "[building $TARGET for $ARCH]"
$IFLAGS="-I$TARGET/include -Iefi"
$LDFLAGS="-flavor link -subsystem:efi_application -entry:efi_main"
$CFLAGS="-ffreestanding -mno-red-zone -std=c11 -Wall -Werror -pedantic -fshort-wchar -target $ARCH-unknown-windows"

foreach($object in (Get-ChildItem -Recurse $TARGET\src\*.c)){
    Write-Host "[CC] $($object.Fullname)"
    Invoke-Expression "$CC $CFLAGS $IFLAGS -c $($object.Fullname) -o $($object.Fullname.Replace(".c", ".o"))"
}
Invoke-Expression "$LD $LDFLAGS $(Get-ChildItem -Recurse $TARGET/src/*.o | Select-Object -ExpandProperty Fullname) -out:$TARGET/$OBJ"
