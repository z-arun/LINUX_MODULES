#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
#include<linux/gpio.h>
#include<linux/delay.h>

// Beagle bone black P9_12
#define LED_GPIO 60u

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("simple module - gpio");
MODULE_AUTHOR("Arun S Nair");

static struct class *cl;

int __init startFun(void)
{
        if(gpio_is_valid(LED_GPIO))
        {
                pr_info("\nTEST:GPIO VALID\n");
        }
        else
        {
                 pr_info("\nTEST:GPIO NOT VALID\n");
        }
        cl = class_create(THIS_MODULE,"my_class");
        if(gpio_request(LED_GPIO,"p912")==0)
        {
                pr_info("\nTEST: GPIO request success\n");
        } else
        {
                 pr_info("\nTEST:gpio request failed\n"); 
        }
        gpio_direction_output(LED_GPIO,1);
        gpio_export(LED_GPIO,0);

        gpio_set_value(LED_GPIO,0);
        mdelay(1000);
        gpio_set_value(LED_GPIO,1);
        mdelay(1000);
        gpio_set_value(LED_GPIO,0);
        mdelay(1000);
        gpio_set_value(LED_GPIO,1);
        mdelay(1000);
        gpio_set_value(LED_GPIO,0);
        mdelay(1000);
        gpio_set_value(LED_GPIO,1);
        mdelay(1000);
        return 0;
}

void __exit endFun(void)
{
        class_destroy(cl);
        gpio_free(LED_GPIO);
}

module_init(startFun);
module_exit(endFun);
