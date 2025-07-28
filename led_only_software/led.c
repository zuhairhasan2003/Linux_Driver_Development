#include<linux/init.h>
#include<linux/module.h>

MODULE_LICENSE("GPL");

int major = 0;
char led_state[] = "0\n";

static ssize_t my_read(struct file *filep, char *user_buffer, size_t size, loff_t *offset)
{
	if(*offset > sizeof(led_state))
		return 0;

	int to_read = (sizeof(led_state) - *offset) < size ? sizeof(led_state) - *offset : size;
	int not_read = copy_to_user(user_buffer, &led_state[*offset], to_read);

	int delta = to_read - not_read;

	*offset += delta;
	return delta;
}

static ssize_t my_write(struct file * filep, const char * user_buffer, size_t size, loff_t * offset)
{
	if(*offset > sizeof(led_state))
		return 0;

	int to_write = (sizeof(led_state) - *offset) < size ? sizeof(led_state) - *offset : size;
	int not_write = copy_from_user(&led_state[*offset] , user_buffer, to_write);

	int delta = to_write - not_write;

	*offset += delta;
	return delta;
}


static struct file_operations fops = 
{
	.read = my_read,
	.write = my_write
};

int my_init(void)
{
	major = register_chrdev(0, "led", &fops);
	printk("Driver registred with a major : %d\n" , major);

	return 0;
}


void my_exit(void)
{
	unregister_chrdev(major, "led");
	printk("Driver unregistred with a major : %d\n", major);
}


module_init(my_init);
module_exit(my_exit);
