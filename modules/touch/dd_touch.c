#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/platform_data/edk9s5422_gpio.h>

#define DEVICE_NAME "touch"

int touch_gpios[2];
int touch_major;

static int dd_touch_open(struct inode * inode, struct file *file)
{
	int i,ret;

	edk9s5422_cap_gpio(touch_gpios);
	for(i=0; i<2; i++)
	{
		ret = gpio_request(touch_gpios[i], "capsw");
	}
}

