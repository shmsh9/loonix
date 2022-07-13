set -e
TARGET="kernel"
OBJ="kernel.elf"
if [[ ${ARCH} == "" ]]
then
	ARCH="$(uname -m)"
fi
CC="clang"
LD="ld.lld"

bash scripts/clean.sh $TARGET

echo "[building $TARGET for $ARCH]"
IFLAGS="-I$TARGET/include -Ibootloader/include -Iefi"
ABI="${ARCH}-none-elf"
LDFLAGS=""
CFLAGS="-fPIC -nostdlib -ffreestanding -mno-red-zone -std=c11 \
		-Wno-unused-function -Wall -Werror -pedantic \
		-target $ABI -ggdb"

for object in $(find $TARGET/src/ -name "*.c")
do
	echo "[CC] $object"
	$CC $CFLAGS $IFLAGS -c $object -o "${object%.c}.o"
done
for object in $(find $TARGET/src/ -name "${ARCH}*.S")
do
	echo "[CC] $object"
	$CC $IFLAGS -target $ABI -c $object -o "${object%.S}.o"
done
#$CC -target $ABI -c $TARGET/src/boot${ARCH}.s -o $TARGET/src/boot${ARCH}.o

$LD -T "${TARGET}/src/link${ARCH}.ld" $LDFLAGS $(find $TARGET -name "*.o") -o $TARGET/$OBJ

