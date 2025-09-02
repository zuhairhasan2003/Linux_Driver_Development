#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/gpio.h>
#include<linux/interrupt.h>
#include <linux/sched/signal.h>
#include<linux/slab.h>

int major;
int buttonGpio;

int irq;

static struct task_struct * task = NULL;
int sigNo = 44;

// fs -----------------------------------------------------------------------------------------

int my_open(struct inode *, struct file *)
{
	printk("Driver - Open called\n");

	task = get_current();
	printk("Driver - User app registered with pid : %d\n" , task->pid);

	return 0;
}

static struct file_operations fops = {
	.open = my_open
};

// interrupt -----------------------------------------------------------------------------------

static irqreturn_t button_irq(int irq, void *dev_id)
{
	printk("Button was presses\n");

	if(task != NULL)
	{
		struct siginfo info;
		memset(&info , 0 , sizeof(info));
		info.si_signo = sigNo;

		send_sig_info(sigNo, (struct kernel_siginfo *) &info, task);
		printk("Driver - Signal Sent\n");
	}

	return IRQ_HANDLED;
}

// ---------------------------------------------------------------------------------------------

static int __init my_init(void)
{
	major = register_chrdev(0, "signal_driver", &fops);

	buttonGpio = 26 + 571;

	if(!gpio_is_valid(buttonGpio))
	{
		printk("Driver - gpio_is_valid() failed\n");
		return -1;
	}

	gpio_request(buttonGpio, "button gpio");
	gpio_direction_input(buttonGpio);

	irq = gpio_to_irq(buttonGpio);

	request_irq(irq, button_irq, IRQF_TRIGGER_RISING, "button irq", NULL);

	printk("Driver - Hello world, Major num : %d\n", major);

	return 0;
}

static void __exit my_exit(void)
{
	gpio_free(buttonGpio);
	unregister_chrdev(major, "signal_driver");
	free_irq(irq, NULL);
	printk("Driver - Bye\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
