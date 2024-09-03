#include "atk_ncr.h"
#include "stdlib.h"
#include "string.h"

#define ATK_DBG_EN	1
#if ATK_DBG_EN
#include "dbger.h"
#define ATK_DBG(fmt, ...)		LOG_DBG(fmt, ##__VA_ARGS__)
#define ATK_ERR(fmt, ...)		LOG_ERR(fmt, ##__VA_ARGS__)
#else
#define ATK_DBG(fmt, ...)
#define ATK_ERR(fmt, ...)
#endif

volatile uint16_t point_num;
atk_ncr_point draw_coor[MAX_POINT];
char result[CHAR_NUM+1];

extern void *pvPortMalloc( size_t xWantedSize );
extern void vPortFree( void *pv );
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

//�ڴ����ú���
void alientek_ncr_memset(char *p,char c,unsigned long len) 
{
	memset(p, c, len);
}	 					  
//�ڴ����뺯��
void *alientek_ncr_malloc(unsigned int size) 
{
	void* ptr = pvPortMalloc(size);
	//ATK_DBG("ATK malloc %d bytes at 0x%08x\n", size, (uint32_t)ptr);
	if(ptr == NULL) {
		ATK_ERR("ATK malloc failed\n");
	}
	return ptr;
}
//�ڴ���պ���
void alientek_ncr_free(void *ptr) 
{
	//ATK_DBG("ATK free 0x%08x\n", (uint32_t)ptr);
	vPortFree(ptr);
}		  
		  



















