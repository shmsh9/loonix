executable(
    'crt0.c','main.c','arch/aarch64_int.c','arch/x86_64_int.c','drivers/acpi.c','drivers/ahci.c',
    'drivers/framebuffer.c','drivers/pci.c','drivers/ps2.c','drivers/rtc.c','drivers/serial.c',
    'drivers/serial_pl011.c','drivers/serial_x86.c','drivers/vt100.c','graphics/bitmap_font.c',
    'graphics/graphics.c','kstd/karray.c','kstd/klist.c','kstd/kstd.c','kstd/kstring.c',
    'kstd/ktime.c','shell/builtins.c','shell/gol.c','shell/shell.c','sys/interrupt.c','sys/io.c',
    'sys/newmem.c','sys/task.c','sys/timer.c','x86_64boot.S','arch/x86_64_cpu.S',
    'arch/x86_64_int.S',
    c_args : [
        '-fstack-protector-strong', '-fstack-protector-all', '-fPIC', '-nostdlib', '-ffreestanding', '-std=c11',
    	'-Wno-unused-function', '-Wall', '-Werror', '-pedantic', '-Wno-unused-but-set-variable',
        '-Wno-strict-prototypes', '-target x86_64-none-elf', '-g', '-O0', 
        '-Wno-microsoft-static-assert', '-Wno-microsoft-enum-value', '-fshort-wchar'
        ],
    link_args: [
        '-flavor ld', '-T src/linkx86_64.ld'
    ],
    include_directories : ['../../efi', '../include', '../../bootloader/include'],
)
