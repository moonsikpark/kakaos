#!/usr/bin/env bash
BOOTLOADER="../bootloader/bootloader.bin"
PREKERNEL="../prekernel/prekernel.bin"
OUTPUT="output/disk.img"
PADDED_PREKERNEL="../prekernel/padded_prekernel.bin"
PREKERNEL_SIZE=$(du -b $PREKERNEL | awk '{print $1}')
SECTOR_NEEDED=$(($PREKERNEL_SIZE / 512 + 1))

# Pad prekernel
dd if=/dev/zero of=$PADDED_PREKERNEL bs=512 count=$SECTOR_NEEDED
dd if=$PREKERNEL of=$PADDED_PREKERNEL conv=notrunc

# Bootloader is already padded. Append bootloader and prekernel.
cat $BOOTLOADER $PADDED_PREKERNEL > $OUTPUT

# Write sector size of prekernel to bootloader.
printf "\x${SECTOR_NEEDED}\x0" | dd of=$OUTPUT bs=1 seek=5 count=2 conv=notrunc
