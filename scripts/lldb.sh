set -e
OFFSET="0x0"
if [[ -z "$ARCH" ]]
then
	ARCH="$(uname -m)"
fi
if [[ $ARCH = "aarch64" ]]
then
		OFFSET="0x40000000"
fi
if [[ $ARCH = "x86_64" ]]
then
		OFFSET="0x1000000"
fi 
lldb kernel/kernel.elf \
	-o "gdb-remote localhost:1234"\
	-o "image load -f kernel.elf -s $OFFSET"
