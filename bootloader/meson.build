project('loonix_bootloader', 'c')
arch = 'aarch64'
executable('bootloader.efi', 'src/main.c', 'src/elf.c', 'src/std.c',
    c_args : [
        '-c', '-target', arch+'-unknown-windows', '-ffreestanding', 
        '-mno-red-zone' ,'-std=c11' , '-Wall' , '-Werror' , '-pedantic', '-fshort-wchar',
        '-fuse-ld=lld-link', '-nostdlib'
        ],
    include_directories : ['../efi', 'include'],
    link_args : ['-Wl,-subsystem:efi_application' ,'-Wl,-entry:efi_main']
)
