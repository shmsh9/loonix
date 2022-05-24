set -e
TARGET="kernel"
OBJ="kernel.elf"
if [[ ${ARCH} == "" ]]
then
	ARCH="$(uname -m)"
fi
CC="clang"
LD="lld"
echo "[building $TARGET for $ARCH]"
IFLAGS="-I$TARGET/include -Ibootloader/include -Iefi"
LDFLAGS="-flavor ld -e main"
CFLAGS="-fpic -ffreestanding -mno-red-zone -std=c11 \
		-Wno-unused-function -Wall -Werror -pedantic -fshort-wchar"
x86_64CFLAGS="-target x86_64-unknown-gnu"
aarch64CFLAGS="-target aarch64-unknown-eabi"
EVIL="${ARCH}CFLAGS"
CFLAGS="${!EVIL} $CFLAGS"

for object in $(find $TARGET/src/ -name "*.c")
do
	echo "[CC] $object"
	$CC $CFLAGS $IFLAGS -c $object -o "${object%.c}.o"
done
$LD $LDFLAGS $TARGET/src/*.o $TARGET/src/drivers/*.o -o $TARGET/$OBJ
rm -rf $target/src/*.o
