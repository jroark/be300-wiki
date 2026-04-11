/*
 * drivers/char/serial_be300.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * stupid misc char driver for the serial on the Casio Cassiopeia BE-300
 * (c) 2002 John Roark <jroark@linux4.be>
 *
 * mknod /dev/serial c 10 16
 */

#include <linux/init.h>
#include <linux/config.h>
#include <linux/tty.h>
#include <linux/major.h>
#include <linux/console.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/serial.h>
#include <linux/serial_reg.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/delay.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include "serial_be300.h"
#define BE300_MINOR   16

static ssize_t be300_read(struct file *file, char *buffer,
		size_t count, loff_t *pos);
static ssize_t be300_write(struct file *file, const char *buffer, 
		size_t count, loff_t *ppos);
static int be300_open(struct inode *inode, struct file *flip);
static int be300_close(struct inode *inode, struct file *flip);

static int be300_users = 0;

struct file_operations be300_fops = {
	read:		be300_read,
	write:		be300_write,
	open:		be300_open,
	release:	be300_close,
};

static struct miscdevice be300_serial = {
	BE300_MINOR, "be300_serial",
	&be300_fops
};

static inline unsigned char serial_inb(int offset)
{
	return readb((unsigned long)BE300_UART_BASE + 
		(offset << BE300_REG_SHIFT));
}

static inline void serial_outb(int offset, unsigned char value)
{
	writeb(value, (unsigned long)BE300_UART_BASE +
		(offset << BE300_REG_SHIFT));
}

static inline void serial_putc(unsigned char ch)
{
	while(!(serial_inb(UART_LSR) & UART_LSR_THRE));
	serial_outb(UART_TX, ch);
}

static inline void serial_puts(const unsigned char *buf, int count)
{
	unsigned int i = 0;

	for(i = 0; i < count; i++)
		serial_putc(buf[i]);
}

static ssize_t be300_read(struct file *file, char *buffer,
		size_t count, loff_t *pos)
{
	int i = 0;
	for(i = 0; i < count; i++)
	{
		while(!(serial_inb(UART_LSR) & UART_LSR_DR));
		put_user(serial_inb(UART_TX), buffer + i);
	}
	return count;
}

static ssize_t be300_write(struct file *file, const char *buffer, 
		size_t count, loff_t *ppos)
{
	serial_puts(buffer, count);	
	return count;
}

static int be300_close(struct inode *inode, struct file *flip)
{
	if(--be300_users)
		return 0;
		
	MOD_DEC_USE_COUNT;
}

static int be300_open(struct inode *inode, struct file *flip)
{
/*	printk("*** open ***\n"); */
	if(be300_users++)
		return 0;
		
	MOD_INC_USE_COUNT;
	return 0;
}

void __init be300_serial_init(void)
{
	misc_register(&be300_serial);
	printk(KERN_INFO "BE-300 serial driver initialized\n");
	return 0;
}

