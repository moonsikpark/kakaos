all: bootloader/bootloader.bin prekernel/prekernel.bin image

bootloader/bootloader.bin:
	@echo Building bootloader...
	
	make -C bootloader

	@echo Bootloader build done.

prekernel/prekernel.bin:
	@echo Building prekernel...
	
	make -C prekernel

	@echo Prekernel build done.

image: bootloader/bootloader.bin prekernel/prekernel.bin
	@echo Building disk image...

	make -C build

	@echo Disk image build done.
	
clean:
	make -C bootloader clean
	make -C prekernel clean
	make -C build clean
