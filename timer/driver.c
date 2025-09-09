#include<linux/init.h>
#include<linux/module.h>
#include<linux/timer.h>
#include<linux/jiffies.h>

static struct timer_list timer;
int count = 0;

void timer_callback(struct timer_list * data)
{
	printk("Driver - Timer callback - %d\n", count++);
	mod_timer(&timer , jiffies + msecs_to_jiffies(5000)); // modified timer to prompt again after 5 secs, will keep doing this until timer not stopped
}

static int __init my_init(void)
{
	printk("Driver - Init\n");

	timer_setup(&timer, timer_callback, 0); // initializing timer
	add_timer(&timer); // starting timer

	return 0;
}

static void __exit my_exit(void)
{
	del_timer(&timer);
	printk("Driver - Exit\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
