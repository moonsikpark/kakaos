all: bootloader.bin

bootloader.bin: bootloader.S
	nasm -o $@ $^

clean:
	rm -f bootloader.bin
	rm -f *.s
