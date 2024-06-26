#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"

//===================================����=================================

#define SysTick         9216                        // 10ms,ϵͳ�δ�Ƶ��, ��4000~16000֮��
#define Timer0_Reload   (65536UL - SysTick)         //Timer 0 �ж�Ƶ��


//==================================��������=============================
extern u16  Timer0Cnt;



//==================================��������=============================

void Timer0_Init(void);
void Timer1_Init(void);
void Delay(WORD ms);

void delay_ms(unsigned int ms);
void delay_us(BYTE us);    
void TimerTask();



#endif

