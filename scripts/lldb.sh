set -e
ARCH="$(uname -m)"
OFFSET="0x0"
if [[ $ARCH = "aarch64" ]]
then
		OFFSET="0x40000000"
fi
if [[ $ARCH = "x86_64" ]]
then
		OFFSET="0x100000"
fi 
lldb kernel/kernel.elf \
	-o "gdb-remote localhost:1234"\
	-o "image load -f kernel.elf -s $OFFSET"
