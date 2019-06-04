#include<reg51.h>
#include<bujin.h>
#include"Lcd1602.h"
/*
���ܣ�ʵ�ֶԲ�������Ŀ���
��������λ�ƣ�����ת��
*/
//���岽�����ʱ��ͼ
code int positiveRe[8]={0x80,0xc0,0x40,0x60,0x20,0x30,0x10,0x90};

//������ʱ�䣬��λΪ����
unsigned int time;


//��չ�ⲿ����i
extern unsigned int i;
extern double arc;
extern bit direction;
extern double n;
extern unsigned count;
extern unsigned char dat[11];


//�����������ʾ����
code unsigned char numbers[10]={0x77,0x14,0xb3,0xb6,0xd4,0xe6,0xef,0x34,0xf7,0xf6};

void run()
{
	

	unsigned int j;

	//�û��ȼ��������
	count=(arc/3.14*180)/STEP;

	//����ÿһ����Լ��Ҫ����ʱN*10us
	time=100000l/n/COUNT;

	//ʵ�����彻��仯
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
		
				//����LCD1602
				if(j%256==0)
				{
					processData((j/256)*6.28/16,direction,n,dat);
					display(dat);
					
				}
				//�ж��Ƿ�ת����һȦ
			if(j%COUNT==0)
			{
				//���������
					SHUMA=numbers[j/COUNT];

			}
			
			//��ʱ��Ӧʱ��
			bujinDelay_n_10us(time-4);
			
			//bujinDelay_nms((unsigned int)(time*1000));
	}
}
	//��ʱ��Ӧʱ��
	void bujinDelay_n_10us(unsigned int i)
	{
		while(i--);
	}
	

