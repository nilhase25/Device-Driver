/*
 * filename: Driver_Module_kset.c
 */


/*
 * This module shows how to create a kset in sysfs called
 * /sys/kernel/Driver_Module
 * Then tree kobjects are created and assigned to this kset, "dev_1", "dev_2",
 * and "dev_3".  In those kobjects, "attr_1" and "attr_2" attributes of the 
 * same name are also created and we can read Or write interger number on it
 */

#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>


/*
 * We had create custom object to register sysfs.
 */
struct Driver_Module_obj {
	struct kobject kobj;
	int attr_1;
	int attr_2;
};
#define to_Driver_Module_obj(x) container_of(x, struct Driver_Module_obj, kobj)

/* A custom attribute are define for "Driver_Mdule_obj". */
struct Driver_Module_attribute {
	struct attribute attr;
	ssize_t (*show)(struct Driver_Module_obj *Driver_Module, struct Driver_Module_attribute *attr, char *buf);
	ssize_t (*store)(struct Driver_Module_obj *Driver_Module, struct Driver_Module_attribute *attr, const char *buf, size_t count);
};
#define to_Driver_Module_attr(x) container_of(x, struct Driver_Module_attribute, attr)

/*
 * This function registered with sysfs and This function get called when read 
 * sysfs file associated with kobject we have registered. We need to transpose 
 * back from a "default" kobject to our custom struct Driver_Module_obj and 
 * then call the show function for that specific object.
 */
static ssize_t Driver_Module_attr_show(struct kobject *kobj,
			     struct attribute *attr,
			     char *buf)
{
	struct Driver_Module_attribute *attribute;
	struct Driver_Module_obj *Driver_Module;

	attribute = to_Driver_Module_attr(attr);
	Driver_Module = to_Driver_Module_obj(kobj);

	if (!attribute->show)
		return -EIO;

	return attribute->show(Driver_Module, attribute, buf);
}

/*
 * Same as above it called when write sysfs file associated with kobject
 */
static ssize_t Driver_Module_attr_store(struct kobject *kobj,
			      struct attribute *attr,
			      const char *buf, size_t len)
{
	struct Driver_Module_attribute *attribute;
	struct Driver_Module_obj *Driver_Module;

	attribute = to_Driver_Module_attr(attr);
	Driver_Module = to_Driver_Module_obj(kobj);

	if (!attribute->store)
		return -EIO;

	return attribute->store(Driver_Module, attribute, buf, len);
}

/* Custom sysfs_ops defined*/
static const struct sysfs_ops Driver_Module_sysfs_ops = {
	.show = Driver_Module_attr_show,
	.store = Driver_Module_attr_store,
};

/*
 * We free the memory held in our object here.
 */
static void Driver_Module_release(struct kobject *kobj)
{
	struct Driver_Module_obj *Driver_Module;

	Driver_Module = to_Driver_Module_obj(kobj);
	kfree(Driver_Module);
}



/*
 * This function called when attributes file read
 */
static ssize_t attr_show(struct Driver_Module_obj *Driver_Module_obj, struct Driver_Module_attribute *attr,
		      char *buf)
{
	int var;

	if (strcmp(attr->attr.name, "attr_1") == 0)
		var = Driver_Module_obj->attr_1;
	else
		var = Driver_Module_obj->attr_2;
	return sprintf(buf, "%d\n", var);
}

/*
 * This function called when attributes file write
 */
static ssize_t attr_store(struct Driver_Module_obj *Driver_Module_obj, struct Driver_Module_attribute *attr,
		       const char *buf, size_t count)
{
	int var, ret;

	ret = kstrtoint(buf, 10, &var);
	if (ret < 0)
		return ret;

	if (strcmp(attr->attr.name, "attr_1") == 0)
		Driver_Module_obj->attr_1 = var;
	else
		Driver_Module_obj->attr_2 = var;
	return count;
}

static struct Driver_Module_attribute attr_1_attribute =
	__ATTR(attr_1, 0664, attr_show, attr_store);
static struct Driver_Module_attribute attr_2_attribute =
	__ATTR(attr_2, 0664, attr_show, attr_store);

/*
 * Create a group of attributes so that we can create and destroy them all
 * at once.
 */
