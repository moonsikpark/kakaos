all: build_bootloader build_prekernel make_image

build_bootloader:
	@echo Building bootloader...

	make -C bootloader

build_prekernel:
	make -C prekernel

make_image: build_bootloader build_prekernel
	@echo Building disk image...

	cat bootloader/bootloader.bin prekernel/prekernel.bin > os.img

clean:
	make -C bootloader clean
	make -C prekernel clean
	rm -f os.img
