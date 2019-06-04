#include<reg51.h>
#include<Lcd1602.h>
#include<intrins.h>

/*
��ʼ������
*/
void lcdInitial()
{
	
	lcdWriteCommon(0x38);//����8λ��ʽ������
	lcdWriteCommon(0x0c);//������ʾ���ع��.����˸
	lcdWriteCommon(0x06);//�趨���뷽ʽ����������λ
	lcdWriteCommon(0x01);//�����Ļ��ʾ
	lcdWriteCommon(0x80);  //�������
	delay40us(2);//�ȴ��������
	
}

//��оƬ��д������
//�����ش������ݽ���
void lcdWriteCommon(unsigned char common)
{

	lcdE=0;  //ʹ��
	lcdRS=0;  //д������
	lcdRW=0;  //д������
	
	INTERFACE=common;  //��������
	delay40us(1);
	
	lcdE=1;   //�����ش���
	delay40us(1);
	lcdE=0;//����ʹ��



}

//д������
void lcdWriteData(unsigned char dat)
{
	lcdE=0; //ʹ��
	lcdRS=1; //ѡ������
	lcdRW=0; //ѡ��д��
	
	INTERFACE=dat;  //��������
	delay40us(1);  //�ȴ������ȶ�
	_nop_();
	
	lcdE=1;
	delay40us(1);
	lcdE=0;  //����ʹ��
	
}

//��ʾ����ַ�
void display(unsigned char dat[11])
{
	unsigned char address=0;
	unsigned char i=0;
	unsigned char str1[]="arc:";
	unsigned char str2[]="dir:";
	unsigned char str3[]="n:";
	
		address+=0x80;//��1�е���ʼ��ַ��������ؿ���λ
	//��ʾ����
		lcdWriteCommon(address);
		for(i=0;i<4;i++)
			{
				lcdWriteData(str1[i]);
			}
		for(i=0;i<5;i++)
			{
				lcdWriteData(dat[4-i]);
			}
			
			//��ʾ����
		lcdWriteCommon(address+11);		
		for(i=0;i<4;i++)
			{
				lcdWriteData(str2[i]);
			}
			lcdWriteData(dat[5]);
			
		address=0x00;
		address+=0xc0;//��2�е���ʼ��ַ��������ؿ���λ
			//��ʾ���ٶ�
		lcdWriteCommon(address);
		for(i=0;i<2;i++)
			{
				lcdWriteData(str3[i]);
			}
			for(i=0;i<5;i++)
			{
				lcdWriteData(dat[10-i]);
			}
}
//��ʱ����
void delay40us(unsigned char time)
{
	unsigned char i,j;
	for(i=time ;i>0;i--)
		for(j=0;j<2;j++);
						
}

//���ݴ�����
void processData(double arc,bit direction,double speed,unsigned char dat[11])
{
	unsigned int temp=0;
	unsigned int i;
	
	//������
	temp=arc*100;
	for(i=0;i<5;i++)
	{
		if(i==2)
		{
			dat[i]='.';
			continue;
		}
		dat[i]=temp%10+'0';
		temp=temp/10;
	}
	
	//������
	if(direction==1)
		dat[i++]='S';
	else
		dat[i++]='N';
	
	//�����ٶ�
	temp=0;
	temp=speed*100;
	for(;i<11;i++)
	{
		if(i==8)
		{
			dat[i]='.';
			continue;
		}
		dat[i]=temp%10+'0';
		temp=temp/10;
	}
}
