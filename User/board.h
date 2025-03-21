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

// 头文件交叉调用会出问题，建议修改合并成一个

/* USER CODE END Includes */

void board_Init(void);

/* USER CODE BEGIN */

typedef struct{
    uint32_t sys_time; // 系统时钟
    uint8_t tick_step; // 步进时钟，用于一定时间执行某次操作
    uint16_t timer_1; // 计时用
    uint32_t sys_slow;
}sys_type; // 结构体sys，时钟使用，具体参数要加再改

extern sys_type sys;

void board_ticks_handle(void); // 主函数循环里调用，每次调用里面所有内容执行一遍
void board_quick(void);
void receive_String_Handle(void); // 事件处理：接收数据
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