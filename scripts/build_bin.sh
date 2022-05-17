#!/bin/bash
set -e
CFLAGS="-nodefaultlibs -fno-stack-check -fno-merge-all-constants -fno-jump-tables -mno-red-zone -fno-omit-frame-pointer -fno-stack-protector -nostdlib -ffreestanding -fshort-wchar -Iinclude -Ignu-efi/inc/"
IFLAGS="-Iinclude -Ignu-efi/inc/ -Istdlib/include"

#build userland
for binary in apps/*.c;
do
	gcc $CFLAGS $IFLAGS -fPIC -e entry "$binary" stdlib/src/*.c -o "${binary%.c}" gnu-efi/$(uname -m)/lib/libefi.a
done
