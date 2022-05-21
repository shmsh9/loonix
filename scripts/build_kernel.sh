set -e
if [[ ${CROSS_COMPILE} != "" ]]
	then
		ARCH="$(echo $CROSS_COMPILE | cut -d '-' -f 1)"
	else
		ARCH="$(uname -m)"
fi
CC="${CROSS_COMPILE}gcc"
echo "[building for $ARCH]"
IFLAGS="-Ibootloader/include -Ignu-efi/inc/ -Ignu-efi/inc/protocol -Ikernel/include -Ignu-efi/inc/$ARCH"
LDFLAGS=""
CFLAGS="-fPIC -Wpedantic -Wshadow -Wall -Werror-implicit-function-declaration \
-DGNU_EFI_USE_MS_ABI -nodefaultlibs -nostdlib -ffreestanding \
-fno-stack-check -fno-stack-protector -fshort-wchar"
x86_64CFLAGS="-m64"
aarch64CFLAGS=""
EVIL="${ARCH}CFLAGS"
CFLAGS="${!EVIL} $CFLAGS"
echo $CFLAGS

echo "[building kernel]"
$CC $LDFLAGS $CFLAGS $IFLAGS -e entry kernel/src/*.c -o kernel/kernel.elf