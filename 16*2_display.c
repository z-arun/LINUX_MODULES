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


#define RW 45
#define EN 69
#define RS 66



#define D0 67
#define D1 68
#define D2 44
#define D3 26
#define D4 46
#define D5 65
#define D6 27
#define D7 47



MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("16X2 LCD  module");
MODULE_AUTHOR("Arun S Nair");




unsigned char darray[8] = {D0,D1,D2,D3,D4,D5,D6,D7};

void update_data(unsigned char d)
{
        int i;
        for(i=0;i<8;i++)
        {
                if(d & (1<<i))
                {
                        gpio_set_value(darray[i],1);
                }
                else
                {
                        gpio_set_value(darray[i],0);
                }
        }

}

void send_command(unsigned char cmd)
{

        gpio_set_value(RW,0);
        gpio_set_value(RS,0);
        gpio_set_value(EN,1);
        mdelay(20);
        update_data(cmd);
        mdelay(20);
        gpio_set_value(EN,0);
}

void send_char(unsigned char data)
{
                gpio_set_value(RW,0);
                gpio_set_value(RS,1);
                gpio_set_value(EN,1);
                mdelay(20);
                update_data(data);
                mdelay(20);
                gpio_set_value(EN,0);
}

void disp_init(void)
{
        send_command(0x38);
        send_command(0x01);
        send_command(0x06);
        send_command(0x0c);
        send_command(0x80);
}


int __init startFun(void)
{
        int i = 0,c = 0;
        char data_array[20] = "Testing @#$%^";

        if(gpio_is_valid(D0) &&
           gpio_is_valid(D1) &&
           gpio_is_valid(D2) &&
           gpio_is_valid(D3) &&
           gpio_is_valid(D4) &&
           gpio_is_valid(D5) &&
           gpio_is_valid(D6) &&
           gpio_is_valid(D7) &&

           gpio_is_valid(EN) &&
           gpio_is_valid(RS) &&
           gpio_is_valid(RW)
           )
        {
                pr_info("\nTEST:GPIO VALID\n");
        }
        else
        {
                 pr_err("\nTEST:GPIO NOT VALID\n");
                 return -1;

        }


        if(  (gpio_request(D0,"d0") == 0 ) &&
             (gpio_request(D1,"d1") == 0 ) &&
             (gpio_request(D2,"d2") == 0 ) &&
             (gpio_request(D3,"d3") == 0 ) &&
             (gpio_request(D4,"d4") == 0 ) &&
             (gpio_request(D5,"d5") == 0 ) &&
             (gpio_request(D6,"d6") == 0 ) &&
             (gpio_request(D7,"d7") == 0 ) &&
             (gpio_request(EN,"en") == 0 ) &&
             (gpio_request(RS,"rs") == 0 ) &&
             (gpio_request(RW,"rw") == 0 )
            )
        {
                pr_info("\nTEST: GPIO request success\n");
        }
        else
        {
                 pr_info("\nTEST:gpio request failed\n");
                 return -1;
        }

        for(i=0;i<8;i++)
        {
                gpio_direction_output(darray[i],1);
                gpio_export(darray[i],0);
        }


        gpio_direction_output(EN,1);
        gpio_export(EN,0);

        gpio_direction_output(RW,1);
        gpio_export(RW,0);

        gpio_direction_output(RS,1);
        gpio_export(RS,0);


        disp_init();


        while(data_array[c])
        {
                send_char(data_array[c++]);
                mdelay(20);
        }

        return 0;
}

void __exit endFun(void)
{
        int i;
        for(i=0;i<8;i++)
        {
             gpio_free(darray[i]);
        }
        gpio_free(EN);
        gpio_free(RS);
        gpio_free(RW);
}

module_init(startFun);
module_exit(endFun);
