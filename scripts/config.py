import os
import platform
import glob
from pathlib import Path

def get_params():
    CC = "clang"
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
def compile_s_files(params, c_flags, s_files):
    for file in s_files:
        print(f"[CC] {file}")
        r = os.system(f"{params['CC']} -masm=intel {file} {c_flags} -o {os.path.splitext(file)[0]}.o")
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