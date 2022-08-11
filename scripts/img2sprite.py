#!/usr/bin/python3
from PIL import Image
from sys import argv
import os

if len(argv) < 2:
    print(f"usage : {argv[0]} image")
    exit(-1)

with open(f"{argv[1]}.h", "w") as f:
    f.write("#include <graphics.h>\n")
    image = Image.open(argv[1])
    pixels = list(image.getdata())
    width, height = image.size
    f.write(f"graphics_pixel {os.path.splitext(argv[1])[0]}_px[] = {{\n")
    for i in range(1, width):
        for j in range(1, height):
            r, g, b, a = image.getpixel(xy=(i, j))
            f.write("\t(graphics_pixel){ .Red = "+str(r)+", .Green = "+str(g)+", .Blue = "+str(b)+", .Alpha ="+str(a)+"},\n")
    f.write("};\n")
    f.write(f"graphics_sprite {os.path.splitext(argv[1])[0]}_sprite = {{ .width = {width}, .height = {height}, .pixels = {os.path.splitext(argv[1])[0]}_px }};\n")
    f.write(f"#define {(os.path.splitext(argv[1])[0]).upper()}_SPRITE_INIT() for(uint64_t i = 0; i < {os.path.splitext(argv[1])[0]}_sprite.width*{os.path.splitext(argv[1])[0]}_sprite.height; i++)");
    f.write(f"{{ {os.path.splitext(argv[1])[0]}_sprite.pixels[i] = {os.path.splitext(argv[1])[0]}_px[i]; }}\n")