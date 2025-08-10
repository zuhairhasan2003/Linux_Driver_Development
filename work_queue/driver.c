#include<linux/module.h>
#include<linux/init.h>
#include<linux/proc_fs.h>
#include<linux/interrupt.h>
#include<linux/workqueue.h>

#define IRQ_NO 11

static struct work_struct workqueue;

void my_queue_func(struct work_struct * work)
{
	printk("Driver - bottom half (workqueue) of interrupt executed\n");
}

static irqreturn_t my_irq_handler(int irq, void * dev_id)
{
	printk("Inside IRQ Handler\n");
	schedule_work(&workqueue);
	printk("Leaving IRQ Handler\n");
	return IRQ_HANDLED;
}

ssize_t	my_read(struct file * filep, char __user *buffer, size_t count, loff_t * ptr)
{
	printk("Driver - Read performed\n");
	my_irq_handler(IRQ_NO , NULL);
	return 0;
}

static struct proc_ops proc_ops =
{
	.proc_read = my_read
};

struct proc_dir_entry * proc_entry;
char proc_entry_name[] = "work_queue_driver";

int my_init(void)
{
	INIT_WORK(&workqueue, my_queue_func);
	request_irq(IRQ_NO, my_irq_handler, 0, "my_dev" , NULL);
	proc_entry = proc_create(proc_entry_name, 444, NULL, &proc_ops);
	printk("Driver - Init\n");
	printk("Driver - /proc/%s created\n", proc_entry_name);
	return 0;
}

void my_exit(void)
{
	proc_remove(proc_entry);
	free_irq(IRQ_NO, NULL);
	printk("Driver - /proc/%s removed\n", proc_entry_name);
	printk("Driver - Exit\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
