#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

static int __init hello_init(void) {
    printk(KERN_INFO "Hello World!\n");
    return 0;
}

module_init(hello_init);
