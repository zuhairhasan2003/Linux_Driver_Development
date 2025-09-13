#include<linux/init.h>
#include<linux/module.h>

extern int shared_var;
extern void shared_func(void);

static int __init my_init(void)
{
	printk("Driver 2 - Init\n");

	shared_func();

	return 0;
}

static void __exit my_exit(void)
{
	printk("Driver 2 - Exit\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
