#include "main.h"
#include "uart.h"
#include "cmd.h"

u16 Rx1_Timer  = 0;
u16 Rx2_Timer  = 0;
u16 Rx3_Timer  = 0;
u16 Rx4_Timer  = 0;


u8  TX1_Cnt;    //���ͼ���
u8  RX1_Cnt;    //���ռ���
bit B_TX1_Busy; //����æ��־

u8  TX2_Cnt;    //���ͼ���
u8  RX2_Cnt;    //���ռ���
bit B_TX2_Busy; //����æ��־

u8  TX3_Cnt;
u8  RX3_Cnt;
bit B_TX3_Busy;

u8  TX4_Cnt;
u8  RX4_Cnt;
bit B_TX4_Busy;


u8  RX1_Buffer[MAX_LENGTH]; //���ջ���
u8  RX2_Buffer[MAX_LENGTH]; //���ջ���
u8  RX3_Buffer[MAX_LENGTH]; //���ջ���
u8  RX4_Buffer[MAX_LENGTH]; //���ջ���

UART_DATA xdata g_UartData[2];


void Uart1_Init(void)        //115200bps@11.0592MHz
{

    TR1 = 0;
    AUXR &= ~0x01;      //S1 BRT Use Timer1;
    AUXR |=  (1<<6);    //Timer1 set as 1T mode
    TMOD &= ~(1<<6);    //Timer1 set As Timer
    TMOD &= ~0x30;      //Timer1_16bitAutoReload;
    TH1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate) / 256);
    TL1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate) % 256);
    ET1 = 0;    //��ֹ�ж�
    INTCLKO &= ~0x02;  //�����ʱ��
    TR1  = 1;

    /*************************************************/
    //UART1ģʽ, 0x00: ͬ����λ���, 0x40: 8λ����,�ɱ䲨����, 
    //           0x80: 9λ����,�̶�������, 0xc0: 9λ����,�ɱ䲨���� 

    SCON = (SCON & 0x3f) | 0x40; 

    PS  = 0;    //�жϸ����ȼ�
    PSH = 0;
    //PS  = 1;    //�����ȼ��ж�
    ES  = 1;    //�����ж�
    REN = 1;    //�������
    
    //UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 
    //                 0x80: P1.6 P1.7, 0xC0: P4.3 P4.4
    P_SW1 &= 0x3f;
    P_SW1 |= 0x40;  
    
    B_TX1_Busy = 0;
    g_UartData[0].RecvLength  = 0;
    g_UartData[0].SendLength  = 0;


}

void Uart2_Init(void)        //115200bps@11.0592MHz
{
    T2R   = 0;
    T2x12 = 1;
    AUXR &= ~(1<<3);
    T2H = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate3)/ 256);
    T2L = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate3)% 256);
    ET2 = 0;    //��ֹ�ж�
    T2R = 1;

    S2CON = (S2CON & 0x3f) | 0x40;
    
    PS2  = 0;    //�жϸ����ȼ�
    PS2H = 0;
    
    ES2   = 1;
    S2REN = 1;
    P_SW2 &= ~0x01; 

    B_TX2_Busy = 0;
    g_UartData[1].RecvLength  = 0;
    g_UartData[1].SendLength  = 0;
        
}

//�ض���Printf
char putchar(char c )
{
    SBUF = c;
    while(!TI);
    TI = 0;
    return c;
}


void UART1_ISR (void) interrupt 4
{
    if(RI)
    {
        RI = 0;
        g_UartData[0].RecvBuff[g_UartData[0].RecvLength] = SBUF;
        g_UartData[0].Timer = 0;
        
        if(++g_UartData[0].RecvLength >= MAX_LENGTH)   
        {
            g_UartData[0].RecvLength  = 0;
        }
    }

    if(TI)
    {
        TI = 0;
        B_TX1_Busy = 0;
    }
}



void UART2_ISR (void) interrupt 8
{
    if(S2RI)
    {
        S2RI = 0;
        g_UartData[1].RecvBuff[g_UartData[1].RecvLength] = S2BUF;
        g_UartData[1].Timer = 0;
        
        if(++g_UartData[1].RecvLength >= MAX_LENGTH)   
        {
            g_UartData[1].RecvLength  = 0;
        }
    }

    if(S2TI)
    {
        S2TI = 0;
        B_TX2_Busy = 0;
    }
}



//UART1 ���ʹ�������
void UART1_SendData(char dat)
{
    ES=0;            //�ش����ж�
    SBUF=dat;            
    while(TI!=1);    //�ȴ����ͳɹ�
    TI=0;            //��������жϱ�־
    ES=1;            //�������ж�
}

//UART1 �����ַ���
void UART1_SendString(char *s)
{
    while(*s)//����ַ���������
    {
        UART1_SendData(*s++);//���͵�ǰ�ַ�
    }
}

void Uart1Send(u8 *buf, u8 len)
{
    u8 i;
    for (i=0;i<len;i++)     
    {
        SBUF = buf[i];
        B_TX1_Busy = 1;
        while(B_TX1_Busy);
    }
}

void Uart2Send(u8 *buf, u8 len)
{
    u8 i;
    for (i=0;i<len;i++)     
    {
        S2BUF = buf[i];
        B_TX2_Busy = 1;
        while(B_TX2_Busy);
    }  
}


void Clear_Uart1_Buf()
{
    memset(g_UartData[0].RecvBuff,0,MAX_LENGTH);
    g_UartData[0].RecvLength = 0;
    g_UartData[0].Timer = 0;
}


void ClearUart1Buf()
{
    memset(RX1_Buffer,0,MAX_LENGTH);
    RX1_Cnt = 0;
}


void ClearUart2Buf()
{
    memset(RX2_Buffer,0,MAX_LENGTH);
    RX2_Cnt = 0;
}

void Clear_Uart2_Buf()
{
    memset(g_UartData[1].RecvBuff,0,MAX_LENGTH);
    g_UartData[1].RecvLength = 0;
    g_UartData[1].Timer = 0;
}


void Uart1Hnd()
{
    if (g_UartData[0].Timer > 20)
    {
        //Rx1_Timer = 0;
        //g_UartData[0].RecvLength = RX1_Cnt;
        g_UartData[0].Timer = 0;
        //memcpy(&g_UartData[0].RecvBuff,RX1_Buffer,RX1_Cnt);
        HndPcFrame();
        //ClearUart1Buf();
    }
}


void Uart2Hnd()
{
    u16 i = 0;
    if (g_UartData[1].Timer > 60)
    {
        g_UartData[1].Timer = 0;

        //DumpCmd(RX2_Buffer, RX2_Cnt);
        //printf("����Uart2\r\n");
        //g_UartData[1].RecvLength = RX2_Cnt;
        
        //printf("g_UartData[1].LEN = %d\r\n",g_UartData[1].RecvLength);
        //PrintData(g_UartData[1].RecvBuff,(u8)g_UartData[1].RecvLength);
        //memcpy(g_UartData[1].RecvBuff,RX2_Buffer,RX2_Cnt);
        //HndPcFrame();
        //ClearUart2Buf();

    }
}


