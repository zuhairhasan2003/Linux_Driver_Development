#include<linux/init.h>
#include<linux/module.h>
#include<linux/sysfs.h>
#include<linux/kobject.h>
#include<linux/uaccess.h>


char kernel_buffer[12] = "Hello World\n";

static ssize_t  my_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    printk("SysFs - show performed\n");
    return sprintf(buf, kernel_buffer); // returns number of bytes read
}

static ssize_t  my_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
    if(count > sizeof(kernel_buffer))
    {
	printk("SysFs - partial store\n");
	memcpy(kernel_buffer, buf, sizeof(kernel_buffer) - 1);
	kernel_buffer[sizeof(kernel_buffer) - 1] = '\n';
	return count;
    }
    printk("SysFs - store performed\n");
    sprintf(kernel_buffer, buf);
    return count; // returns number of bytes copied
}

struct kobject *kobj;
struct kobj_attribute my_attr = __ATTR(hello , 0660 , my_show , my_store);

int my_init(void)
{
    printk("SysFs - Init\n");
    kobj = kobject_create_and_add("my_sysfs" , NULL);
    sysfs_create_file(kobj, &my_attr.attr);
    return 0;
}

void my_exit(void)
{
    sysfs_remove_file(kobj, &my_attr.attr);
    kobject_put(kobj);
    printk("SysFs - Exit\n");
}


MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
