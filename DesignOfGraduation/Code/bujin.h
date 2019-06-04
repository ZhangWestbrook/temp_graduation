#ifndef _BUJIN_H_
#define _BUJIN_H_

//宏定义步距角
#define STEP (5.625/64)

//宏定义高低电平
#define A 0
#define B 1

//定义相关脉冲接口
#define PORT P2

#define SHUMA P0

//定义一圈需要的脉冲个数
#define COUNT 4096

//各功能函数的申明
void run();
void bujinDelay_n_10us(unsigned int i);

#endif
