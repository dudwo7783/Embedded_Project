#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	int dev, i;
	char buf[8] = {0, };

	if(argc != 2){
		printf("ex : %s 01001010\n", argv[0]);

		return -1;
	}

	dev = open("/dev/led", O_WRONLY);

	for(i=0; i<8; i++)
		if(argv[1][i] == '1') buf[i] = 1;

	write(dev, buf, 8*sizeof(char));
	close(dev);

	return 0;
}