static struct attribute *Driver_Module_default_attrs[] = {
	&attr_1_attribute.attr,
	&attr_2_attribute.attr,
	NULL,
};
ATTRIBUTE_GROUPS(Driver_Module_default);

/*
 * We define ktype for our kobjects. 
 */
static struct kobj_type Driver_Module_ktype = {
	.sysfs_ops = &Driver_Module_sysfs_ops,
	.release = Driver_Module_release,
	.default_groups = Driver_Module_default_groups,
};

static struct kset *Driver_Module_kset;
static struct Driver_Module_obj *dev_1_obj;
static struct Driver_Module_obj *dev_2_obj;
static struct Driver_Module_obj *dev_3_obj;

static struct Driver_Module_obj *create_Driver_Module_obj(const char *name)
{
	struct Driver_Module_obj *Driver_Module;
	int retval;

	/* allocate the memory for the object */
	Driver_Module = kzalloc(sizeof(*Driver_Module), GFP_KERNEL);
	if (!Driver_Module)
		return NULL;

	/*
	 * We specified a kset for this kobject
	 */
	Driver_Module->kobj.kset = Driver_Module_kset;

	/*
	 * Initialize and add the kobject to the kernel.We not pass parent here
	 * because parent placed beneath that kset
	 */
	retval = kobject_init_and_add(&Driver_Module->kobj, &Driver_Module_ktype, NULL, "%s", name);
	if (retval) {
		kobject_put(&Driver_Module->kobj);
		return NULL;
	}

	/*
	 * Here we sending the uevent that the kobject was added to the system.
	 */
	kobject_uevent(&Driver_Module->kobj, KOBJ_ADD);

	return Driver_Module;
}

static void destroy_Driver_Module_obj(struct Driver_Module_obj *Driver_Module)
{
	kobject_put(&Driver_Module->kobj);
}

static int __init Driver_Module_init(void)
{
	/*
	 * Create a kset with the name of "Driver_Module",
	 * located under /sys/kernel/
	 */
	Driver_Module_kset = kset_create_and_add("Driver_Module", NULL, kernel_kobj);
	if (!Driver_Module_kset)
		return -ENOMEM;
	
	printk(KERN_INFO "Driver Module: kset creation done!!!\n");

	/*
	 * We had create three object here and add them to kset
	 */
	dev_1_obj = create_Driver_Module_obj("dev_1");
	if (!dev_1_obj)
		goto dev_1_error;
	printk(KERN_INFO "Driver Module: dev_1 kobject creation done!!!\n");

	dev_2_obj = create_Driver_Module_obj("dev_2");
	if (!dev_2_obj)
		goto dev_2_error;
	printk(KERN_INFO "Driver Module: dev_2 kobject creation done!!!\n");

	dev_3_obj = create_Driver_Module_obj("dev_3");
	if (!dev_3_obj)
		goto dev_3_error;
	printk(KERN_INFO "Driver Module: dev_3 kobject creation done!!!\n");

	printk(KERN_INFO "Driver Module: Driver Module Initialization Done!!!\n");

	return 0;

dev_3_error:
	destroy_Driver_Module_obj(dev_3_obj);
	printk(KERN_ERR "Driver Module: dev_3 kobject creation Failed!!!\n");
dev_2_error:
	destroy_Driver_Module_obj(dev_1_obj);
	printk(KERN_ERR "Driver Module: dev_2 kobject creation Failed!!!\n");
dev_1_error:
	kset_unregister(Driver_Module_kset);
	printk(KERN_ERR "Driver Module: dev_1 kobject creation Failed!!!\n");
	printk(KERN_ERR "Driver Module: Driver Module Initialization Failed!!!\n");
	return -EINVAL;
}

static void __exit Driver_Module_exit(void)
{
	destroy_Driver_Module_obj(dev_3_obj);
	destroy_Driver_Module_obj(dev_2_obj);
	destroy_Driver_Module_obj(dev_1_obj);
	kset_unregister(Driver_Module_kset);
	printk(KERN_INFO "Driver Module: Driver Module Removed!!!\n");
}

module_init(Driver_Module_init);
module_exit(Driver_Module_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Embedded Buddies");
MODULE_DESCRIPTION("kset example Driver Module");
MODULE_VERSION("1:0.1");
