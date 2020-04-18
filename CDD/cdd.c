#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h>                       
#define  DEVICE_NAME "cdd"   
#define  CLASS_NAME  "cdd_Class"     
 
 
static int    majorNo;      
static struct class*  cdd_Class  = NULL; 
static struct device* cdd_Device = NULL;           
 
static int     cdd_open(struct inode *, struct file *);
static int     cdd_release(struct inode *, struct file *);
static ssize_t cdd_read(struct file *, char *, size_t, loff_t *);
static ssize_t cdd_write(struct file *, const char *, size_t, loff_t *);
 
static struct file_operations cdd_fops =
{
   	.open = cdd_open,
   	.read = cdd_read,
   	.write = cdd_write,
   	.release = cdd_release,
};
 

static int __init cdd_init(void)
{
   	printk(KERN_INFO "cdd: Initializing the cdd\n");

   	//allocating major number dynamically
   	majorNo = register_chrdev(0, DEVICE_NAME, &cdd_fops);

   	if (majorNo<0)
	{
      		printk(KERN_ALERT "cdd: failed to allocating major number\n");
      		return majorNo;
   	}
   	printk(KERN_INFO "cdd: Allocate the major number %d\n", majorNo);

	// creation of device class
   	cdd_Class = class_create(THIS_MODULE, CLASS_NAME);

   	if (IS_ERR(cdd_Class))
	{       
		printk(KERN_ALERT "Device class creation failed\n");         
      		unregister_chrdev(majorNo, DEVICE_NAME);
      		return PTR_ERR(cdd_Class);          
   	}
   	printk(KERN_INFO "cdd: device class created\n");
 
   	cdd_Device = device_create(cdd_Class, NULL, MKDEV(majorNo, 0), NULL, DEVICE_NAME);

   	if (IS_ERR(cdd_Device))
	{
		printk(KERN_ALERT "device creation failed\n");               
      		class_destroy(cdd_Class);           
      		unregister_chrdev(majorNo, DEVICE_NAME);
      		return PTR_ERR(cdd_Device);
   	}
	printk(KERN_INFO "cdd: device created\n");
 
   	printk(KERN_INFO "cdd: cdd initialization done\n"); 
   	return 0;
}
 

static void __exit cdd_exit(void)
{
	device_destroy(cdd_Class, MKDEV(majorNo, 0));     
   	class_unregister(cdd_Class);                          
   	class_destroy(cdd_Class);  
   	unregister_chrdev(majorNo, DEVICE_NAME);  
   	printk(KERN_INFO "cdd: cdd removed\n");

}

static int cdd_open(struct inode *inode, struct file *file)
{
   	//dump_stack();
   	printk(KERN_INFO "cdd: cdd has been opened\n");
   	return 0;
}
 
static ssize_t cdd_read(struct file *file, char *buffer, size_t len, loff_t *offset)
{
	//dump_stack();
	printk(KERN_INFO "cdd: cdd has been read\n");
      	return 0;

}
 
static ssize_t cdd_write(struct file *file, const char *buffer, size_t len, loff_t *offset)
{
   	//dump_stack();
   	printk(KERN_INFO "cdd: writen on cdd %ld\n", len);
   	return len;
}
 
static int cdd_release(struct inode *inode, struct file *file)
{
	//dump_stack();
   	printk(KERN_INFO "cdd: cdd has been closed\n");
   	return 0;
}
 

module_init(cdd_init);
module_exit(cdd_exit);

MODULE_LICENSE("GPL v2");           
MODULE_AUTHOR("Embedded Buddies");    
MODULE_DESCRIPTION("A simple Linux char device driver"); 
MODULE_VERSION("1:0.1");            
