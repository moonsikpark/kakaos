NASM = nasm
GCC = gcc -c -m32 -ffreestanding -std=c99
LD = ld -T linker_script/script.lds -nostdlib

OBJS =	main.o \
		common.o \
		framebuffer.o

all: prekernel.bin

entrypoint.bin: entrypoint.s
	$(NASM) -o $@ $<

%.o: %.c
	$(GCC) -o $@ $<

main.bin: $(OBJS)
	$(LD) -o $@ $^

prekernel.bin: entrypoint.bin main.bin
	cat $^ > $@

clean:
	rm -f *.bin
	rm -f *.o
