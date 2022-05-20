set -e
if [[ ${CROSS_COMPILE} != "" ]]
	then
		ARCH="$(echo $CROSS_COMPILE | cut -d '-' -f 1)"
	else
		ARCH="$(uname -m)"
fi
CC="${CROSS_COMPILE}gcc"
echo "[building for $ARCH]"
CFLAGS="-g -s -fpic -D__MAKEWITH_GNUEFI -Wshadow -Wall \
-Bsymbolic -DGNU_EFI_USE_MS_ABI -nodefaultlibs \
-fno-strict-aliasing -fno-merge-all-constants -fno-stack-check \
-fno-stack-protector -nostdlib -ffreestanding -fshort-wchar -DCONFIG_$ARCH \
--std=c11 -DGNU_EFI_USE_MS_ABI"
x86_64CFLAGS="-m64"
aarch64CFLAGS="-march=armv8-a"
EVIL="${ARCH}CFLAGS"
CFLAGS="${!EVIL} $CFLAGS"
echo $CFLAGS
IFLAGS="-Iinclude -Ignu-efi/inc/ -Istdlib/include -Ignu-efi/inc/$ARCH"

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
	$CC $CFLAGS $IFLAGS -e entry "$binary" -o "${binary%.c}" stdlib/stdlib.a
done
