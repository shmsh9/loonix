import os
import platform
import glob

def get_params():
    CC = "clang"
    LD = "lld"
    try:
        ARCH = os.environ["ARCH"]
    except:
        ARCH = platform.uname()[-2]
    return {
        "ARCH" : ARCH,
        "CC"   : CC,
        "LD"   : LD
    }

def get_c_files(path):
    pass
def get_files_glob(path):
    return glob.glob(path, recursive=True)

def compile_c_files(params, c_flags, c_files):
    for file in c_files:
        print(f"{params['CC']} {file} {c_flags} -o {os.path.splitext(file)[0]}.o")
        r = os.system(f"{params['CC']} {file} {c_flags} -o {os.path.splitext(file)[0]}.o")
        if(r != 0):
            exit(r)

def clean_files_glob(path):
    f = get_files_glob(path)
    for file in f:
        os.remove(file)
def link_o_files(params, ld_flags, o_files, output):
    f = " ".join(o_files)
    r = os.system(f"{params['LD']} {ld_flags} {f} -out:{output}")