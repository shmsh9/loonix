FLAGS="-nodefaultlibs -fno-stack-check -fno-merge-all-constants -fno-jump-tables -mno-red-zone -fno-omit-frame-pointer -fno-stack-protector -nostdlib -ffreestanding -fshort-wchar -Iinclude -Ignu-efi/inc/"
gcc $FLAGS -fPIC -e entry apps/test.c -o apps/test.bin
gcc $FLAGS -fPIC -e entry apps/ls.c -o apps/ls.bin
