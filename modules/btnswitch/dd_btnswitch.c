#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/platform_data/edk9s5422_gpio.h>

#define IRQ_BUTTON_1	538
#define IRQ_BUTTON_2    539
#define IRQ_BUTTON_3    540

#define DEVICE_NAME "btnswitch"

static DECLARE_WAIT_QUEUE_HEAD(wait_queue);

int irq_num = 0;
int irqNum[3];
int int_gpios[3];

int btnswitch_major;

static irqreturn_t button_interrupt(int irq, void *dev_id)
{
	irq_num = irq;

	wake_up_interruptible(&wait_queue);

	return IRQ_HANDLED;
}

static int dd_interrupt_open(struct inode * inode, struct file * file)
{
	int res, i;

	edk9s5422_int_gpio(int_gpios);
	for(i=0;i<3;i++)
		irqNum[i]=gpio_to_irq(int_gpios[i]);

	res = request_irq(irqNum[0], button_interrupt,IRQF_TRIGGER_FALLING, "GPIO", NULL);
	if(res < 0)
		printk(KERN_ERR "%s : Request for IRQ %d failed\n", __FUNCTION__,irqNum[0]);

	res = request_irq(irqNum[1], button_interrupt,IRQF_TRIGGER_FALLING, "GPIO", NULL);
        if(res < 0)
                printk(KERN_ERR "%s : Request for IRQ %d failed\n", __FUNCTION__,irqNum[1]);

	res = request_irq(irqNum[2], button_interrupt,IRQF_TRIGGER_FALLING, "GPIO", NULL);
        if(res < 0)
                printk(KERN_ERR "%s : Request for IRQ %d failed\n", __FUNCTION__,irqNum[2]);

	return 0;
}

static int dd_interrupt_release(struct inode * inode, struct file * file)
{
	int i;

	for(i=0;i<3;i++)
		free_irq(irqNum[i], NULL);

	return 0;
}

static ssize_t dd_interrupt_read(struct file * file, char * buf, size_t length, loff_t * ofs)
{
	char btn = 0;
	interruptible_sleep_on(&wait_queue);

	if(irq_num == IRQ_BUTTON_1)		btn = 1;
	else if(irq_num == IRQ_BUTTON_2)	btn = 2;
	else if(irq_num == IRQ_BUTTON_3)        btn = 3;
	else					btn = 0;

	put_user(btn, buf);
	return 0;
}

static struct file_operations dd_interrupt_fops = {
	.owner = THIS_MODULE,
	.open = dd_interrupt_open,
	.release = dd_interrupt_release,
	.read = dd_interrupt_read,
};

static int dd_interrupt_init(void){
	printk("dd_interrupt_init, \n");
	
	btnswitch_major = register_chrdev(0, DEVICE_NAME, &dd_interrupt_fops);
	printk("dd_interrupt_init, major number is %d\n", btnswitch_major);

	return 0;
}

static void dd_interrupt_exit(void){
	printk("dd_interrupt_exit, \n");
	unregister_chrdev(btnswitch_major, DEVICE_NAME);
}
module_init(dd_interrupt_init);
module_exit(dd_interrupt_exit);

MODULE_LICENSE("Dual BSD/GPL");
