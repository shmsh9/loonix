set -e
ARCH="$(uname -m)"
OFFSET="0x0"
if [[ $ARCH = "aarch64" ]]
then
		OFFSET="0x40001000"
fi
if [[ $ARCH = "x86_64" ]]
then
		OFFSET="0x1000000"
fi 
gdb -q \
		-ex "target remote localhost:1234"\
		-ex "add-symbol-file kernel/kernel.elf -s .text $OFFSET"\
