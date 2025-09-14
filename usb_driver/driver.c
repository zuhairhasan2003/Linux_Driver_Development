#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/usb.h>

int my_usb_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	printk("Driver - Usb Probe function called\n");

	struct usb_host_interface * cur_intf = intf->cur_altsetting;

	int numEndpoints = cur_intf->desc.bNumEndpoints;
	printk("Driver - Total endpoints : %d\n", numEndpoints);

	struct usb_host_endpoint * aval_endpoints = cur_intf->endpoint;

	for(int i = 0 ; i < numEndpoints ; i++)
	{
		printk("Driver - Endpoint %d , Length : %d\n", i+1, aval_endpoints->desc.bLength);
	}

	return 0;
}

void my_usb_disconnect(struct usb_interface *intf)
{
	printk("Driver - Usb Disconnect called\n");
}

struct usb_device_id my_usb_table[] = {
	{USB_DEVICE(0x05ac , 0x12a8)},
	{}
};
MODULE_DEVICE_TABLE(usb, my_usb_table);

struct usb_driver usb_driver = {
	.name = "my_usb_driver",
	.probe = my_usb_probe,
	.disconnect = my_usb_disconnect,
	.id_table = my_usb_table
};

static int __init my_init(void)
{
	int ret = usb_register(&usb_driver);
	if(ret)
	{
		printk("Driver - Unable to register USB Driver\n");
		return -1;
	}

	printk("Driver - Init\n");
	return 0;
}

static void __exit my_exit(void)
{
	usb_deregister(&usb_driver);
	printk("Driver - Exit\n");
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
