#include"serialPort.h"

extern bit flag;
//���ڳ�ʼ��������
void serialInit()
{
	TMOD=0X20;//�ö�ʱ��1��ģʽ2
	TH1=TL1=0XFD;//��������Ϊ9600
	SCON=0X50;//���ڷ�ʽ1��TI��RI���㣬�������
	TR1=1;  //��ʱ��
}

//����һ������һ���ֽڵĺ���
void sendByte(unsigned char dat)
{
	SBUF=dat;
	while(!TI);  //�ȴ����
	TI=0;
}

//����һ������һ���ֽڵĺ���
unsigned char readByte()
{
	unsigned char dat;
	while(!RI&&flag);
	dat=SBUF;
	RI=0;
	return dat;
}
