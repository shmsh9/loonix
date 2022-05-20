#!/bin/bash
set -e
ARCH="$(uname -m)"
CFLAGS="-fpic -D__MAKEWITH_GNUEFI -Wshadow -Wall -Bsymbolic -m64 -DGNU_EFI_USE_MS_ABI -nodefaultlibs -fno-stack-protector -nostdlib -ffreestanding -fshort-wchar"
IFLAGS="-Iinclude -Ignu-efi/inc/ -Istdlib/include -Ignu-efi/inc/$ARCH"
#build "libc"
echo "[building stdlib]"
for c in stdlib/src/*.c
do
	echo "[CC] $c"
	gcc -c $CFLAGS $IFLAGS $c -o "${c%.c}.o"
done
ar rcs stdlib/stdlib.a stdlib/src/*.o
rm stdlib/src/*.o
echo "[building userland]"
#build userland
for binary in apps/*.c;
do
	echo "[CC] $binary"
	gcc $CFLAGS $IFLAGS -e entry "$binary" -o "${binary%.c}" stdlib/stdlib.a
done
