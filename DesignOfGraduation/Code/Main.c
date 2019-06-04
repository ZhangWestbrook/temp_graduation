#include<reg51.h>
#include<bujin.h>
#include<remoteController.h>
#include<Lcd1602.h>

#include"serialPort.h"


/********************************************************************************************
功能： 该程序是通过自写的上位机实现步进电机的控制，可以控制步进电机的转向，转动位移，转动速度，并能通过红外热感感应到人实现
步进电机的急停。并能将相关数据上传到电脑，用以文件的形势来保存这些数据


**********************************************************************************************/
extern unsigned int count;
extern unsigned int time;
void delay(unsigned int i);
//将接收到的数据进行转换
void convert();

//中断初始化函数
void interInit();

//定义一个计时全局变量
unsigned int i;

//定义全局变量
bit direction=0;
double arc=0.0;
double arc_temp=0.0;
double n=1;
unsigned int count; 
unsigned char dat[11];

//定义按键触发位
sbit accelerate=P3^7;
sbit decline=P3^6;
sbit startAndEnd=P3^5;
sbit reverse_direction=P3^4;

sbit int0=P3^2;
sbit int1=P3^3;

bit flag_start=0;
bit flag=1;



//以下主程序我列出了主要的框架，还要通过以后的编写来实现
void main()
{
	

	unsigned char i;
	//unsigned char symbal;
	
	
	//串口初始化
	serialInit();
	//lcd1602初始化
	lcdInitial();
	//中断初始化
	interInit();
	SHUMA=0x77;
	while(1)
	{

		//上位机托管控制
		if(flag)
		{
			if(readByte()=='*')
			{
				if(flag)
				{
				for(i=0;i<11;i++)
				{
					dat[i]=readByte();
				}
				//转换为实际的数据
				convert();
				//开始运行
				run();
				}
			}


		}
		
		
		//下位机托管控制
		if(flag==0)
		{
			arc=62.80;
			n=0.25;
			//向上位机发送数据
			
			//调用步进电机运行程序   
			run();
			
			
		}
		
		
		//在运行过程中存在两个中断  一个是通过按键实现的普通暂停中断 另一个是保障人身安全的急停中断
		//如果走过一圈更新数码管显示
	}
	
}

void delay(unsigned int i)
{
	while(--i);
}


//输入格式比如 22.22522.22
void convert()
{
	unsigned int temp=0;
	unsigned char i;
	//处理弧度
	arc=0.0;
	for(i=0;i<5;i++)
	{
		if(dat[i]=='.')
			continue;
		temp*=10;
		temp+=(dat[i]-'0');
		
	}
	arc=temp;
	arc=arc*0.01;
	
	//处理方向
	if((dat[5]-'0')>0)
		direction =1;
	else 
		direction =0;
	
	
	//处理速度
	n=0.0;
	temp=0;
	for(i=0;i<5;i++)
	{
		if(dat[i+6]=='.')
			continue;
		temp*=10;
		temp+=(dat[6+i]-'0');
	}
	n=temp;
	n=n*0.01;
}

//中断初始化函数
void interInit()
{
	//初始化中断0
	IT0=1; //边沿触发方式
	EX0=1; //开中断
	EA=1;
	
	//初始化中断1
	IT1=1;
	EX1=1; //开中断
	
	startAndEnd=1;
	decline=1;
	accelerate=1;
}

//中断函数
void turn() interrupt 0
{
	EX0=0;
	
	direction=!direction;
	arc=0.0;
	count=0;
	EX0=1;
}

//定时函数，实现对i进行增加
void clock() interrupt 1
{

}

//中断1，进行判断启动，加/减速，正反向
void key() interrupt 2
{
	//关中断1
	EX1=0;
	
	if(startAndEnd==0)  //是否启动，是否由上位机托管
	{
		delay(100);  //消抖
		if(startAndEnd==0)
		{
			while(!startAndEnd);
				flag=!flag;  //切换模式
				arc=0.0;
				count=0;
		}
	}
	else
	{
		if(reverse_direction==0)  //是否变向
		{
			delay(100);
			if(reverse_direction==0)
			{
				while(!reverse_direction);
				direction=!direction;
				
			}
		}
		else
		{
			if(accelerate==0)  //是否加速
			{
				delay(100);
				if(accelerate==0)
				{
					while(!accelerate);
					n=n+0.1;
					time=10000l/n/COUNT;
					
				}
			}
			else
					if(decline==0)  //是否减速
				{
					delay(100);
					if(decline==0)
					{
						while(!decline);
						if((n-0.1)>0.0) {n=n-0.1;time=10000l/n/COUNT;}
					}
				}
		}
		
	}
	EX1=1;
}

void init_clock()
{
	
}