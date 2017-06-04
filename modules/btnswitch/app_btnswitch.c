#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int dev;

int main(int argc, char *argv[])
{
	char btn;

	dev = open("dev/btnswitch", O_RDWR);
	if(dev < 0)
	{
		printf("error: cannot open button switch device.\n");
		return 1;
	}

	printf("Please push the button!\n");
	read(dev, &btn, 1);

	if(btn == 0)
	{
		printf("error: incorrect data from button switch device. \n");
		close(dev);
		return 1;
	}

	printf("S%d Button pushed.\n", (int)btn);

	close(dev);
	return 0;
}
