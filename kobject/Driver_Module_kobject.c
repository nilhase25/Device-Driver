/*
 * filename: Driver_Module_kobject.c
 */

#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>

/* Below are attributes variables*/
static int attr_1; /* read and write attribute */
static int attr_2; /* read only attribute */

/*
 * This function are use for reading the attributes i.e. "attr_1" & "attr_2" 
 */
static ssize_t attr_show(struct kobject *kobj, struct kobj_attribute *attr,
		      char *buf)
{
	int var;

	if (strcmp(attr->attr.name, "attr_1") == 0)
		var = attr_1;
	else
		var = attr_2;
	return sprintf(buf, "%d\n", var);
}


/*
 * This function are use for writing the attributes i.e. "attr_1" & "attr_2" 
 */

static ssize_t attr_store(struct kobject *kobj, struct kobj_attribute *attr,
		       const char *buf, size_t count)
{
	int var, ret;

	ret = kstrtoint(buf, 10, &var);
	if (ret < 0)
		return ret;

	if (strcmp(attr->attr.name, "attr_1") == 0)
		attr_1 = var;
	else
		attr_2 = var;
	return count;
}

static struct kobj_attribute attr_1_attribute =
	__ATTR(attr_1, 0664, attr_show, attr_store);
static struct kobj_attribute attr_2_attribute =
	__ATTR(attr_2, 0444, attr_show, attr_store);


/*
 * Create a "attr_1" & "attr_2" attributes group so that we can create and destroy both
 * at once.
 */
static struct attribute *attrs[] = {
	&attr_1_attribute.attr,
	&attr_2_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};


static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *Driver_Module_kobj;

static int __init Driver_Module_init(void)
{
	int retval;

	/*
	 * Create a simple kobject with the name of "Driver_Module_kobj"
	 * and create directory "Driver_Module" located under /sys/kernel/
	 */
	
	Driver_Module_kobj = kobject_create_and_add("Driver_Module", kernel_kobj);
	if (!Driver_Module_kobj)
		return -ENOMEM;

	/* 
	 * Create the files associated with this kobject under directory /sys/kernel/Driver_Module 
	 */
	retval = sysfs_create_group(Driver_Module_kobj, &attr_group);
	if (retval)
		kobject_put(Driver_Module_kobj);

	printk(KERN_INFO "Driver Module Initialization Done!!!\n");

	return retval;
}

static void __exit Driver_Module_exit(void)
{
	kobject_put(Driver_Module_kobj);
	printk(KERN_INFO "Driver Module Removed!!!\n");
}

module_init(Driver_Module_init);
module_exit(Driver_Module_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Emedded Buddies");
MODULE_DESCRIPTION("kobject and sysfs example Driver Module");
MODULE_VERSION("1:0.1");
