#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Ordering_System.h"


/*
 * Class:     com_example_ordering_system_MainActivity
 * Method:    Print7Segment
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_example_ordering_1system_MainActivity_Print7Segment
(JNIEnv * env, jobject thiz, jint status, jstring table_number)
{
	int dev;
	int i, test;
	int is_error = 0;
	int size=0;
	char val_integer[4] = {0, };
	char val_temp = 0;
	char val_zero[4] = {0, };
	time_t time_prev;

	char tn[32];
	const char *temp=(*env)->GetStringUTFChars(env, table_number,0);

	memset(tn,'\0', 32);

	sprintf(tn, "%s", temp);

	size = strlen(tn);

	if(size != 1 && size != 2){   
		is_error = 1;
	}

	if(!is_error){
		for(i=0 ; i<size ; i++){
			if( !( (temp[i] >= '0' && temp[i] <= '9') || (temp[i] >= 'a' && temp[i] <= 'f') || (temp[i] >= 'A' && temp[i] <= 'F') )) {
				is_error = 1;
				break;
			}
		}
	}

	if(is_error){
		return -1;
	}

	if( (dev = open("/dev/7segment", O_WRONLY)) < 0 ){
		printf("error : cannot open device.\n");
		return -2;
	}

	for(i=0 ; i<size ; i++){
		if(temp[i] >= '0' && temp[i] <= '9')
			val_integer[3-i] = temp[i] - '0';
		if(temp[i] >= 'A' && temp[i] <= 'F')
			val_integer[3-i] = 10 + temp[i] - 'A';
		if(temp[i] >= 'a' && temp[i] <= 'f')
			val_integer[3-i] = 10 + temp[i] - 'a';
	}

	if(size==2){
		val_temp = val_integer[2];
		val_integer[2] = val_integer[3];
		val_integer[3] = val_temp;
	}

	time(&time_prev);

	switch(status){
		case 0:
			while(time(NULL) - time_prev < 4){
				write(dev, val_integer, sizeof(char) * size);
			}
			break;

		case 1:
			while(time(NULL) - time_prev < 4){
				write(dev, val_integer, sizeof(char) * size);
			}
			break;

		case 2:
			while(time(NULL) - time_prev < 2){
				write(dev, val_integer, sizeof(char) * size);
			}
			break;

		case 3:
			while(time(NULL) - time_prev < 2){
				write(dev, val_integer, sizeof(char) * size);
			}
			break;
		defalt:
			break;
	}

	close(dev);
	return 0;
}

/*
 * Class:     com_example_ordering_system_MainActivity
 * Method:    PrintLed
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_example_ordering_1system_MainActivity_PrintLed
  (JNIEnv * env, jobject thiz, jint status)
{
	int dev;
	int i, option;
	int test = 0;
	char buf[8] = {0, };

	option = status;

	if((dev = open("/dev/led", O_WRONLY)) < 0)
		return -1;

	if(status == 0){
		while(test<8){
			for(i=0;i<8;i++)
				buf[i] = 0;

			write(dev, buf, 8*sizeof(char));
			usleep(250000);

			for(i=0;i<8;i++)
				buf[i] = '1';

			write(dev, buf, 8*sizeof(char));
			usleep(250000);
			test++;
		}
	}
	else if(status == 1){
		while(test<8){
			for(i=0;i<8;i++){
				if(i%2==0)
					buf[i] = 0;
				else
					buf[i] = '1';
			}

			write(dev, buf, 8*sizeof(char));
			usleep(150000);

			for(i=0;i<8;i++){
				if(i%2==0)
					buf[i] = '1';
				else
					buf[i] = 0;
			}


			write(dev, buf, 8*sizeof(char));
			usleep(150000);
			test++;
		}
	}
	else if(status == 2){
		while(test<8){
			for(i=0;i<8;i++){
				if(i%3==0)
					buf[i] = '1';
				else
					buf[i] = 0;
			}

			write(dev, buf, 8*sizeof(char));
			usleep(250000);

			for(i=0;i<8;i++){
				if(i%3==0)
					buf[i] = 0;
				else
					buf[i] = '1';
			}

			write(dev, buf, 8*sizeof(char));
			usleep(250000);
			test++;
		}
	}
	else if(status == 3){
		while(test<8){
			for(i=0;i<8;i++){
				if(i%4==0)
					buf[i] = '1';
				else
					buf[i] = 0;
			}   

			write(dev, buf, 8*sizeof(char));
			usleep(150000);

			for(i=0;i<8;i++){
				if(i%4==0)
					buf[i] = 0;
				else
					buf[i] = '1';
			}

			write(dev, buf, 8*sizeof(char));
			usleep(150000);
			test++;
		}
	}
	else{
		memset(buf,'\0', sizeof(buf));
	}

	for(i=0;i<8;i++)
		buf[i] = 0;

	write(dev, buf, 8*sizeof(char));
	close(dev);

	return 0;

}

/*
 * Class:     com_example_ordering_system_MainActivity
 * Method:    GetRes
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_ordering_1system_MainActivity_GetRes
  (JNIEnv * env, jobject thiz)
{

	int dev, adc_data;
	int max = 0;
	time_t inittime;
	FILE *f_in;

	f_in = fopen("/sys/devices/12d10000.adc/iio:device0/in_voltage0_raw", "r");
	fscanf(f_in, "%d", &adc_data);
	fclose(f_in);

	if(adc_data<1000)
		max = 5;
	else if(adc_data >= 1000 && adc_data<2000)
		max = 10;
	else if(adc_data >= 2000 && adc_data<3000)
		max = 15;
	else if(adc_data >= 3000 && adc_data<4000)
		max = 20;
	else
		max = 25;

	return max;
}

/*
 * Class:     com_example_ordering_system_MainActivity
 * Method:    GetTouch
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_ordering_1system_MainActivity_GetTouch
  (JNIEnv * env, jobject thiz)
{
	char btn;
	int dev;

	dev = open("/dev/btnswitch", O_RDWR);

	if(dev<0)
		return -1;

	read(dev, &btn, 1);

	close(dev);

	return btn;
}
