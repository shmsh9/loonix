project('loonix_kernel', 'c')
executable('kernel.elf', 'src/main.c', 'src/kstd.c', 'src/io.c',
            'src/crt0.c', 'src/acpi.c', 'src/newmem.c', 
            'src/irq/irq.c', 'src/graphics/font8x8.c', 'src/graphics/graphics.c',
            'src/drivers/framebuffer.c', 'src/drivers/ps2.c', 'src/drivers/serial.c',
            'src/drivers/vt100.c', 'src/shell/builtins.c', 'src/shell/shell.c',
            'src/x86_64boot.S', 'src/irq/x86_64asmirq.S',
    c_args : [
        '-c',
        '-fstack-protector-strong', '-fstack-protector-all', '-fPIC', '-nostdlib' ,'-ffreestanding', '-std=c11',
    	'-Wno-unused-function', '-Wall', '-Werror', '-pedantic', '-Wno-unused-but-set-variable',
        '-g', '-O0'
        ],
    include_directories : ['../efi', 'include', '../bootloader/include'],
)
configure_file(input: 'src/linkaarch64.ld', output: 'linkaarch64.ld', copy: true)
configure_file(input: 'src/linkx86_64.ld', output: 'linkx86_64.ld', copy: true)