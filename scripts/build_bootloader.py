#!/usr/bin/python3
import config
def main():
    target = "bootloader"
    param = config.get_params()
    if param["ARCH"] == "x86_64":
        out_file = f"{target}/bootx64.efi"
    if param["ARCH"] == "aarch64":
        out_file = f"{target}/bootaa64.efi"
    inc_flags = f"-I{target}/include -Iefi"
    c_files = config.get_files_glob(f"{target}/src/","*.c")
    c_flags = f"-c {inc_flags} -target {param['ARCH']}-unknown-windows -ffreestanding -std=c11 -Wall -Werror -pedantic -fshort-wchar"
    ld_flags = "-flavor link -subsystem:efi_application -entry:efi_main -out:{out_file}"
    config.clean_files_glob(f"{target}/src/","*.o")
    config.clean_files_glob(f"{target}/src/","*.efi")
    config.compile_c_files(param, c_flags, c_files)
    o_files = config.get_files_glob(f"{target}/src/","*.o")
    config.link_o_files(param, ld_flags, o_files, f"-out:{out_file}")
    config.clean_files_glob(f"{target}/src/","*.o")
main()
