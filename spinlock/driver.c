#include<linux/module.h>
#include<linux/init.h>
#include<linux/kthread.h>
#include<linux/spinlock.h>
#include<linux/delay.h>

spinlock_t lock;

struct task_struct * thread1;
struct task_struct * thread2;

int thread1_func(void * data)
{
	printk("Driver - Entering Thread 1\n");
	msleep(500); // wait for 0.5 sec for thread2 to get the lock first

	spin_lock(&lock);
	printk("Driver - Thread 1 Lock acquired\n");
	spin_unlock(&lock);
	printk("Driver - Thread 1 Lock released\n");

	return 0;
}

int thread2_func(void * data)
{
	printk("Driver - Entering Thread 2\n");

	spin_lock(&lock);
	printk("Driver - Thread 2 Lock acquired\n");

	// counting till 1000 before releasing the spinlock
	for(int i = 0 ; i < 1000 ; i++)
	{
		printk("%d\n", i);
	}

	spin_unlock(&lock);
	printk("Driver - Thread 2 Lock released\n");

	return 0;
}

static int __init my_init(void)
{
	thread1 = kthread_create(thread1_func, NULL, "thread1");
	thread2 = kthread_create(thread2_func, NULL, "thread2");

	printk("Driver - Hello World\n");

	spin_lock_init(&lock);

	wake_up_process(thread1);
	wake_up_process(thread2);

	return 0;
}

static void __exit my_exit(void)
{
	printk("Driver - Bye\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
