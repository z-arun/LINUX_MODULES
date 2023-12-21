
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/delay.h>
#include<linux/semaphore.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arun S Nair");
MODULE_DESCRIPTION("simple module using kthreadi and semaphore");


struct task_struct * ts;
struct task_struct * ts1;
struct task_struct * ts2;
static int abc=123;
static struct semaphore sem;


int mythread(void * ptr)
{
        int x = 0;
        pr_alert("In thread 1");
        while(!kthread_should_stop())
        {
                pr_alert("Thtread 1\n");
                if(++x % 10 == 0)
                {
                        pr_alert("UPPPP thead 1 \n");
                        up(&sem);
                }
                ssleep(1);
        }
        return 0;
}

int thread2(void * ptr)
{
        int y = 0;
        while(!kthread_should_stop())
        {
                pr_alert("Thread 2 \n");
                ssleep(1);
                if(++y % 5 == 0)
                {
                        pr_alert("UPPPPP thread 2\n");
                        up(&sem);
                }
        }
        return 0;
}

int thread3(void * ptr)
{


        while(!kthread_should_stop())
        {
                down(&sem);
                pr_alert("DOWN ------- In thread 3 \n");
                ssleep(1);
        }
        return 0;
}

int __init startFun(void)
{
        printk(KERN_INFO "\nHello world\n");
        ts = kthread_create(mythread,(void *)(&abc),"mythread1");
        ts1 = kthread_create(thread2,NULL,"mythread2");
        ts2 = kthread_create(thread3,NULL,"Mythread3");
        if(ts && ts1 && ts2)
        {
                pr_alert("Thread created ");
        }
        sema_init(&sem,0);
        wake_up_process(ts);
        wake_up_process(ts1);
        wake_up_process(ts2);
        return 0;
}

void __exit endFun(void)
{
        printk(KERN_INFO "\nbye world\n");
        kthread_stop(ts);
        kthread_stop(ts1);
        kthread_stop(ts2);
}

module_init(startFun);
module_exit(endFun);
