NASM = nasm
GCC = gcc -c -m32 -ffreestanding -std=c99
LD = ld -T linker_script/script.lds -nostdlib

all: prekernel.bin

entrypoint.bin: entrypoint.s
	$(NASM) -o $@ $<

main.o: main.c
	$(GCC) -o $@ $<

main.bin: main.o
	$(LD) -o $@ $^

prekernel.bin: entrypoint.bin main.bin
	cat $^ > $@
		
clean:
	rm -f *.bin
	rm -f *.o
