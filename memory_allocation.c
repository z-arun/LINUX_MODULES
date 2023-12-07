#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/slab.h>
#include<linux/vmalloc.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arun S Nair");
MODULE_DESCRIPTION("Simpe module");

void * ptr  = NULL;
void * ptr1 = NULL;
void * ptr2 = NULL;

int __init startFun(void)
{
        ptr  = kmalloc(10,GFP_KERNEL); // both physical and virtual continuous
        ptr1 = kzalloc(10,GFP_KERNEL); // initializes to 0
        ptr2 = vmalloc(10);            // physical memory may not be continuous
        if(ptr == NULL)
        {
                pr_alert("Memory allocation failed");
                return -1;
        }
        (*(int*)ptr) = 100;
        printk(KERN_INFO "\nHello world %x  address:: %x\n",(*(int *)ptr),ptr);
        return 0;
}

void __exit endFun(void)
{
        kfree(ptr);
        kfree(ptr1);
        vfree(ptr2);
        printk(KERN_INFO "\nbye world\n");
}

module_init(startFun);
module_exit(endFun);
