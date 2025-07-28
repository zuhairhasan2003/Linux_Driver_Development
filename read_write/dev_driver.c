#include<linux/fs.h>
#include<linux/module.h>
#include<linux/init.h>

static int major = 0;
static char text[64];

static ssize_t my_read(struct file *filep , char * user_buffer, size_t size, loff_t * offset)
{
	int not_read = 0;
	int to_read = (sizeof(text) - *offset) > size ? size : (sizeof(text) - *offset);
	int read = 0;

	if(*offset >= sizeof(text))
		return 0;
	else
	{
		not_read = copy_to_user(user_buffer , &text[*offset], to_read);
		read += to_read - not_read;
	}

	*offset += read;

	return read;
}


static ssize_t my_write(struct file * filep, const char * user_buffer, size_t size, loff_t * offset)
{
	int to_read = sizeof(text) - *offset > size ? size : sizeof(text) - *offset;
	int not_read = 0;
	int read = 0;

	if(*offset >= sizeof(text))
		return 0;
	else
	{
		not_read = copy_from_user(&text[*offset] , user_buffer , to_read);
		read = to_read - not_read;
	}

	*offset += read;
	return read;
}

static struct file_operations fops = {
	.read = my_read,
	.write = my_write
};


int my_init(void)
{
	major = register_chrdev(0, "hello_driver", &fops);
	printk("hello_driver registred with MAJOR : %d\n" , major);
	return 0;
}

void my_exit(void)
{
	unregister_chrdev(major, "hello_driver");
	printk("Bye from hello_driver!\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
