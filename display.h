#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <STC15F2K60S2.h>
#include "sys_cfg.h"



void Busy() ;
void Write_Data(uchar k) ;
void Write_Cmd(uchar cmd) ;
void ShowQQChar(uchar addr,uchar *english,uchar count);//DDRAM��ַ���������ֽ�ָ�룬���ݳ���
void Disp(uchar y,uchar x,uchar i,uchar *z);//�ַ��ͺ�����ʾ
void WRCGRAM1(uchar addr);//���»���
void WRCGRAM2(uchar addr);//˫�»���
void WRCGRAM3(uchar addr);//���»��߼ӵ�
void WRCGRAM4(uchar addr);//˫�»��߼ӵ�
void CLEARGDRAM(void);//��ͼ����
void WRGDRAM(uchar Y1,uchar clong,uchar hight,uchar *TAB1);
void Cursor(uchar x,uchar y);
void DISPIcon(uchar x,uchar y,uchar clong,uchar hight,uchar *Icon);//����ͼ��
void Ini_Lcd(void);  	 
void antibackgroud(uint hang);//ĳһ���׷�����ʾ;
void Delay_1ms(uint x);
void delayNus(uchar x);	//LCD����ʱָ�������֮�����ʱ

#endif