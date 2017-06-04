#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/platform_data/edk9s5422_gpio.h>
#include "textlcd_ioctl.h"

#define DEVICE_NAME "textlcd"

int lcd_gpios[11];
int textlcd_major;

void setcommand(unsigned char command);
void writebyte(unsigned char cData);
void initialize_textlcd(void);
int function_set(void);
int displaty_control(int display_enable);
int cursor_control(int cursor_enable);
int cursor_shift(int set_shift);
int entry_mode_set(void);
int return_home(void);
int clear_display(void);
int set_ddram_address(int pos);

void setcommand(unsigned char command){
	int i;

	gpio_direction_output(TEXTLCD_RS, 0);
	gpio_direction_output(TEXTLCD_EN, 0);

	udelay(100);
	for(i=0 ; i<8 ; i++){
		if(command & 0x01)   gpio_direction_output(lcd_gpios[i], 1);
		else         gpio_direction_output(lcd_gpios[i], 0);

		command >>= 1;
	}
	udelay(10);

	gpio_direction_output(TEXTLCD_EN, 1);   udelay(10);
	gpio_direction_output(TEXTLCD_EN, 0);   udelay(4);
}

void writebyte(unsigned char cData){
	int i;

	gpio_direction_output(TEXTLCD_RS, 1);
	gpio_direction_output(TEXTLCD_EN, 0);

	udelay(100);
	for(i=0 ; i<8 ; i++){
		if(cData & 0x01)   gpio_direction_output(lcd_gpios[i], 1);
		else         gpio_direction_output(lcd_gpios[i], 0);
		cData >>= 1;
	}
	udelay(10);

	gpio_direction_output(TEXTLCD_EN, 1);   udelay(10);
	gpio_direction_output(TEXTLCD_EN, 0);   udelay(4);
}

void initialize_textlcd(){
	setcommand(0x38);
	setcommand(0x38);
	setcommand(0x0c);
	setcommand(0x01);
	mdelay(2);
	setcommand(0x06);
}

int function_set(void){
	setcommand(0x38);
	return 1;
}

int display_control(int display_enable){
	if(display_enable == 0)   setcommand(0x0c);
	else          setcommand(0x08);

	return 1;
}

int cursor_control(int cursor_enable){
	if(cursor_enable == 0)   setcommand(0x0e);
	else         setcommand(0x0c);

	return 1;
}

int cursor_shift(int set_shift){
	if(set_shift == 0)   setcommand(0x14);
	else          setcommand(0x10);

	return 1;
}

int entry_mode_set(void){
	setcommand(0x06);   return 1;
}

int return_home(){
	setcommand(0x02);   return 1;
}

int clear_display(){
	setcommand(0x01);   return 1;
}

int set_ddram_address(int pos){
	if(pos == 0)   setcommand(0x80);
	else       setcommand(0xC0);

	return 1;
}

static int dd_textlcd_open(struct inode * inode, struct file * file){
	int ret, i;
	unsigned char pStr[] = "Hello!!!", pStr1[] = "Welcome!";

	edk9s5422_lcd_gpio(lcd_gpios);
	for(i=0 ; i<11 ; i++){
		ret = gpio_request(lcd_gpios[i], "lcd");
		if(ret<0){
			printk(KERN_INFO "dd_textlcd_oen() gpio_request err!\n");
			printk(KERN_INFO "num = %d \n", i);
			return -1;
		}
		else
			gpio_direction_output(lcd_gpios[i], 0);
	}

	initialize_textlcd();

	set_ddram_address(0);
	for(i=0 ; i<8 ; i++)   writebyte(pStr[i]);

	set_ddram_address(1);
	for(i=0 ; i<8 ; i++)   writebyte(pStr1[i]);

	return 0;
}

static int dd_textlcd_release(struct inode *inode ,struct file*file)
{
	int i;
	for(i=0;i<11;i++)
		gpio_free(lcd_gpios[i]);

	return 0;

}

static DEFINE_MUTEX(dd_textlcd_mutex);
static long dd_textlcd_ioctl(struct file * file, unsigned int cmd, unsigned long cData)
{
	switch(cmd){
		case TEXTLCD_FUNCTION_SET:
			function_set();
			break;
		case TEXTLCD_DISPLAY_ON:
			display_control(0);
			break;
		case TEXTLCD_DISPLAY_OFF:
			display_control(1);
			break;
		case TEXTLCD_DISPLAY_CURSOR_ON:
			cursor_control(0);
			break;
		case TEXTLCD_DISPLAY_CURSOR_OFF:
			cursor_control(1);
			break;
		case TEXTLCD_CURSOR_SHIFT_RIGHT:
			cursor_shift(0);
			break;
		case TEXTLCD_CURSOR_SHIFT_LEFT:
			cursor_shift(1);
			break;
		case TEXTLCD_ENTRY_MODE_SET:
			entry_mode_set();
			break;
		case TEXTLCD_RETURN_HOME:
			return_home();
			break;
		case TEXTLCD_CLEAR:
			clear_display();
			break;
		case TEXTLCD_DD_ADDRESS_1:
			set_ddram_address(0);
			break;
		case TEXTLCD_DD_ADDRESS_2:
			set_ddram_address(1);
			break;
		case TEXTLCD_WRITE_BYTE:
			writebyte(cData);
			break;
		default:
			printk("drive : no such command!\n");
			return -ENOTTY;

	}
	mutex_unlock(&dd_textlcd_mutex);
	return 0;

}

static struct file_operations dd_textlcd_fops = {
	.owner = THIS_MODULE,
	.open = dd_textlcd_open,
	.unlocked_ioctl = dd_textlcd_ioctl,
	.release = dd_textlcd_release,
};

static int dd_textlcd_init(void){
	printk("dd_textlcd_init, \n");
	textlcd_major = register_chrdev(0, DEVICE_NAME, &dd_textlcd_fops);
	printk("major number is %d\n", textlcd_major);


	return 0;
}

static void dd_textlcd_exit(void)
{
	printk("dd_textlcd_exit\n");
	unregister_chrdev(textlcd_major,DEVICE_NAME);
}

module_init(dd_textlcd_init);
module_exit(dd_textlcd_exit);

MODULE_LICENSE("Dual BSD/GPL");
