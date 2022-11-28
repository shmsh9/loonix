import config

c_files = [ f"'{x}'".replace("\\", "/") for x in config.get_files_glob(f"kernel/src/","*.c") ]
s_files = [f"'{x}'".replace("\\", "/") for x in config.get_files_glob("kernel/src/", "x86_64*.S")]
print(
    ",".join(c_files)+','+
    ",".join(s_files)+
    ",\n"
)