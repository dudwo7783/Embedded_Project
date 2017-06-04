#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/platform_data/edk9s5422_gpio.h>

#define MAJOR_NUMBER 0
#define DEVICE_NAME "led"

int led_gpios[8];
int led_major;

static int dd_led_open(struct inode * inode, struct file * file)
{
	int ret,i ;

	edk9s5422_led_gpio(led_gpios);

	for(i=0; i<8; i++)
	{
		ret = gpio_request(led_gpios[i], "led");
		if(ret < 0)
		{
			printk(KERN_INFO "dd_led_open(), gpio_request err! (number : %d)\n", i);
			return -1;
		}
		else
			gpio_direction_output(led_gpios[i], 0);
	}
	return 0;
}

static int dd_led_release(struct inode * inode, struct file * file)
{
	int i;

	for(i=0; i<8; i++){
		gpio_free(led_gpios[i]);
		printk(KERN_INFO "dd_led_close(), (number : %d)\n", i);
	}

	return 0;
}

static ssize_t dd_led_write(struct file * file, const char * buf, size_t length, loff_t * ofs)
{
	int ret, i;
	unsigned char cbuf[8];

	ret = copy_from_user(cbuf,buf,length);

	for(i=0; i<8; i++)
		gpio_direction_output(led_gpios[i], (unsigned int)cbuf[i]);

	return length;
}

static struct file_operations dd_led_fops = {
	.owner = THIS_MODULE,
	.open = dd_led_open,
	.release = dd_led_release,
	.write = dd_led_write,
};

static int dd_led_init(void){
	led_major = register_chrdev(0, DEVICE_NAME, &dd_led_fops);
	printk("dd_led_init, major number is %d\n", led_major);

	return 0;
}

static void dd_led_exit(void){
	printk("dd_led_exit, \n");
	unregister_chrdev(led_major, DEVICE_NAME);
}

module_init(dd_led_init);
module_exit(dd_led_exit);

MODULE_LICENSE("Dual BSD/GPL");
