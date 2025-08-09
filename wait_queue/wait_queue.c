#include<linux/module.h>
#include<linux/init.h>
#include<linux/kthread.h>
#include<linux/slab.h>
#include<linux/wait.h>
#include <linux/delay.h>

struct task_struct *thread1;
struct task_struct *thread2;

wait_queue_head_t wq;

int lock ;

int thread1Func(void *)
{
    printk("Thread 1, Locked\n");
    wait_event(wq, lock == 1);
    printk("Thread 1, Un locked\n");
    return 0;
}

int thread2Func(void *)
{
    printk("Thread 2 Sleep\n");
    ssleep(5);
    printk("Thread 2 Awake\n");
    lock = 1;
    wake_up(&wq); // unlocked the lock and now thread1 can continue
    return 0;
}

int my_init(void)
{
    printk("Module Init\n");

    lock = 0 ; // its locked!
    init_waitqueue_head(&wq);

    thread1 = kthread_create(thread1Func, NULL, "thread1");
    thread2 = kthread_create(thread2Func, NULL, "thread2");

    wake_up_process(thread1);
    wake_up_process(thread2);

    return 0;
}

void my_exit(void)
{
    printk("Module Exit\n");
}


module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
