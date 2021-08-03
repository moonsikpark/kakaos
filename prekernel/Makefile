all: prekernel.bin

prekernel.bin: prekernel.asm
	nasm -o prekernel.bin prekernel.asm

clean:
	rm -f prekernel.asm