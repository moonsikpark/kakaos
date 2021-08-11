all: bootloader.bin

bootloader.bin: bootloader.asm
	nasm -o $@ $^

clean:
	rm -f bootloader.bin
