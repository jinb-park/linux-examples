#!/bin/sh

# prerequisite
# we need the latest version of mkfs.ext4, how can we find and install it?
# trying to build it from https://github.com/tytso/e2fsprogs.
# put the prebuilt file derived from it into here, and use that instead of system's.

# clean-up first
./clean.sh

# ISSUE:
#   losetup can't support verity-enabled ext4 image.
#   I don't know whether or not it's because of the lack of supporting loopback in fs-verity.
#   it might be because of losetup utility itself.
#   anyway, continue to do with real partition. (quite a difficult than loopback..)

# With real partition via USB:
#    1. Connecting an USB, and make sure that umount all associated with the USB device.
#    2. sudo ./mkfs.ext4 -O verity /dev/sda (USB device)
#    ---- 1 & 2 are needed only once by your hand ---- 
#    3. mkdir ./tmp
#    4. sudo mount /dev/sda ./tmp
#    5. sudo chown -R jinbum:jinbum ./tmp
#    6. echo "hello" > ./tmp/hello.txt
#
#    Then, all things properly are ready to run ex1!

mkdir ./tmp
sudo mount /dev/sda ./tmp
sudo chown -R jinbum:jinbum ./tmp

gcc -o ex1 ex1.c
cp -f ex1 ./tmp/
