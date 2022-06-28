set -e
TARGET="kernel"
OBJ="kernel.elf"
if [[ ${ARCH} == "" ]]
then
	ARCH="$(uname -m)"
fi
CC="clang"
LD="lld"

bash scripts/clean.sh $TARGET

echo "[building $TARGET for $ARCH]"
IFLAGS="-I$TARGET/include -Ibootloader/include -Iefi"
LDFLAGS="-flavor ld -e _start"
ABI="${ARCH}-unknown-gnu"
CFLAGS="-fPIC -nostdlib -ffreestanding -mno-red-zone -std=c11 \
		-Wno-unused-function -Wall -Werror -pedantic -fshort-wchar \
		-target $ABI -ggdb"

for object in $(find $TARGET/src/ -name "*.c")
do
	echo "[CC] $object"
	$CC $CFLAGS $IFLAGS -c $object -o "${object%.c}.o"
done
$CC -fPIC -c -target $ABI $TARGET/src/boot${ARCH}.s -o $TARGET/src/boot${ARCH}.o
$LD $LDFLAGS $(find $TARGET -name "*.o") -o $TARGET/$OBJ

