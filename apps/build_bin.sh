FLAGS="-nodefaultlibs -fno-stack-check -fno-merge-all-constants -fno-jump-tables -mno-red-zone -fno-omit-frame-pointer -fno-stack-protector -nostdlib -ffreestanding -fshort-wchar -Iinclude -Ignu-efi/inc/"
rm apps/*.a
rm apps/*.o
gcc $FLAGS -c apps/lc.c -o apps/lc.o
ar rcs apps/lc.a apps/lc.o
gcc $FLAGS -fPIC -e main apps/test.c -o apps/test.bin apps/lc.a

