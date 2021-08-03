all: build_bootloader

build_bootloader: bootloader.asm
	nasm -o bootloader.bin bootloader.asm

clean:
	rm -f bootloader.bin