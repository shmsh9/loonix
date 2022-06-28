set -e

gdb -q -ex "file kernel/kernel.elf" -ex "target remote localhost:1234"
