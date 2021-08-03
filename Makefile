all: build_bootloader make_image

build_bootloader:
	@echo Building bootloader...

	make -C bootloader

make_image:
	@echo Building disk image...

	cp bootloader/bootloader.bin os.img

clean:
	make -C bootloader clean
	rm -f os.img
