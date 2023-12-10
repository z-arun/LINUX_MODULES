#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/delay.h>
#include<linux/mutex.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arun S Nair");
MODULE_DESCRIPTION("simple module using kthreadi and mutex");

struct task_struct * ts;
struct task_struct * ts1;
static int abc=123;
static struct mutex mu;
static int x = 0;
static bool mutex_status;

int mythread(void * ptr)
{

        pr_alert("In thread 1");
        while(!kthread_should_stop())
        {
                mutex_lock(&mu); // sleeping here
                //mutex_trylock(&mu); // no sleep
                x++;
                mutex_status  = mutex_is_locked(&mu);
                mutex_unlock(&mu);
                pr_alert("In thread one %d %d\n",x,mutex_status);
                ssleep(1);
        }
        return 0;
}

int thread2(void * ptr)
{
        while(!kthread_should_stop())
        {
                mutex_lock(&mu);
                x++;
                mutex_unlock(&mu);

                pr_alert("In thread two %d \n",x);
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
        mutex_init(&mu);
        return 0;
}

void __exit endFun(void)
{
        printk(KERN_INFO "\nbye world\n");
        kthread_stop(ts);
        kthread_stop(ts1);
        mutex_destroy(&mu);
}

module_init(startFun);
module_exit(endFun);
