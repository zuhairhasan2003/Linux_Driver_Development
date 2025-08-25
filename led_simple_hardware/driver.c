// Just a driver that turns an led when inserted into kernel, and turns it off when removed from kernel

// For raspberry pi 5 :
//	GPIO 13 connected to positive end of LED

#include<linux/module.h>
#include<linux/init.h>
#include<linux/gpio.h>

// base gpio chip add = 571
#define led_gpio 584 // gpio 13

int my_init(void)
{
	if(!gpio_is_valid(led_gpio))
	{
		printk("Driver - Invalid gpio\n");
		return -1;
	}

	if(gpio_request(led_gpio, "BLUE LED"))
	{
		printk("Driver - Problem in gpio_req()\n");
		return -1;
	}

	gpio_direction_output(led_gpio, 1);

	printk("Driver - Gpio set to HIGH - 1\n");

	printk("Driver - Init\n");
	return 0;
}

void my_exit(void)
{
	gpio_set_value(led_gpio, 0);
	printk("Driver - Gpio Released\n");
	gpio_free(led_gpio);
	printk("Driver - Exit\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
