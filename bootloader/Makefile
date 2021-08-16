all: bootloader.bin

bootloader.bin: bootloader.S
	nasm -o $@ $^

clean:
	rm -f *.bin
	rm -f *.s
