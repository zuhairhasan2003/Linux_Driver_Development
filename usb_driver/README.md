How to use this driver:
	1) Remove "usb_storage" driver:
		sudo rmmod usb_storage
	2) Insert VendorId and DeviceId inplace of (0x05ac , 0x12a8)
	3) Insert module, after making:
		sudo insmod driver.ko
	4) Insert USB
