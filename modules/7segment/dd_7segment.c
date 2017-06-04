#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/platform_data/edk9s5422_gpio.h>

#define DEVICE_NAME "7segment"

int seg_gpios[12];
int seg_major;
unsigned int ibuf[8];

#define SEG1   seg_gpios[8]
#define SEG2    seg_gpios[9]
#define SEG3    seg_gpios[10]
#define SEG4    seg_gpios[11]

static int dd_segment_open(struct inode * inode, struct file * file){
	int i, ret;

	edk9s5422_seg_gpio(seg_gpios);

	for(i=0 ; i<8 ; i++){
		ret = gpio_request(seg_gpios[i], "seg");
		if(ret < 0){
			printk(KERN_INFO "dd_seg_open() gpio_request err!\n");
			return -1;
		}
		else
			gpio_direction_output(seg_gpios[i], 0);
	}

	for(i=9 ; i<12 ; i++){
		ret = gpio_request(seg_gpios[i], "seg");
		if(ret < 0){
			printk(KERN_INFO "dd_seg_open() gpio_request err!\n");
			return -1;
		}
		else
			gpio_direction_output(seg_gpios[i], 1);
	}

	return 0;
}

static int dd_segment_release(struct inode * inode, struct file * file){
	int i;

	for(i=0 ; i<12 ; i++)
		gpio_free(seg_gpios[i]);

	return 0;
}

int Getsegmentcode_base(int x){
	unsigned int i;

	for(i=0 ; i<8 ; i++)
		ibuf[i] = 0;

	switch(x){
		case 0 :
			ibuf[0] = 1;
			ibuf[1] = 1;
			ibuf[2] = 1;
			ibuf[3] = 1;
			ibuf[4] = 1;
			ibuf[5] = 1;
			break;

		case 1 :
			ibuf[1] = 1;
			ibuf[2] = 1;
			break;

		case 2 :
			ibuf[0] = 1;
			ibuf[1] = 1;
			ibuf[3] = 1;
			ibuf[4] = 1;
			ibuf[6] = 1;   
			break;      

		case 3 :
			ibuf[0] = 1;
			ibuf[1] = 1;
			ibuf[2] = 1;
			ibuf[3] = 1;
			ibuf[6] = 1;         
			break;

		case 4 :
			ibuf[1] = 1;
			ibuf[2] = 1;
			ibuf[5] = 1;
			ibuf[6] = 1;
			break;

		case 5 :
			ibuf[0] = 1;
			ibuf[2] = 1;
			ibuf[3] = 1;
			ibuf[5] = 1;
			ibuf[6] = 1;
			break;

		case 6 :
			ibuf[0] = 1;
			ibuf[1] = 1;
			ibuf[3] = 1;
			ibuf[4] = 1;
			ibuf[5] = 1;
			ibuf[6] = 1;
			break;

		case 7 :
			ibuf[0] = 1;
			ibuf[1] = 1;
			ibuf[2] = 1;
			ibuf[5] = 1;
			break;

		case 8 :
			ibuf[0] = 1;
			ibuf[1] = 1;
			ibuf[2] = 1;
			ibuf[3] = 1;
			ibuf[4] = 1;
			ibuf[5] = 1;
			ibuf[6] = 1;
			break;

		case 9 :
			ibuf[0] = 1;
			ibuf[1] = 1;
			ibuf[2] = 1;
			ibuf[3] = 1;
			ibuf[5] = 1;
			ibuf[6] = 1;
			break;

		case 10 :
			ibuf[0] = 1;
			ibuf[1] = 1;
			ibuf[2] = 1;
			ibuf[4] = 1;
			ibuf[5] = 1;
			ibuf[6] = 1;
			break;

		case 11 :
			ibuf[0] = 1;
			ibuf[1] = 1;
			ibuf[2] = 1;
			ibuf[3] = 1;
			ibuf[4] = 1;
			ibuf[5] = 1;
			ibuf[6] = 1;
			break;

		case 12 :
			ibuf[0] = 1;
			ibuf[3] = 1;
			ibuf[4] = 1;
			ibuf[5] = 1;
			break;         

		case 13 :
			ibuf[0] = 1;
			ibuf[1] = 1;
			ibuf[2] = 1;
			ibuf[3] = 1;
			ibuf[4] = 1;
			ibuf[5] = 1;
			break;

		case 14 :
			ibuf[0] = 1;
			ibuf[3] = 1;
			ibuf[4] = 1;
			ibuf[5] = 1;
			ibuf[6] = 1;
			break;

		case 15 :
			ibuf[0] = 1;
			ibuf[4] = 1;
			ibuf[5] = 1;
			ibuf[6] = 1;
			break;

		default :
			for(i=0 ; i<8 ; i++) ibuf[i] = 1;
			break;
	}

	return 0;
}

static ssize_t dd_segment_write(struct file * file, const char * buf, size_t length, loff_t * ofs){
	unsigned int ret, i;
	unsigned char data[4];

	ret = copy_from_user(data, buf, 4);

	Getsegmentcode_base((unsigned int)data[0]);
	gpio_direction_output(SEG1, 0);
	for(i=0 ; i<8 ; i++)   gpio_direction_output(seg_gpios[i], (unsigned int)ibuf[i]);
	mdelay(5);

	Getsegmentcode_base((unsigned int)data[1]);
	gpio_direction_output(SEG1, 1);
	gpio_direction_output(SEG2, 0);
	for(i=0 ; i<8 ; i++)    gpio_direction_output(seg_gpios[i], (unsigned int)ibuf[i]);
	mdelay(5);

	Getsegmentcode_base((unsigned int)data[2]);
	gpio_direction_output(SEG2, 1);
	gpio_direction_output(SEG3, 0);
	for(i=0 ; i<8 ; i++)    gpio_direction_output(seg_gpios[i], (unsigned int)ibuf[i]);
	mdelay(5);

	Getsegmentcode_base((unsigned int)data[3]);
	gpio_direction_output(SEG3, 1);
	gpio_direction_output(SEG4, 0);
	for(i=0 ; i<8 ; i++)    gpio_direction_output(seg_gpios[i], (unsigned int)ibuf[i]);
	mdelay(5);

	gpio_direction_output(SEG4, 1);

	return length;
}

static struct file_operations dd_segment_fops = {
	.owner = THIS_MODULE,
	.open = dd_segment_open,
	.release = dd_segment_release,
	.write = dd_segment_write,
};

static int dd_segment_init(void){
	printk("dd_segment_init, ");
	seg_major = register_chrdev(0, DEVICE_NAME, &dd_segment_fops);
	printk("major number is %d\n", seg_major);

	return 0;
}

static void dd_segment_exit(void){
	printk("dd_segment_exit, \n");
	unregister_chrdev(seg_major, DEVICE_NAME);
}

module_init(dd_segment_init);
module_exit(dd_segment_exit);

MODULE_LICENSE("Dual BSD/GPL");
