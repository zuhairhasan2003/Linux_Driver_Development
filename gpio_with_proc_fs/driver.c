// When you perform read operation on /proc/MyDeviceDriver you can se the led to triger on and off
#include<linux/gpio.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/proc_fs.h>

// gpio ---------------------------------------------------------------------------------------------
#define BASE 571
int ledPinNum = BASE + 26; // attach led to gpio 26 of raspberry pi 5
int ledValue = 0;

// proc fs ------------------------------------------------------------------------------------------

struct proc_dir_entry * proc_entry;

ssize_t	my_read(struct file *filep, char __user * buffer, size_t count, loff_t * ptr)
{
	printk("Driver - Read\n");
	ledValue = (ledValue == 0 ? 1 : 0);
	gpio_set_value(ledPinNum , ledValue);
	return 0;
}

struct proc_ops proc_ops = {
	.proc_read = my_read
};

// --------------------------------------------------------------------------------------------------

int my_init(void)
{

	proc_entry = proc_create("MyDeviceDriver", 666, NULL, &proc_ops);

	if(!gpio_is_valid(ledPinNum))
	{
		printk("Driver - Gpio pin is invalid\n");
		return -1;
	}

	if(gpio_request(ledPinNum , "GpioLed"))
	{
		printk("Driver - Gpio request failed\n");
		return -1;
	}

	gpio_direction_output(ledPinNum , ledValue);

	printk("Driver - Module init\n");
	return 0;
}

void my_exit(void)
{
	gpio_free(ledPinNum);
	proc_remove(proc_entry);
	printk("Driver - Module exit\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
