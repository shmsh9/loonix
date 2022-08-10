#!/usr/bin/pwsh

$ErrorActionPreference = "stop"
$TARGET="kernel"
$OBJ="kernel.elf"
$CC="clang"
$LD="ld.lld"

$ARCH_TABLE = @{
    "x86" = "x86_64"
    "AMD64" = "x86_64"
    "aarch64" = "aarch64"
}
if($ARCH -eq $null){
    	$ARCH = $ARCH_TABLE[$env:PROCESSOR_ARCHITECTURE]
}


.\scripts\clean.ps1 $TARGET

Write-Host "[building $TARGET for $ARCH]"
$IFLAGS="-I$TARGET/include -Ibootloader/include -Iefi"
$ABI="${ARCH}-none-elf"
$LDFLAGS=""
$CFLAGS="-fstack-protector-strong -fstack-protector-all -fPIC -nostdlib -ffreestanding -mno-red-zone -std=c11 -Wno-unused-function -Wall -Werror -pedantic -target $ABI -g -O0"
foreach($object in (Get-ChildItem -Recurse $TARGET\src\*.c)){
    Write-Host "[CC] $($object.Fullname)"
    Invoke-Expression "$CC $CFLAGS $IFLAGS -c $($object.Fullname) -o $($object.Fullname.Replace(".c", ".o"))"
}
foreach($object in (Get-ChildItem -Recurse $TARGET\src\$ARCH*.S)){
    Write-Host "[CC] $($object.Fullname)"
    Invoke-Expression "$CC -target $ABI $IFLAGS -c $($object.Fullname) -o $($object.Fullname.Replace(".S", ".o"))"
}
Invoke-Expression "$LD -T '$TARGET/src/link$ARCH.ld' $LDFLAGS $(Get-ChildItem -Recurse $TARGET/*.o | Select-Object -ExpandProperty Fullname) -o $TARGET/$OBJ"
#objcopy --only-keep-debug "$TARGET/$OBJ" "$TARGET/${OBJ}.dbg"
#strip "$TARGET/$OBJ"
