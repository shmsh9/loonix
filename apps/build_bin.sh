gcc -mno-red-zone -fno-omit-frame-pointer -fno-stack-protector -fPIC -nostdlib -ffreestanding -fshort-wchar -Iinclude -Ignu-efi/inc/ -e main apps/test.c -o apps/test.bin
