NASM = nasm -f elf64
GCC = gcc -c -m64 -ffreestanding -std=c99
LD = ld -melf_x86_64 -T linker_script/script.lds -nostdlib

OBJS =	main.o \
		framebuffer.o \
		common.o \
		keyboard.o

all: kernel.bin

%.o: %.S
	$(NASM) -o $@ $<

%.o: %.c
	$(GCC) -o $@ $<

kernel.bin: entrypoint.o $(OBJS)
	$(LD) -o $@ $^

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.s
