#ifndef __SPIUSER_H
#define __SPIUSER_H

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
#include "spi.h"
#include "stm32f3xx_hal_spi.h"

/* USER CODE END Includes */

void spiuser_Init(void);

/* USER CODE BEGIN */

typedef struct
{
    uint8_t pres; // 波特率分频倍数
    uint8_t devided_addrNum; // 有几个地址？
    uint8_t spiReader[270]; // SPI读取结果     
    uint8_t devided_data[128];
    uint16_t devided_addrLength[512]; // 地址长度
    uint16_t devided_addr[512]; // 写入地址，可写多个
    uint16_t next_addr;
    union
    {
        uint64_t tired_test_string_64[1];
        uint8_t tired_test_string_8[8];
    }tired_test_string;
    union
    {
        uint64_t tired_test_receive_64[1];
        uint8_t tired_test_receive_8[8];
    }tired_test_receive;
    ErrorStatus transmit;
    ErrorStatus receive;
    ErrorStatus tired_test_Read;
    ErrorStatus cut_electric;
    ErrorStatus electric_turn_on;
    ErrorStatus electric_turn_off;
}spi_type;

void enable_SPI(void);

void disable_SPI(void);

void SPI_Write_Flash(uint8_t* buffer, uint16_t length, uint16_t addr);

void SPI_Read_Flash(uint8_t* inbuffer, uint16_t length, uint16_t addr);

void setSPIRate(uint32_t prescaler);

void multi_infor_relay(UART_HandleTypeDef *huart, uint16_t length, uint16_t address);

extern spi_type spi1;

void read_ID(void);

void SPI_WRSR(void);

void SPI_FastRead(uint8_t* inbuffer, uint16_t length, uint16_t addr);

void SPI_Rate_changer(uint32_t prescaler);

void SPI_Write_zeros_ones(uint8_t* buffer, uint16_t length, uint16_t addr, uint8_t addr_high); // 全零全一测试用，添加了地址最高位

void SPI_Read_zeros_ones(uint8_t* inbuffer, uint16_t length, uint16_t addr, uint8_t addr_high);

uint8_t ReadSR();

/* USER CODE END */

#ifdef __cplusplus
}
#endif

#endif