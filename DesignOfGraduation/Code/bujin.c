#include<reg51.h>
#include<bujin.h>
#include"Lcd1602.h"
/*
功能：实现对步进电机的控制
参数：角位移，方向，转速
*/
//定义步进电机时序图
code int positiveRe[8]={0x80,0xc0,0x40,0x60,0x20,0x30,0x10,0x90};

//定义间隔时间，单位为毫秒
unsigned int time;


//拓展外部变量i
extern unsigned int i;
extern double arc;
extern bit direction;
extern double n;
extern unsigned count;
extern unsigned char dat[11];


//定义数码管显示数组
code unsigned char numbers[10]={0x77,0x14,0xb3,0xb6,0xd4,0xe6,0xef,0x34,0xf7,0xf6};

void run()
{
	

	unsigned int j;

	//用弧度计算出步数
	count=(arc/3.14*180)/STEP;

	//计算每一步大约需要的延时N*10us
	time=100000l/n/COUNT;

	//实现脉冲交替变化
	for(j=0;j<count;j++)
	{
		if(direction)
		switch(j%8)
		{
			case 0:PORT=(PORT&0X0F)|positiveRe[0];break;
			case 1:PORT=(PORT&0X0F)|positiveRe[1];break;
			case 2:PORT=(PORT&0X0F)|positiveRe[2];break;
			case 3:PORT=(PORT&0X0F)|positiveRe[3];break;
			case 4:PORT=(PORT&0X0F)|positiveRe[4];break;
			case 5:PORT=(PORT&0X0F)|positiveRe[5];break;
			case 6:PORT=(PORT&0X0F)|positiveRe[6];break;
			case 7:PORT=(PORT&0X0F)|positiveRe[7];break;
		}
		else
			switch(j%8)
				{
			case 7:PORT=(PORT&0X0f)|positiveRe[0];break;
			case 6:PORT=(PORT&0X0f)|positiveRe[1];break;
			case 5:PORT=(PORT&0X0f)|positiveRe[2];break;
			case 4:PORT=(PORT&0X0f)|positiveRe[3];break;
			case 3:PORT=(PORT&0X0f)|positiveRe[4];break;
			case 2:PORT=(PORT&0X0f)|positiveRe[5];break;
			case 1:PORT=(PORT&0X0f)|positiveRe[6];break;
			case 0:PORT=(PORT&0X0f)|positiveRe[7];break;
				}
		
				//更新LCD1602
				if(j%256==0)
				{
					processData((j/256)*6.28/16,direction,n,dat);
					display(dat);
					
				}
				//判断是否转过了一圈
			if(j%COUNT==0)
			{
				//更新数码管
					SHUMA=numbers[j/COUNT];

			}
			
			//延时相应时间
			bujinDelay_n_10us(time-4);
			
			//bujinDelay_nms((unsigned int)(time*1000));
	}
}
	//延时相应时间
	void bujinDelay_n_10us(unsigned int i)
	{
		while(i--);
	}
	

