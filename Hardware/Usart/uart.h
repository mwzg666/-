#ifndef __UART_H__
#define __UART_H__

//===================================����=================================
#include "main.h"

#define Baudrate        115200UL
#define Baudrate3       9600UL

#define UART_BUFF_LENGTH 128

#define MAX_LENGTH      200

#define USER_MAIN_DEBUG




typedef struct
{
    alt_u32   Timer;
    alt_u16   RecvLength;                  //���յ����ݳ���
    alt_u16   SendLength;                  //Ҫ�������ݳ���
    alt_u8    RecvBuff[MAX_LENGTH];	       //���ڽ��ջ���
    alt_u8    SendBuff[MAX_LENGTH];	       //���ڽ��ջ���
}UART_DATA;


//==================================��������=============================

extern u8  TX1_Cnt;    //���ͼ���
extern u8  RX1_Cnt;    //���ռ���
extern bit B_TX1_Busy; //����æ��־
extern u16 Rx1_Timer;

extern u8  TX2_Cnt;    //���ͼ���
extern u8  RX2_Cnt;    //���ռ���
extern bit B_TX2_Busy; //����æ��־
extern u16 Rx2_Timer;

extern u8  RX3_Cnt;    //���ռ���
extern u8  TX3_Cnt;    //���ͼ���
extern bit B_TX3_Busy; //����æ��־
extern u16 Rx3_Timer;

extern u8  RX4_Cnt;    //���ռ���
extern u8  TX4_Cnt;    //���ͼ���
extern bit B_TX4_Busy; //����æ��־
extern u16 Rx4_Timer;


extern u8  xdata RX2_Buffer[]; //���ջ���
extern u8  xdata RX3_Buffer[]; //���ջ���

extern UART_DATA xdata g_UartData[2];

//==================================��������=============================

void Uart1_Init(void);
void Uart2_Init(void);


void Uart1Send(u8 *buf, u8 len);
void Uart2Send(u8 *buf, u8 len);


void ClearUart1Buf();
void ClearUart2Buf();


void Clear_Uart1_Buf();
void Clear_Uart2_Buf();


void Uart1Hnd();
bool Uart2Hnd();

#endif



