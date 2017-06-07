#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/platform_data/edk9s5422_gpio.h>

#define DEVICE_NAME "touch"

int touch_gpios[2];
int touch_major;

static int dd_touch_open(struct inode *inode, struct file *file)
{
   int i,ret;
   
   edk9s5422_cap_gpio(touch_gpios);
   for(i=0;i<2;i++)
   {
      ret = gpio_request(touch_gpios[i],"capsw");
      if(ret<0)
      {
         printk(KERN_INFO "dd_touch_open() %d gpio_request err!\n",i);
         return -1;
      }
   }
   return 0;
}

static int dd_touch_release(struct inode *inode, struct file *file)
{
   int i;
   for(i=0;i<2;i++)
      gpio_free(touch_gpios[i]);

   return 0;
}

static ssize_t dd_touch_read(struct file *file, char *buf, size_t length, loff_t *ofs)
{
   char touched[2]={0,};
   int ret;

   if(gpio_get_value(touch_gpios[0]) == 0x00)
      touched[0] = 1;

   if(gpio_get_value(touch_gpios[1]) == 0x00)
      touched[1] = 1;

   ret = copy_to_user(buf, touched, sizeof(char) *2);

   if(ret<0)
      return -1;

   return length;
}

static struct file_operations dd_touch_fops = {
   .owner = THIS_MODULE,
   .open = dd_touch_open,
   .release = dd_touch_release,
   .read = dd_touch_read,
};

static int dd_touch_init(void){
   printk("dd_touch_init, ");
   touch_major = register_chrdev(0,DEVICE_NAME, &dd_touch_fops);
   printk("major number is %d\n", touch_major);

   return 0;
}

static void dd_touch_exit(void){
   printk("dd_touch_exit, \n");
   unregister_chrdev(touch_major,DEVICE_NAME);
}

module_init(dd_touch_init);
module_exit(dd_touch_exit);

MODULE_LICENSE("Dual BSD/GPL");
