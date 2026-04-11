#!/bin/sh

mknod console c 5 1
mknod ttyS0 c 4 64
mknod tty c 5 0
mknod ram0 b 1 0
mknod ram1 b 1 1
mknod mem c 1 1
mknod kmem c 1 2
mknod null c 1 3
mknod zero c 1 5
mknod fb0 c 29 0
mknod ptmx c 5 2

SUFFIXES="0 1 2 3 4 5 6 7 8 9 a b c d e f"
MAJOR=176
for i in $SUFFIXES; do
	mknod ptya$i c 2 $MAJOR
	MAJOR=`expr $MAJOR + 1`
done

# reset MAJOR
MAJOR=176
for i in $SUFFIXES; do
	mknod ttya$i c 3 $MAJOR
	MAJOR=`expr $MAJOR + 1`
done

# reset and make tty devices
SUFFIXES="0 1 2 3 4 5 6 7 8 9"
MAJOR=0
for i in $SUFFIXES; do
	mknod tty$i c 4 $MAJOR
	MAJOR=`expr $MAJOR + 1`
done

ln -s fb0 fb
ln -s ram1 ram
ln -s ram0 ramdisk
ln -s ../proc/kcore kcore
ln -s ../proc/self/fd/0 stdin
ln -s ../proc/self/fd/1 stdout
ln -s ../proc/self/fd/2 stderr
