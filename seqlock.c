#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/delay.h>
#include<linux/mutex.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arun S Nair");
MODULE_DESCRIPTION("simple module using kthread and seqlock");

struct task_struct * ts;
struct task_struct * ts1;
struct task_struct * ts2;
static int abc = 123;
static int x = 0;
static seqlock_t sq;

int mythread(void * ptr)
{
        pr_alert("In thread 1");
        while(!kthread_should_stop())
        {
                write_seqlock(&sq);
                x++;
                write_sequnlock(&sq);
                pr_alert("In thread 1 %d \n",x);
                msleep(5000);
        }
        return 0;
}

int thread2(void * ptr)
{
        int lock;
        while(!kthread_should_stop())
        {

                do
                {
                        lock = read_seqbegin(&sq);
                        pr_err("In thread 2 :: %d\n",x);

                }while(read_seqretry(&sq,lock));

                ssleep(2);
        }
        return 0;
}

int thread3(void * ptr)
{
        int seq;
        while(!kthread_should_stop())
        {
                do
                {
                        seq = read_seqbegin(&sq);
                        pr_alert("In thread 3 :: %d   \n",x);
                }while(read_seqretry(&sq,seq));
                ssleep(1);
        }
        return 0;
}

int __init startFun(void)
{
        printk(KERN_INFO "\n Hello world \n");
        seqlock_init(&sq); 
        ts = kthread_create(mythread,(void *)(&abc),"mythread1");
        ts1 = kthread_create(thread2,NULL,"mythread2");
        ts2 = kthread_create(thread3,NULL,"my thread 3");
        if(ts && ts1 && ts2)
        {
                pr_alert("Thread created ");
        }
        wake_up_process(ts);
        wake_up_process(ts1);
        wake_up_process(ts2);
        return 0;
}

void __exit endFun(void)
{
        printk(KERN_INFO "\n bye world \n");
        kthread_stop(ts);
        kthread_stop(ts1);
        kthread_stop(ts2);
}

module_init(startFun);
module_exit(endFun);
