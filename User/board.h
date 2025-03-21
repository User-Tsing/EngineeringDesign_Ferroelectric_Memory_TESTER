#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN Includes */

#include "main.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "usartuser.h"
#include "spiuser.h"
#include "stm32f3xx_hal_spi.h"

// ͷ�ļ�������û�����⣬�����޸ĺϲ���һ��

/* USER CODE END Includes */

void board_Init(void);

/* USER CODE BEGIN */

typedef struct{
    uint32_t sys_time; // ϵͳʱ��
    uint8_t tick_step; // ����ʱ�ӣ�����һ��ʱ��ִ��ĳ�β���
    uint16_t timer_1; // ��ʱ��
    uint32_t sys_slow;
}sys_type; // �ṹ��sys��ʱ��ʹ�ã��������Ҫ���ٸ�

extern sys_type sys;

void board_ticks_handle(void); // ������ѭ������ã�ÿ�ε���������������ִ��һ��
void board_quick(void);
void receive_String_Handle(void); // �¼�������������
void receive_String_Handle_devided(void);
void check_data_1(void);
void check_data_1_Beta(void);
void to_tired_test(void);
void board_slow(void);
void to_get_data_devided(void);
void clc_array(void);
void to_check_flag(void);
void all_in_transmit(void);
uint8_t acsii_translate_front(uint8_t aa);
uint8_t acsii_translate_behind(uint8_t aa);
uint8_t get_int_size(uint16_t num);
void get_ascii_from_int(uint8_t* numascii, uint16_t num, uint8_t numsize);
void turn_off_spi_electric(void);
void turn_on_spi_electric(void);
void clc_screen(void);
void zeros_to_trans(void);
void ones_to_trans(void);
void to_tired_test_zeros_ones(void);
void zeros_to_trans_tired(void);
void ones_to_trans_tired(void);
void cut_off_zeros_ones(void);
void trans_to_check_with_addr(void);
void receive_to_check_with_addr(void);
void zeros_ones_trans_report(void);
void cut_off_test_by_screan_ones(void);
void cut_off_test_by_screan_zeros(void);

/* USER CODE END */

#ifdef __cplusplus
}
#endif

#endif