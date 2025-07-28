#include<linux/module.h>
#include<linux/init.h>
#include<linux/kthread.h>
#include<linux/slab.h>

struct task_struct * thread1;
int thread1_func(void * data)
{
	printk("Thread 1 awake and the value is = %d\n", *((int*)data));
	return 0;
}

struct task_struct * thread2;
int thread2_func(void * data)
{
	printk("Thread 2 awake!\n");
	return 0;
}

int my_init(void)
{
	printk("Module Init\n");

	// in this example (where have to pass values bw 2 threads, its imp to allocate mem dynamically) (Stack is not shared, but heap is!)
	int *value = kmalloc(sizeof(int), GFP_KERNEL);
	*value = 66;

	thread1 = kthread_create(thread1_func, (void*) value, "thread1_func");
	thread2 = kthread_create(thread2_func, NULL , "thread2_func");

	printk("Threads created, now waking up...\n");
	wake_up_process(thread1);
	wake_up_process(thread2);

	return 0;
}

void my_exit(void)
{
	printk("Module Exit\n");
}


MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
