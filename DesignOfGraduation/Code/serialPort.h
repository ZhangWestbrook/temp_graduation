#ifndef __SERIALPORT_H_
#define __SERIALPORT_H_
#include<reg51.h>

//��������
void serialInit();
void sendByte(unsigned char dat);
unsigned char readByte();


#endif