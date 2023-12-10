#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arun S Nair");
MODULE_DESCRIPTION("simple module using kthread");


struct task_struct * ts;
struct task_struct * ts1;

static int abc=123;
atomic_t var = ATOMIC_INIT(100);

int mythread(void * ptr)
{

        pr_alert("In thread 1 %d",atomic_read(&var));
        atomic_set(&var,200);
        while(!kthread_should_stop())
        {
                atomic_add(1,&var);
                pr_alert("In thread one %d ",atomic_read(&var));
                ssleep(1);
        }
        return 0;
}


int thread2(void * ptr)
{
        while(!kthread_should_stop())
        {
                atomic_add(2,&var);
                atomic_sub(1,&var);
                atomic_inc(&var);
                atomic_dec(&var);
                pr_alert("In thread two %d",atomic_read(&var));
                ssleep(2);
        }
        return 0;
}

int __init startFun(void)
{
        printk(KERN_INFO "\nHello world\n");
        ts = kthread_create(mythread,(void *)(&abc),"mythread1");
        ts1 = kthread_create(thread2,NULL,"mythread2");
        if(ts && ts1)
        {
                pr_alert("Thread created ");
        }
        wake_up_process(ts);
        wake_up_process(ts1);
        return 0;
}

void __exit endFun(void)
{
        printk(KERN_INFO "\nbye world\n");
        kthread_stop(ts);
        kthread_stop(ts1);
}

module_init(startFun);
module_exit(endFun);
