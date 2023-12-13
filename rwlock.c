#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arun S Nair");
MODULE_DESCRIPTION("simple module using kthread and rwlock");


struct task_struct * ts;
struct task_struct * ts1;
struct task_struct * ts2;

static int abc = 200;
static rwlock_t rwl;
static int x = 0;

int mythread(void * ptr)
{

        pr_alert("In thread 1");
        while(!kthread_should_stop())
        {
                write_lock(&rwl); 
                x++;                  // writing in this thread only
                write_unlock(&rwl);
                pr_alert("In thread one %d \n",x);
                ssleep(1);
        }
        return 0;
}

int thread2(void * ptr)
{
        while(!kthread_should_stop())
        {
                read_lock(&rwl);
                pr_alert("In thread two %d \n",x);
                read_unlock(&rwl);
                ssleep(2);
        }
        return 0;
}

int thread3(void * ptr)
{
        while(!kthread_should_stop())
        {
                read_lock(&rwl);
                pr_alert("In thread 3 x = %d\n",x);
                read_unlock(&rwl);
                ssleep(3);
        }
        return 0;
}

int __init startFun(void)
{
        printk(KERN_INFO "\nHello world\n");
        ts = kthread_create(mythread,(void *)(&abc),"mythread1");
        ts1 = kthread_create(thread2,NULL,"mythread2");
        ts2 = kthread_create(thread3,NULL,"mythread3");
        if(ts && ts1 && ts2)
        {
                pr_alert("Thread created ");
        }
        wake_up_process(ts);
        wake_up_process(ts1);
        wake_up_process(ts2);
        rwlock_init(&rwl);
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
