set -e
if [[ ${CROSS_COMPILE} != "" ]]
	then
		ARCH="$(echo $CROSS_COMPILE | cut -d '-' -f 1)"
	else
		ARCH="$(uname -m)"
fi
CC="${CROSS_COMPILE}gcc"
LD="${CROSS_COMPILE}ld"
echo "[building for $ARCH]"
IFLAGS="-Ibootloader/include -Ignu-efi/inc/ -Ignu-efi/inc/protocol -Ikernel/include -Ignu-efi/inc/$ARCH"
LDFLAGS="-shared -fPIC -e entry"
CFLAGS="-fPIC -mno-red-zone -pedantic -Wshadow -Wall -Werror-implicit-function-declaration \
-DGNU_EFI_USE_MS_ABI -nodefaultlibs -nostdlib -ffreestanding \
-fno-stack-check -fno-stack-protector -fshort-wchar"
x86_64CFLAGS="-m64"
aarch64CFLAGS=""
EVIL="${ARCH}CFLAGS"
CFLAGS="${!EVIL} $CFLAGS"
echo $CFLAGS

echo "[building kernel]"
for object in kernel/src/*.c
do
	$CC $CFLAGS $IFLAGS -c $object -o "${object%.c}.o"
done
$LD $LDFLAGS kernel/src/*.o -o kernel/kernel.elf
rm -rf kernel/src/*.o
