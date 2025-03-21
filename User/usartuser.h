#ifndef __USARTUSER_H
#define __USARTUSER_H

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN Includes */

#include "main.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "board.h"
#include "spiuser.h"

/* USER CODE END Includes */

void usartuser_Init(void);

/* USER CODE BEGIN */

typedef struct
{
    ErrorStatus uart_Receive_String;
    ErrorStatus uart_to_check_spi;
    ErrorStatus data_check_flag;
    ErrorStatus tired_test;
    ErrorStatus tired_test_by_screen; // ���ʹ�� FunctionalState
    ErrorStatus cut_off_test;
    ErrorStatus receiveforward;
    ErrorStatus devided_data_get;
    ErrorStatus devided_receive_data;
    ErrorStatus cut_off_continue;
    ErrorStatus clc_order;
    ErrorStatus check_flag_on;
    ErrorStatus all_to_transmit;
    ErrorStatus all_to_transmit_start;
    ErrorStatus spi_to_change;
    ErrorStatus zeros_to_test;
    ErrorStatus ones_to_test;
    ErrorStatus zeros_test; // ȫ����Ա�־λ��20250220����
    ErrorStatus ones_test; // ȫһ���Ա�־λ��20250220����
    ErrorStatus zeros_trans; // ȫ����Ա�־λ��20250220����
    ErrorStatus ones_trans; // ȫһ���Ա�־λ��20250220����
    ErrorStatus zeros_to_test_tired;
    ErrorStatus ones_to_test_tired;
    ErrorStatus zeros_test_tired; // ȫ����Ա�־λ��20250220����
    ErrorStatus ones_test_tired; // ȫһ���Ա�־λ��20250220����
    ErrorStatus zeros_trans_tired; // ȫ����Ա�־λ��20250220����
    ErrorStatus ones_trans_tired; // ȫһ���Ա�־λ��20250220����
    ErrorStatus to_tired_test;
    ErrorStatus zeros_tired_finish;
    ErrorStatus ones_tired_finish;
    ErrorStatus tired_trans_report;
    ErrorStatus cut_off_to_continue_next;
    ErrorStatus cut_off_continue_next;
    ErrorStatus zeros_ones_report;
    ErrorStatus cut_off_to_zeros;
    ErrorStatus cut_off_to_ones;
    ErrorStatus zeros_ones_report_cut;
    ErrorStatus all_correct_normal;
    ErrorStatus all_correct_cut_off;
    
    ErrorStatus trans_with_addr; // �������Զ����ַ�շ���ַ
    ErrorStatus receive_with_addr;
    union{
        uint16_t addr_16[1];
        uint8_t addr_8[2];
    }get_addr_set;
    uint8_t get_addr_set_high;
    
    uint8_t RXBuffer[270]; // ���ڽ��ջ��������������ɣ����ܽ��մ��ļ�������������������ȷ����������գ�
    uint8_t Rx_current_step; // �����жϵ�ǰ����
    uint8_t Rx_step_buf[3]; // ���ڽ��գ������м�վ
    uint8_t getArray[270]; // �������ݱ���
    uint8_t compareArray[270];
    uint8_t devided_data[256];
    uint8_t to_transfer_all[520];
    uint8_t devided_data_num;
    uint8_t spi_changer;
    uint8_t report_step;
    uint16_t contentLength; // �����ַ�����
    uint16_t Rx_Count; // ���ڽ��ռ���
    uint16_t cut_off_time;
    uint16_t get_data_all_in;
    uint32_t uart_time; // ���ڽ���ʱ��
    
    uint16_t address_spi;
    uint8_t address_spi_high;
    
    union
    {
        uint16_t num_of_error_16[1];
        uint8_t num_of_error_8[2];
    }num_of_error; // �������
    union
    {
        uint16_t num_of_test_16[1];
        uint8_t num_of_test_8[2];
    }num_of_test; // ���Դ���
    union
    {
        uint16_t num_of_test_set_16[1];
        uint8_t num_of_test_set_8[2];
    }num_of_test_set; // �趨���Դ���
    union
    {
        uint16_t RX_Num_16[1];
        uint8_t Rx_Num_8[2];
    }RX_Num; // ������������������У��λ��
    union
    {
        uint64_t trans64[32];
        uint32_t trans32[63];
        uint16_t trans16[128];
        uint8_t trans8[256];
    }to_trans;
    union
    {
        uint16_t cutdown_16[1];
        uint8_t cutdown_8[2];
    }cut_down_time;
    union
    {
        uint16_t datanum_16[1];
        uint8_t datanum_8[2];
    }devided_datanum;
    
    uint8_t zeros_buffer[2048]; // ȫ�㷢������
    uint8_t ones_buffer[2048]; // ȫһ��������
    uint8_t spi_rx_buffer[2048];
    uint8_t error_array[64];
    uint8_t zeros_ones_range_num;
    
    union{
        uint16_t error_16[64];
        uint8_t error_8[128];
    }error_rate_in_tired;
    
}usart_type;

typedef struct
{
    ErrorStatus clear_all;
    ErrorStatus clear_end;
    ErrorStatus trans;
    ErrorStatus to_show_receive;
    ErrorStatus to_show;
    uint8_t show_receive;
    uint8_t step;
}screen_type;

void arrayCopy(uint8_t* aa, uint8_t* bb, uint16_t num);
void usart_finish_ithandle(UART_HandleTypeDef *huart);
void screen_trans(void);

extern usart_type uart1;
extern usart_type uart2;
extern screen_type screen;

/* USER CODE END */

#ifdef __cplusplus
}
#endif

#endif