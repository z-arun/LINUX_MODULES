#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/delay.h>
#include<linux/mutex.h>
#include<linux/completion.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arun S Nair");
MODULE_DESCRIPTION("simple module using kthread and completion");

struct task_struct * ts;
struct task_struct * ts1;
struct task_struct * ts2;
static int abc = 123;

struct completion comp;


int mythread(void * ptr)
{
        int x = 0;
        pr_alert("In thread 1\n");
        while(!kthread_should_stop())
        {
                msleep(400);
                pr_alert("In thread 1 \n");
                if(x++ == 10)
                {
                        complete_all(&comp);
                }
        }

        return 0;
}

int thread2(void * ptr)
{
        wait_for_completion(&comp);
        pr_alert("Thread 2 after completion \n");
        ssleep(1);
        while(!kthread_should_stop())
                {
                        pr_alert("Thread 2 \n");
                        ssleep(1);
                }

        return 0;
}

int thread3(void * ptr)
{
        wait_for_completion(&comp);
        pr_alert("In thread 3 after completion \n");
        ssleep(1);
        while(!kthread_should_stop())
        {
                pr_err("Thread 3 \n");
                ssleep(1);
        }
        return 0;
}

int __init startFun(void)
{
        printk(KERN_INFO "\nHello world\n");
        ts = kthread_create(mythread,(void *)(&abc),"mythread1");
        ts1 = kthread_create(thread2,NULL,"mythread2");
        ts2 = kthread_create(thread3,NULL,"my thread 3");
        if(ts && ts1 && ts2)
        {
                pr_alert("Threads created \n");
        }

        init_completion(&comp);
        wake_up_process(ts);
        wake_up_process(ts1);
        wake_up_process(ts2);

        return 0;
}

void __exit endFun(void)
{
        printk(KERN_INFO "\nbye world\n");
        if(completion_done(&comp))
        {
                complete_all(&comp);
        }
        kthread_stop(ts);
        kthread_stop(ts1);
        kthread_stop(ts2);
}

module_init(startFun);
module_exit(endFun);
