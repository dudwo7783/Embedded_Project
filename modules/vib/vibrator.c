#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/platform_data/edk9s5422_gpio.h>

#define MAJOR_NUMBER 0
#define DEVICE_NAME "vib"

int vib_gpios;
int vib_major;

static int dd_vib_open(struct inode *inode, struct file *file)
{
	int ret;

	printk("dd_vib_open, \n");


	edk9s5422_vib_gpio(&vib_gpios);

	ret = gpio_request(vib_gpios, "vib");

	if(ret < 0)
	{
		printk(KERN_INFO "dd_vib_open() gpio_request err!\n");
		return -1;
	}
	else
	{
		gpio_direction_output(vib_gpios, 0);
	}

	return 0;
}

static int dd_vib_release(struct inode *inode ,struct file*file)
{	
	printk("dd_vib_release, \n");
	gpio_free(vib_gpios);
	return 0;
}
static ssize_t dd_vib_write(struct file *file, const char * buf, size_t length, loff_t *ofs)
{
	int ret, temp_buf;

	printk("dd_vib_write, \n");

	ret = copy_from_user(&temp_buf, buf, length);

	if(temp_buf != 0){
		gpio_direction_output(vib_gpios, 1);
		mdelay(temp_buf);
	}

	gpio_direction_output(vib_gpios, 0);

	return 0;
}

static struct file_operations dd_vib_fops = {
	.owner = THIS_MODULE,
	.open = dd_vib_open,
	.release = dd_vib_release,
	.write = dd_vib_write,
};

static int dd_vib_init(void)
{
	vib_major = register_chrdev(0, DEVICE_NAME, &dd_vib_fops);
	printk("dd_vib_init, major number is %d\n", vib_major);
	
	return 0;
}

static void dd_vib_exit(void)
{
	printk("dd_vib_exit, \n");

	unregister_chrdev(vib_major, DEVICE_NAME);
}


module_init(dd_vib_init);
module_exit(dd_vib_exit);

MODULE_LICENSE("Dual BSD/GPL");
