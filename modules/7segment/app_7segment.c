#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char * argv[]){
	int dev;
	int i;
	int is_error = 0;
	char val_integer[4] = {0, };
	time_t time_prev;

	if(argc != 2 || strlen(argv[1]) != 4)   
		is_error = 1;

	if(!is_error){
		for(i=0 ; i<4 ; i++){
			if( !( (argv[1][i] >= '0' && argv[1][i] <= '9') || (argv[1][i] >= 'a' && argv[1][i] <= 'f') || (argv[1][i] >= 'A' && argv[1][i] <= 'F') )) {
				is_error = 1;
				break;
			}
		}
	}

	if(is_error){
		printf("incorrect input.\n");
		printf("example : %s 1234\n", argv[0]);
		return 1;
	}

	if( (dev = open("/dev/7segment", O_WRONLY)) < 0 ){
		printf("error : cannot open device.\n");
		return 1;
	}

	for(i=0 ; i<4 ; i++){
		if(argv[1][i] >= '0' && argv[1][i] <= '9')
			val_integer[i] = argv[1][i] - '0';
		if(argv[1][i] >= 'A' && argv[1][i] <= 'F')
			val_integer[i] = 10 + argv[1][i] - 'A';
		if(argv[1][i] >= 'a' && argv[1][i] <= 'f')
			val_integer[i] = 10 + argv[1][i] - 'a';

	}

	time(&time_prev);
	while(time(NULL) - time_prev < 5)
		write(dev, val_integer, sizeof(char) * 4);

	close(dev);
	return 0;
}
