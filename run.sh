#!/usr/bin/env bash

# Requires the EDKII working tree to be initialized [source edksetup.sh]

BUILD=DEBUG
#BUILD=RELEASE

OUT=Bin/EDK2/${BUILD}_GCC/X64/UefiLoader

LDR_OUT=${OUT}/BootMgr/BootMgr/${BUILD}/*.efi
EXT2_OUT=${OUT}/Drivers/Ext2/Ext2/${BUILD}/*.efi

build
rv=$?
if [ $rv -ne 0 ]; then
   exit $rv
fi

dd if=/dev/zero of=Bin/DISK.IMG bs=1M count=1024

(
   echo g
   echo n
   echo 1
   echo 2048
   echo +256M
   echo t
   echo 1

   echo n
   echo 2
   echo
   echo +256M
   echo t
   echo 2
   echo 20

   echo n
   echo 3
   echo
   echo
   echo t
   echo 3
   echo 20

   echo w
) | fdisk Bin/DISK.IMG

LD=$(sudo losetup -fP --show Bin/DISK.IMG)

sudo mkfs.fat -F 32 ${LD}p1
sudo mkfs.ext2 -L PART0 ${LD}p2
sudo mkfs.ext2 -L PART1 ${LD}p3

sudo rm -rf /mnt/uldr
sudo mkdir /mnt/uldr/efi -p
sudo mkdir /mnt/uldr/pt1 -p
sudo mkdir /mnt/uldr/pt2 -p

sudo mount ${LD}p1 /mnt/uldr/efi
sudo mount ${LD}p2 /mnt/uldr/pt1
sudo mount ${LD}p3 /mnt/uldr/pt2

sudo mkdir /mnt/uldr/efi/EFI/BOOT/DRIVERS -p

sudo cp $EXT2_OUT /mnt/uldr/efi/EFI/BOOT/DRIVERS
sudo cp $LDR_OUT /mnt/uldr/efi/EFI/BOOT

sudo umount /mnt/uldr/efi
sudo umount /mnt/uldr/pt1
sudo umount /mnt/uldr/pt2

sudo losetup -d $LD
sudo rm -rf /mnt/uldr

echo Done.

qemu-system-x86_64 -cpu qemu64 -bios Bin/OVMF.fd -net none -vga std \
   -drive file=Bin/DISK.IMG,if=ide,format=raw -rtc base=localtime \
   -serial stdio -nographic -monitor none
