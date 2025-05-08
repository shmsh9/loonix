import os
import platform
import glob
from pathlib import Path

def get_params():
    try:
        CC = os.environ["CC"]
    except:
        CC = "clang"
    try:
        LD = os.environ["LD"]
    except:
        LD = "lld"
    ARCH_TABLE = {
        "x86_64" : "x86_64",
        "AMD64"  : "x86_64",
        "aarch64": "aarch64"
    }
    try:
        ARCH = os.environ["ARCH"]
    except:
        ARCH = platform.uname()[-2]
    return {
        "ARCH" : ARCH_TABLE[ARCH],
        "CC"   : CC,
        "LD"   : LD
    }

def get_c_files(path):
    pass
def get_files_glob(path, glob):
    return Path(path).rglob(glob)

def compile_c_files(params, c_flags, c_files):
    for file in c_files:
        print(f"[CC] {file}")
        r = os.system(f"{params['CC']} {file} {c_flags} -o {os.path.splitext(file)[0]}.o")
        if(r != 0):
            exit(r)
def compile_rs_files(params, rs_files, std):
    for file in rs_files:
        print(f"[CC] {file}")
        if std:
            r = os.system(f"rustc --extern kstd={std} {file} --target={params['ARCH']}-unknown-none --crate-type=staticlib -o {os.path.splitext(file)[0]}.a")
        else:
            n = f"lib{os.path.splitext(file)[0].split("/")[-1]}.rlib"
            p = "/".join(os.path.splitext(file)[0].split("/")[0:-1])
            n = f"{p}/{n}"
            r = os.system(f"rustc {file} --target={params['ARCH']}-unknown-none --crate-type=rlib -o {n}")
        if(r != 0):
            exit(r)

def compile_s_files(params, c_flags, s_files):
    for file in s_files:
        print(f"[CC] {file}")
        cfg = get_params()
        flags = " -fPIC"
        if cfg["ARCH"] == "x86_64":
            c_flags += " -masm=intel"
        if params["CC"] == "gcc":
            r = os.system(f"clang {c_flags} {file} -o {os.path.splitext(file)[0]}_s.o")
        else:
            r = os.system(f"{params['CC']} {c_flags} {file} -o {os.path.splitext(file)[0]}_s.o")
        if(r != 0):
            exit(r)

def clean_files_glob(path, glob):
    f = get_files_glob(path, glob)
    for file in f:
        os.remove(file)
def link_o_files(params, ld_flags, o_files, output):
    f = " ".join([str(x) for x in o_files])
    r = os.system(f"{params['LD']} {ld_flags} {f} {output}")
    if r != 0:
        exit(r)
