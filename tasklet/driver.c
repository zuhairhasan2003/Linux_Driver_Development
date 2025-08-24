#include<linux/init.h>
#include<linux/module.h>
#include<linux/proc_fs.h>
#include<linux/interrupt.h>
#include<linux/slab.h>

#define IRQ 11
irqreturn_t my_handler(int , void*); // forward declaration of irq handler

// tasklet ---------------------------------------------------------------------------------------
void my_tasklet_func(unsigned long data)
{
	printk("This is a tasklet function, this runs atomically and usually contains heavy tasks that can take alot of time of interrupt handler!\n");
}
struct tasklet_struct * tasklet = NULL;

// proc fs work ---------------------------------------------------------------------------------
struct proc_dir_entry * proc_entry;
ssize_t	my_read(struct file * filep, char __user *buffer, size_t count, loff_t * ptr)
{
	printk("Driver - read performed!\n");
	my_handler(IRQ, NULL);
	return 0;
}
struct proc_ops proc_ops = {
	.proc_read = my_read
};

// irq handler ----------------------------------------------------------------------------------
irqreturn_t my_handler(int irq, void* dev_id)
{
	printk("Driver - Inside irq handler!\n");
	tasklet_schedule(tasklet);
	return IRQ_HANDLED;
}

int my_init(void)
{
	proc_entry = proc_create("taskelt driver", 666, NULL, &proc_ops);
	request_irq(IRQ, my_handler, 0, "demo", NULL);

	tasklet =kmalloc(sizeof(struct tasklet_struct) , GFP_KERNEL);
	tasklet_init(tasklet, my_tasklet_func, 1);

	printk("Driver - hello world!\n");
	return 0;
}

void my_exit(void)
{
	printk("Driver - bye!\n");
	proc_remove(proc_entry);
	free_irq(IRQ, NULL);
	tasklet_kill(tasklet);
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
