set -e
if [[ ${CROSS_COMPILE} != "" ]]
	then
		ARCH="$(echo $CROSS_COMPILE | cut -d '-' -f 1)"
	else
		ARCH="$(uname -m)"
fi
CC="${CROSS_COMPILE}gcc"
echo "[building for $ARCH]"
IFLAGS="-Iinclude -Ignu-efi/inc/ -Ignu-efi/inc/protocol -Istdlib/include -Ignu-efi/inc/$ARCH"
LDFLAGS="-Wl,--defsym=EFI_SUBSYSTEM=10 -Lgnu-efi/$ARCH/lib -fpic \
-s -Wl,-Bsymbolic -nostdlib -shared"
CFLAGS="-Wpedantic -Wshadow -Wall -Werror-implicit-function-declaration \
-DGNU_EFI_USE_MS_ABI -D__MAKEWITH_GNUEFI -DCONFIG_$ARCH \
-nodefaultlibs -ffreestanding -fno-strict-aliasing \
-fno-merge-all-constants -fno-stack-check \
-fno-stack-protector -fshort-wchar \
-fno-jump-tables --std=c11"
x86_64CFLAGS="-m64"
aarch64CFLAGS=""
EVIL="${ARCH}CFLAGS"
CFLAGS="${!EVIL} $CFLAGS"
echo $CFLAGS

#build "libc"
echo "[building stdlib]"
for c in stdlib/src/*.c
do
	echo "[CC] $c"
	$CC -c $CFLAGS $IFLAGS $c -o "${c%.c}.o"
done
ar rcs stdlib/stdlib.a stdlib/src/*.o
rm stdlib/src/*.o

#build userland
echo "[building userland]"
for binary in apps/*.c;
do
	echo "[CC] $binary"
	$CC $LDFLAGS $CFLAGS $IFLAGS -e entry "$binary" -o "${binary%.c}" stdlib/stdlib.a
done
