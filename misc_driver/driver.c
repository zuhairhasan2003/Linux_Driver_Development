#include<linux/init.h>
#include<linux/module.h>
#include<linux/miscdevice.h>
#include<linux/fs.h>

ssize_t my_read(struct file *filep, char __user *buffer, size_t size, loff_t *ptr)
{
	printk("Driver - Read Performed\n");
	return 0;
}

ssize_t my_write(struct file *filep, const char __user *buffer, size_t size, loff_t *ptr)
{
	printk("Driver - Write performed\n");
	return size;
}

static struct file_operations fops = {
	.read = my_read,
	.write = my_write
};

struct miscdevice misc = {
	.minor = 1,
	.name = "my_misc_driver",
	.fops = &fops
};

static int __init my_init(void)
{
	if(misc_register(&misc) != 0)
	{
		printk("Driver - Cant register misc driver\n");
		return -1;
	}
	printk("Driver - Init\n");
	return 0;
}

static void __exit my_exit(void)
{
	misc_deregister(&misc);
	printk("Driver - Exit\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
