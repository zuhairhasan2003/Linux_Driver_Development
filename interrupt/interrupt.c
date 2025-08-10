#include<linux/module.h>
#include<linux/init.h>
#include<linux/proc_fs.h>
#include<linux/interrupt.h>
#include<asm/io.h> // for asm(), used for creating a software intrrupt

#define IRQ_NO 11

static irqreturn_t my_interrupt_service_routine(int irq, void * dev_id)
{
	printk("Inside ISR\n");
	return IRQ_HANDLED;
}

ssize_t	my_read(struct file * filep, char __user * buffer, size_t count, loff_t * ptr)
{
	printk("Interrupt - Read performed\n");
	my_interrupt_service_routine(IRQ_NO, NULL); // copying a intrrupt to arrive from a hardware device(which I dont have at the moment)
	return 0;
}

static struct proc_dir_entry *proc_entry;

static struct proc_ops proc_ops =
{
	.proc_read = my_read
};

char proc_entry_name[] = "interrupt_proc_driver";
int my_init(void)
{
	proc_entry = proc_create(proc_entry_name, 444, NULL, &proc_ops);

	// requesting an interrupt request
	request_irq(IRQ_NO, my_interrupt_service_routine , 0 , "my_device", NULL);

	printk("Interrupt - Init\n");
	printk("Interrupt - /proc entry with name %s created\n", proc_entry_name);
	return 0;
}

void my_exit(void)
{
	proc_remove(proc_entry);
	free_irq(IRQ_NO, NULL);
	printk("Interrupt - /proc entry with name %s removed\n", proc_entry_name);
	printk("Interrupt - Exit\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
