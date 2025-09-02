#include <fcntl.h>
#include <stdbool.h>
#include<stdio.h>
#include<signal.h>
#include <unistd.h>

bool recieved;

void signal_handler(int sig)
{
	printf("Button press signal recieved\n");
	recieved = true;
}

int main()
{
	int fd = open("/dev/buttonDriver", O_RDONLY);

	signal(44, signal_handler);

	recieved = false;

	while(!recieved){
		sleep(1);
	}

	close(fd);

	return 0;
}
