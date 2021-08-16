#!/usr/bin/env python3
import os
import math
dir_path = os.path.abspath(os.path.join(os.path.realpath(os.path.dirname(__file__)), os.pardir))

output = "/build/output/disk.img"

jobs = [
    ["/bootloader/bootloader.bin", "/bootloader/bootloader_padded.bin"],
    ["/prekernel/prekernel.bin", "/prekernel/prekernel_padded.bin"],
    ["/kernel/kernel.bin", "/kernel/kernel_padded.bin"],
]

def pad_image(image_path, padded_image_path) -> int:
    image = open(dir_path + image_path, "rb")
    image.seek(0, os.SEEK_END)
    size = image.tell()
    padded_size = math.ceil(size / 512)
    image.seek(0)
    contents = image.read()

    padded_image = open(dir_path + padded_image_path, "wb")
    padded_image.write(contents)
    while padded_image.tell() < padded_size * 512:
        padded_image.write(b'\0')
    
    image.close()
    padded_image.close()

    return padded_size

def write_disk(images, size_all, size_prekernel):
    disk = open(dir_path + output, "wb")

    for image in images:
        f = open(dir_path + image, "rb")
        contents = f.read()
        disk.write(contents)
        f.close()
    
    disk.seek(5, os.SEEK_SET)
    disk.write(size_all.to_bytes(2, byteorder="little"))

    disk.seek(7, os.SEEK_SET)
    disk.write(size_prekernel.to_bytes(2, byteorder="little"))

    disk.close()

def main():
    for item in jobs:
        size = pad_image(*item)
        item.append(size)
    
    write_disk([i[1] for i in jobs], jobs[1][2] + jobs[2][2], jobs[1][2])

if __name__ == "__main__":
    main()
