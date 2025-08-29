#include<linux/init.h>
#include<linux/module.h>
#include<linux/kthread.h>
#include<linux/mutex.h>
#include <linux/delay.h>
#include<linux/slab.h>

int shared = 0;

struct mutex lock;

struct task_struct *kthread1;
struct task_struct *kthread2;

// the expected output is that thread 2 will print fist and the value of shared would be 1 finally

int func(void * data)
{
    int value = *(int *) data;
    if (value == 1)
    {
        ssleep(1);
        mutex_lock(&lock);
        shared = 1;
        printk("Driver - Thread 1 Running\n");
        mutex_unlock(&lock);
    }
    else if (value == 2)
    {
        mutex_lock(&lock);
        shared = 2;
        printk("Driver - Thread 2 Running\n");
        mutex_unlock(&lock);
    }
    return 0;
}

int my_init(void)
{

    mutex_init(&lock);

    printk("Driver - Enter\n");

    int * arg1 = kmalloc(sizeof(int) , GFP_KERNEL);
    int * arg2 = kmalloc(sizeof(int) , GFP_KERNEL);

    *arg1 = 1;
    *arg2 = 2;

    kthread1 = kthread_create(func, (void*) arg1, "kthread1");
    kthread2 = kthread_create(func, (void*) arg2, "kthread2");

    wake_up_process(kthread1);
    wake_up_process(kthread2);

    return 0;
}


void my_exit(void)
{
    printk("Driver - Value of shared var after execution : %d\n", shared);
    printk("Driver - Exit\n");
}


MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
