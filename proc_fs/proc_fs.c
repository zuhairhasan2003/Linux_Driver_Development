#include<linux/module.h>
#include<linux/init.h>
#include<linux/proc_fs.h>

ssize_t	my_read(struct file * filep, char __user * user_buffer, size_t size, loff_t * offet)
{
	printk("Read was called !\n");
	return 0;
}

struct proc_ops proc_ops = {
	.proc_read = my_read
};

int my_init(void)
{
	proc_create("my_proc_fs", 444, NULL, &proc_ops);
	printk("Module init called !\n");
	return 0;
}

void my_exit(void)
{
	remove_proc_entry("my_proc_fs" , NULL);
	printk("Module exit called !\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);


