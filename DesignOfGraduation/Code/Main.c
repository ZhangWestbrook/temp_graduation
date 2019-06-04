#include<reg51.h>
#include<bujin.h>
#include<remoteController.h>
#include<Lcd1602.h>

#include"serialPort.h"


/********************************************************************************************
���ܣ� �ó�����ͨ����д����λ��ʵ�ֲ�������Ŀ��ƣ����Կ��Ʋ��������ת��ת��λ�ƣ�ת���ٶȣ�����ͨ�������ȸи�Ӧ����ʵ��
��������ļ�ͣ�����ܽ���������ϴ������ԣ������ļ���������������Щ����


**********************************************************************************************/
extern unsigned int count;
extern unsigned int time;
void delay(unsigned int i);
//�����յ������ݽ���ת��
void convert();

//�жϳ�ʼ������
void interInit();

//����һ����ʱȫ�ֱ���
unsigned int i;

//����ȫ�ֱ���
bit direction=0;
double arc=0.0;
double arc_temp=0.0;
double n=1;
unsigned int count; 
unsigned char dat[11];

//���尴������λ
sbit accelerate=P3^7;
sbit decline=P3^6;
sbit startAndEnd=P3^5;
sbit reverse_direction=P3^4;

sbit int0=P3^2;
sbit int1=P3^3;

bit flag_start=0;
bit flag=1;



//�������������г�����Ҫ�Ŀ�ܣ���Ҫͨ���Ժ�ı�д��ʵ��
void main()
{
	

	unsigned char i;
	//unsigned char symbal;
	
	
	//���ڳ�ʼ��
	serialInit();
	//lcd1602��ʼ��
	lcdInitial();
	//�жϳ�ʼ��
	interInit();
	SHUMA=0x77;
	while(1)
	{

		//��λ���йܿ���
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
				//ת��Ϊʵ�ʵ�����
				convert();
				//��ʼ����
				run();
				}
			}


		}
		
		
		//��λ���йܿ���
		if(flag==0)
		{
			arc=62.80;
			n=0.25;
			//����λ����������
			
			//���ò���������г���   
			run();
			
			
		}
		
		
		//�����й����д��������ж�  һ����ͨ������ʵ�ֵ���ͨ��ͣ�ж� ��һ���Ǳ�������ȫ�ļ�ͣ�ж�
		//����߹�һȦ�����������ʾ
	}
	
}

void delay(unsigned int i)
{
	while(--i);
}


//�����ʽ���� 22.22522.22
void convert()
{
	unsigned int temp=0;
	unsigned char i;
	//������
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
	
	//������
	if((dat[5]-'0')>0)
		direction =1;
	else 
		direction =0;
	
	
	//�����ٶ�
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

//�жϳ�ʼ������
void interInit()
{
	//��ʼ���ж�0
	IT0=1; //���ش�����ʽ
	EX0=1; //���ж�
	EA=1;
	
	//��ʼ���ж�1
	IT1=1;
	EX1=1; //���ж�
	
	startAndEnd=1;
	decline=1;
	accelerate=1;
}

//�жϺ���
void turn() interrupt 0
{
	EX0=0;
	
	direction=!direction;
	arc=0.0;
	count=0;
	EX0=1;
}

//��ʱ������ʵ�ֶ�i��������
void clock() interrupt 1
{

}

//�ж�1�������ж���������/���٣�������
void key() interrupt 2
{
	//���ж�1
	EX1=0;
	
	if(startAndEnd==0)  //�Ƿ��������Ƿ�����λ���й�
	{
		delay(100);  //����
		if(startAndEnd==0)
		{
			while(!startAndEnd);
				flag=!flag;  //�л�ģʽ
				arc=0.0;
				count=0;
		}
	}
	else
	{
		if(reverse_direction==0)  //�Ƿ����
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
			if(accelerate==0)  //�Ƿ����
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
					if(decline==0)  //�Ƿ����
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