#ifndef __ATK_NCR_H
#define __ATK_NCR_H
#include "stdint.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//
//��������ĸʶ�������ALIENTEK�ṩ,�����ṩ2��LIB,�����ʹ��
//ATKNCR_M_Vx.x.lib��ATKNCR_N_Vx.x.lib��Ψһ�������Ƿ�ʹ�ö�̬�ڴ����.
//����:M,������Ҫ�õ�malloc�İ汾,����ʵ��alientek_ncr_malloc��alientek_ncr_free��������
//     N,������ͨ�汾,����Ҫʵ��alientek_ncr_malloc��alientek_ncr_free��������
//     Vx.x,����ǰʶ�����İ汾.		 	  
//����:֧������/Сд��ĸ/��д��ĸ/�������ʶ��ģʽ.		  
//��ʶ�����ʹ�������൱��.					   
//��һ��:����alientek_ncr_init����,��ʼ��ʶ�����
//�ڶ���:��ȡ����ĵ�������(������2�������ϵĲ�ͬ������������)
//������:����alientek_ncr����,�õ�ʶ����.
//���Ĳ�:�������Ҫ��ʶ��,�����alientek_ncr_stop����,��ֹʶ��.�������Ҫ����,���ظ�2,3������.		   
//�汾:V2.0
//����ԭ��@ALIENTEK
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved													    								  
//��Ȩ���У�����ؾ���
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��ʹ��ATKNCR_M_Vx.x.lib��ʱ��,����Ҫ���ATK_NCR_TRACEBUF1_SIZE��ATK_NCR_TRACEBUF2_SIZE
//��ʹ��ATKNCR_N_Vx.x.lib��ʱ��,�������ʶ������,���ʵ�����ATK_NCR_TRACEBUF1_SIZE��ATK_NCR_TRACEBUF2_SIZE��ֵ
#define ATK_NCR_TRACEBUF1_SIZE	500*4  	//�����һ��tracebuf��С(��λΪ�ֽ�),�����������,��Ѹ������ʵ��Ĵ�
#define ATK_NCR_TRACEBUF2_SIZE	250*4	//����ڶ���tracebuf��С(��λΪ�ֽ�),�����������,��Ѹ������ʵ��Ĵ�

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����켣��������
//__packed typedef struct _atk_ncr_point		// ARM_compiler_v5
typedef struct __attribute__((packed)) _atk_ncr_point		// ARM_compiler_v6
{
	short x; 	//x������
	short y;	//y������
}atk_ncr_point;		

// recognize mode
#define RECOGNIZE_123		1
#define RECOGNIZE_ABC		2
#define RECOGNIZE_abc		3
#define RECOGNIZE_Aa1		4
// output number
#define CHAR_NUM	6
// max point to record
#define MAX_POINT		1000
// alientek_ncr_init() malloc HEAP size for ATKNCR_M_V2.0.lib
//#define ATK_INIT_HEAP_SIZE	(512+512+512+160+64+64+496)		// 2320Bytes; info source: LOG from alientek_ncr_malloc()
extern atk_ncr_point draw_coor[MAX_POINT];
extern volatile uint16_t point_num;
extern char result[CHAR_NUM+1];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ⲿ���ú���
//��ʼ��ʶ����
//����ֵ:0,��ʼ���ɹ�
//       1,��ʼ��ʧ��
unsigned char alientek_ncr_init(void);
//ֹͣʶ����
void alientek_ncr_stop(void);

//ʶ����ʶ��
//track:������󼯺� 
//potnum:�������ĵ���,����track�Ĵ�С
//charnum:��������Ľ����,������ϣ��������ٸ�ƥ����
//mode:ʶ��ģʽ
//1,��ʶ������
//2,��ʶ���д��ĸ
//3,��ʶ��Сд��ĸ
//4,���ʶ��(ȫ��ʶ��)
//result:���������(����Ϊ:charnum+1���ֽ�)	
/** @note: the track will be changed! */
void alientek_ncr(atk_ncr_point * track,int potnum,int charnum,unsigned char mode,char*result);

//�ڴ����ú��� 
void alientek_ncr_memset(char *p,char c,unsigned long len);	
//��̬�����ڴ�,��ʹ��ATKNCR_M_Vx.x.libʱ,����ʵ��.
void *alientek_ncr_malloc(unsigned int size);			 
//��̬�ͷ��ڴ�,��ʹ��ATKNCR_M_Vx.x.libʱ,����ʵ��.
void alientek_ncr_free(void *ptr);						 	    
#endif



