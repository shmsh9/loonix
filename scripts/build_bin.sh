#!/bin/bash
set -e

CFLAGS="-D__MAKEWITH_GNUEFI -Wshadow -Wall -Bsymbolic -m64 -DGNU_EFI_USE_MS_ABI -nodefaultlibs -fno-stack-protector -nostdlib -ffreestanding -fshort-wchar -Iinclude -Ignu-efi/inc/"
IFLAGS="-Iinclude -Ignu-efi/inc/ -Istdlib/include"

#build userland
for binary in apps/*.c;
do
	gcc $CFLAGS $IFLAGS -fPIC -e entry "$binary" stdlib/src/*.c -o "${binary%.c}" gnu-efi/$(uname -m)/lib/libefi.a
done
