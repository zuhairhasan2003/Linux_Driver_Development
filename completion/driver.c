#include<linux/init.h>
#include<linux/module.h>
#include<linux/completion.h>
#include<linux/kthread.h>
#include<linux/delay.h>

// completion ----------------------------------------------------------------------------------

struct completion comp;

// kthread ----------------------------------------------------------------------------------

struct task_struct * kthread1;
struct task_struct * kthread2;

int thread1_func(void * data)
{
	while(!kthread_should_stop())
	{
		printk("Thread 1 Running\n");
		ssleep(2);
	}
	complete(&comp);
	return 0;
}

int thread2_func(void * data)
{
	wait_for_completion(&comp); // always wait for thread 2 to finish first
	printk("Thread 2 Running\n");
	return 0;
}

static int __init my_init(void)
{
	// 2 threads created
	kthread1 = kthread_create(thread1_func, NULL, "kthread1");
	kthread2 = kthread_create(thread2_func, NULL, "kthread2");

	// completion init
	init_completion(&comp);

	// awoke thread 1 and 2
	wake_up_process(kthread2); // thread 2 should wait for thread 1 to complete, although it was awoke first
	wake_up_process(kthread1);


	printk("Driver - init\n");
	return 0;
}

static void __exit my_exit(void)
{
	kthread_stop(kthread1); // when the module is removed, thread1 is stopped and then thread2 gets awake
	printk("Driver - exit\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
