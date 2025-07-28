#include <stdio.h>
#include <stdlib.h>
#include<sys/ioctl.h>
#include<fcntl.h>

#include "ioctl_commands.h"


int main()
{
        int fd;
        int32_t value = 0;
	int32_t input = 0;

        fd = open("/dev/ioctl_file", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }

	printf("Enter the value to write to file : ");
	scanf("%d", &input);
	ioctl(fd, WR_VALUE, (int32_t *)&input);

	ioctl(fd, RD_VALUE, (int32_t *) &value);
        printf("Value readed from ioctl dev driver = %d\n", value);

        close(fd);
}
