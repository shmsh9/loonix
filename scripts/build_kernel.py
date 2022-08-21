#!/usr/bin/python3
import config
def main():
    target = "kernel"
    out_file = f"{target}/kernel.elf"
    param = config.get_params()
    inc_flags = f"-I{target}/include -Iefi -Ibootloader/include"
    c_files = config.get_files_glob(f"{target}/src/","*.c")
    s_files = config.get_files_glob(f"{target}/src/", f"{param['ARCH']}*.S")
    c_flags = f"-c {inc_flags} -fstack-protector-strong -fstack-protector-all -fPIC -nostdlib -ffreestanding -std=c11 \
    		-Wno-unused-function -Wall -Werror -pedantic -Wno-unused-but-set-variable\
            -target {param['ARCH']}-none-elf -g -O0 -Wno-microsoft-static-assert -Wno-microsoft-enum-value -fshort-wchar"
    ld_flags = f"-flavor ld -T {target}/src/link{param['ARCH']}.ld"
    config.clean_files_glob(f"{target}/src/","*.o")
    config.clean_files_glob(f"{target}/src/","*.elf")
    config.compile_c_files(param, c_flags, c_files)
    config.compile_s_files(param, c_flags, s_files)
    o_files = config.get_files_glob(f"{target}/src/","*.o")
    config.link_o_files(param, ld_flags, o_files, f"-o {out_file}")
    config.clean_files_glob(f"{target}/src/","*.o")
main()