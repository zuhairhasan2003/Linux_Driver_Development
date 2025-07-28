#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include "ioctl_commands.h"

MODULE_LICENSE("GPL");

int major = 0;

int32_t value = 6;

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
		case RD_VALUE:
			if(copy_to_user((int32_t *) arg , &value, sizeof(value)))
				printk("Can not read complete data\n");
			else
				printk("Read performed sucessfully!\n");
			break;
		case WR_VALUE:
			if(copy_from_user(&value, (int32_t *) arg, sizeof(value)))
				printk("Can not write complete data\n");
			else
				printk("Write performed sucessfully\n");
			break;
		default:
			printk("Invalid args!\n");
	}
	return 0;
}

int my_open(struct inode *, struct file *)
{
	printk("my_open was called\n");
	return 0;
}

int my_release(struct inode *, struct file *)
{
	printk("my_release was called\n");
	return 0;
}

struct file_operations fops = {
	.unlocked_ioctl = my_ioctl,
	.open = my_open,
	.release = my_release
};

int my_init(void)
{
	major = register_chrdev(0, "ioctl_driver", &fops);
	printk("Hello from ioctl module, MAJOR : %d\n", major);
	return 0;
}

void my_exit(void)
{
	unregister_chrdev(major, "ioctl_driver");
	printk("Bye from ioctl module\n");
}

module_init(my_init);
module_exit(my_exit);
