#!/usr/bin/python3
import config
import json
import os
def preprocess_headers(path):
    config.clean_files_glob(path,"*.h")
    for f in config.get_files_glob(path,"*.json"):
        m = json.load(open(f, "r"))
        h = open(f"{path}/{m['filename']}", "w")
        hn = m['filename'].replace('.','_')
        h.writelines([f"#ifndef _loonix_preprocessed_{hn}\n", f"#define _loonix_preprocessed_{hn}\n"])
        for x in m['macros']:
            if x["type"] == "string":
                h.writelines([x["content"], '\n'])
            if x["type"] == "eval":
                h.writelines("\n".join(eval(x["content"])))
        h.writelines("\n#endif\n")
        h.close()

def main():
    target = "kernel"
    out_file = f"{target}/kernel.elf"
    param = config.get_params()
    inc_flags = f"-I{target}/include -Iefi -Ibootloader/include"
    c_files = config.get_files_glob(f"{target}/src/","*.c")
    s_files = config.get_files_glob(f"{target}/src/", f"{param['ARCH']}*.S")
    rs_files = [ 
        x for x in config.get_files_glob(f"{target}/rs/","*.rs")
        if x.name[0:-3] == str(x).replace("\\","/").split("/")[-2]
    ]
    if param["CC"] == "clang":
        c_flags = f"-c {inc_flags} -fstack-protector-strong -fstack-protector-all -fPIC -nostdlib -ffreestanding -std=c2y \
                -Wno-unused-function -Wall -Werror -pedantic -Wno-unused-but-set-variable -Wno-gnu-auto-type\
                -Wno-strict-prototypes -Wno-gnu-statement-expression-from-macro-expansion -Wno-unused-variable \
                -Wno-microsoft-static-assert -Wno-microsoft-enum-value -Wno-gnu-designator\
                -target {param['ARCH']}-none-elf -g -O0 -ggdb -fshort-wchar"
    if param["CC"] == "gcc":
        c_flags = f"-c {inc_flags} -fstack-protector-strong -fstack-protector-all -fPIC -nostdlib -ffreestanding -std=c2y \
                -Wno-unused-function -Wall -Wno-unused-but-set-variable -Wno-unused-variable \
                -Wno-strict-prototypes -g -O2 -Wno-strict-aliasing -fshort-wchar"
    if param["LD"] == "lld":
        ld_flags = f" -flavor ld -T {target}/src/link{param['ARCH']}.ld -allow-multiple-definition {' '.join(['-L./'+str(x)[0:-(len(x.name))] for x in rs_files])} {' '.join(['-l:'+x.name.replace('.rs','.a') for x in rs_files])}"
    if param["LD"] == "ld":
        ld_flags = f" -T {target}/src/link{param['ARCH']}.ld -allow-multiple-definition {' '.join(['-L./'+str(x)[0:-(len(x.name))] for x in rs_files])} {' '.join(['-l:'+x.name.replace('.rs','.a') for x in rs_files])}"
    config.clean_files_glob(f"{target}/src/","*.o")
    config.clean_files_glob(f"{target}/src/","*.elf")
    preprocess_headers(f"{target}/include/preprocessed/")
    config.compile_c_files(param, c_flags, c_files)
    config.compile_s_files(param, c_flags, s_files)
    config.compile_rs_files(param, [f"{target}/rs/kstd/kstd.rs"], None)
    config.compile_rs_files(param, rs_files, f"{target}/rs/kstd/libkstd.rlib")
    o_files = config.get_files_glob(f"{target}/src/","*.o")
    config.link_o_files(param, ld_flags, o_files, f"-o {out_file}")
    config.clean_files_glob(f"{target}/src/","*.o")
    config.clean_files_glob(f"{target}/rs/","*.a")
    config.clean_files_glob(f"{target}/rs/","*.rlib")

main()
