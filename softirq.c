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
#include<linux/interrupt.h>

#define LED_GPIO 60u  //P9_12

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("simple module");
MODULE_AUTHOR("Arun S Nair");


/*
 * To use softirq, there are some compile time changes needed 
 * in /linux/interrupt.h and /kernel/softirq.c
 *
 * enum
 * {
 *      HI_SOFTIRQ=0,
 *      TIMER_SOFTIRQ,
 *      NET_TX_SOFTIRQ,
 *      NET_RX_SOFTIRQ,
 *      BLOCK_SOFTIRQ,
 *      IRQ_POLL_SOFTIRQ,
 *      TASKLET_SOFTIRQ,
 *      SCHED_SOFTIRQ,
 *      HRTIMER_SOFTIRQ,
 *      RCU_SOFTIRQ,
 *     
 *      MY_SOFT_IRQ <-------add enum element for new softIRQ
 *
 *      NR_SOFTIRQS
 * };
 *
 * in kernel/softirq.c add EXPORT_SYMBOL for the following 
 *
 * open_softirq
 * raise_softirq
 * raise_softirq_trqoff
 *
 */

static struct class *cl;

static irqreturn_t irqHandler(int irq,void * dev_id)
{
        static unsigned long flag;
        local_irq_save(flag);
        pr_info("\n TEST : in IRQQQ");
        local_irq_restore(flag);
        raise_softirq(MY_SOFT_IRQ);
        return IRQ_HANDLED;
}

static void my_soft_irq_handler(struct softirq_action *sa)
{
        pr_info("In soft irq handler\n");
}

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

        gpio_direction_input(LED_GPIO);
        gpio_export(LED_GPIO,1);

        uint8_t value =  gpio_get_value(LED_GPIO);

        pr_info("\nGPIO value  %d  ", value);


        if(gpio_set_debounce(LED_GPIO,200) < 0)
                pr_info("\nGPIO debounce set error");

        int irqnum = gpio_to_irq(LED_GPIO);
                pr_info("\nirq num : %d  ", irqnum);

        if(request_irq(irqnum,irqHandler,IRQF_TRIGGER_RISING,"test_irq",NULL))
                pr_info("\nirq register failed");

        open_softirq(MY_SOFT_IRQ,my_soft_irq_handler);

        return 0;
}

void __exit endFun(void)
{
        class_destroy(cl);
        gpio_free(LED_GPIO);
}

module_init(startFun);
module_exit(endFun);
