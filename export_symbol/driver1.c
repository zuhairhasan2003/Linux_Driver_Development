#include<linux/init.h>
#include<linux/module.h>

int shared_var = 1;
void shared_func(void)
{
	printk("Value of shared variable : %d\n", ++shared_var);
}

EXPORT_SYMBOL(shared_var);
EXPORT_SYMBOL(shared_func);

static int __init my_init(void)
{
	printk("Driver 1 - Init\n");
	return 0;
}

static void __exit my_exit(void)
{
	printk("Driver 1 - Exit\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
