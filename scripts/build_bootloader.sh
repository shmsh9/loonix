set -e
TARGET="bootloader"
OBJ="bootx64.efi"
if [[ ${ARCH} == "" ]]
then
	ARCH="$(uname -m)"
fi
if [[ ${ARCH} == "aarch64" ]]
then
    OBJ="bootaa64.efi"
fi
CC="clang"
LD="lld"
echo "[building $TARGET for $ARCH]"
IFLAGS="-I$TARGET/include -Iefi"
LDFLAGS="-flavor link -subsystem:efi_application -entry:efi_main"
CFLAGS="-ffreestanding -MMD -mno-red-zone -std=c11 \
		-Wall -Werror -pedantic -fshort-wchar"
x86_64CFLAGS="-target x86_64-unknown-windows"
aarch64CFLAGS="-target aarch64-unknown-windows"
EVIL="${ARCH}CFLAGS"
CFLAGS="${!EVIL} $CFLAGS"

for object in $TARGET/src/*.c
do
	echo "[CC] $object"
	$CC $CFLAGS $IFLAGS -c $object -o "${object%.c}.o"
done
$LD $LDFLAGS $TARGET/src/*.o -out:$TARGET/$OBJ
rm -rf $target/src/*.o
