project('loonix_kernel', 'c')
arch = 'x86_64'
executable('kernel.elf', 'src/main.c', 'src/kstd.c', 'src/io.c',
            'src/crt0.c', 'src/acpi.c', 'src/newmem.c', 
            'src/irq/irq.c', 'src/graphics/font8x8.c', 'src/graphics/graphics.c',
            'src/drivers/framebuffer.c', 'src/drivers/ps2.c', 'src/drivers/serial.c',
            'src/drivers/vt100.c', 'src/shell/builtins.c', 'src/shell/shell.c',
            'src/'+arch+'boot.S', 'src/irq/'+arch+'asmirq.S',
    c_args : [
       '-c', '-fstack-protector-strong', '-fstack-protector-all', '-fPIC', '-nostdlib' ,'-ffreestanding', '-std=c11',
    	'-fuse-ld=lld', '-Wno-unused-function', '-Wall', '-Werror', '-pedantic', '-Wno-unused-but-set-variable',
        '-g', '-O0', '-target '+arch+'-none-elf'
        ],
    include_directories : ['../efi', 'include', '../bootloader/include'],
    link_args : ['-Wl,-flavor ld -T kernel/src/link'+arch+'.ld','-Wl,-subsystem:efi_application' ,'-Wl,-entry:efi_main']
)
