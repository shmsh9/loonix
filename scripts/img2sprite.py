#!/usr/bin/python3
from PIL import Image
from sys import argv
import os
if len(argv) < 2:
    print(f"usage : {argv[0]} image")
    exit(-1)

with open(f"{argv[1]}.h", "w") as f:
    image = Image.open(argv[1])
    pixels = list(image.getdata())
    width,height = image.size
    #f.write(f"#define {(os.path.splitext(argv[1])[0]).upper()}_SPRITE_NEW() graphics_sprite_new({width},{height})\n")
    #f.write(f"#define {(os.path.splitext(argv[1])[0]).upper()}_SPRITE_INIT(sprite) {{\\\n")
    f.write(f"static uint8_t {(os.path.splitext(argv[1])[0]).upper()}_PIXELS[{(width*height)*4}] = {{\n")
    pxpos = 0
    for p in pixels:
        try:
            b, g, r, a = p
        except:
            b,g,r = p
            a = 0xff
        f.write(f"\t{r}, {g}, {b}, {a},\n")
            #f.write(f"sprite->pixels[{pxpos}] = (graphics_pixel){{ .Red = {r}, .Blue = {b}, .Green = {g}, .Alpha = {a} }};\\\n");
        pxpos += 1
    f.write("};\n")
