#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>

static int __init HelloWorldInit(void){
	printk(KERN_INFO "Hello World Module Init\n");
	return 0;
}

static void __exit HelloWorldExit(void){
	printk(KERN_INFO "Hello World Module Exit\n");
}

module_init(HelloWorldInit);
module_exit(HelloWorldExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nilesh Hase nilhase25@gmail.com");
MODULE_DESCRIPTION("Hello World");
MODULE_VERSION("1.0.0");
