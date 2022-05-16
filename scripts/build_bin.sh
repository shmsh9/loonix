#!/bin/bash
set -e
FLAGS="-nodefaultlibs -fno-stack-check -fno-merge-all-constants -fno-jump-tables -mno-red-zone -fno-omit-frame-pointer -fno-stack-protector -nostdlib -ffreestanding -fshort-wchar -Iinclude -Ignu-efi/inc/"
for binary in apps/*.c;
do
	gcc $FLAGS -fPIC -e entry "$binary" -o "${binary%.c}" gnu-efi/$(uname -m)/lib/libefi.a
done
