NASM = nasm
GCC = gcc -c -m32 -ffreestanding -std=c99
LD = ld -T linker_script/script.lds -nostdlib

OBJS =	main.o \
		common.o \
		framebuffer.o \
		paging.o \
		load_kernel.o \
		init.o

all: prekernel.bin

entrypoint.bin: entrypoint.S
	$(NASM) -o $@ $<

%.o: %.S
	$(NASM) -f elf32 -o $@ $<

%.o: %.c
	$(GCC) -o $@ $<

main.bin: $(OBJS)
	$(LD) -o $@ $^

prekernel.bin: entrypoint.bin main.bin
	cat $^ > $@

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.s
