#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	int dev, sec;

	dev = open("/dev/vib", O_WRONLY);

	printf("Input number 100 ~ 10000 : ");
	scanf("%d", &sec);

	write(dev, &sec, 4);
	
	close(dev);

	return 0;
}
