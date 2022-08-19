project('loonix_bootloader', 'c')
executable('bootloader.efi', 'src/main.c', 'src/elf.c', 'src/std.c',
    c_args : [
        '-c', '-target', 'x86_64-unknown-windows', '-ffreestanding', 
        '-mno-red-zone' ,'-std=c11' , '-Wall' , '-Werror' , '-pedantic', '-fshort-wchar',
        ],
    include_directories : ['../efi', 'include'],
    link_args : ['-fuse-ld=lld-link', '-Wl,-flavor link' ,'-Wl,-subsystem:efi_application' ,'-Wl,-entry:efi_main']
)
