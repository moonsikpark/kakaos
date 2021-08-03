all: prekernel.bin

prekernel.bin: entrypoint.s
	nasm -o prekernel.bin $<

clean:
	rm -f prekernel.bin
