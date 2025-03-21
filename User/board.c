#include <stdio.h>
#include <string.h>
#include "board.h"

/* board.c文件，用于制作时分复用多线程 */

// 移植须知：在stm32f3xx_it中有定义

// 各项功能在此

sys_type sys; // 结构体实例，在此定义，初始化只初始化值

void board_Init(void) // 初始化函数，在主函数循环开始前调用
{
    sys.sys_time = 0x00000000; // 初始化为零，sys时钟中断加一做系统时钟
    sys.tick_step = 0x00;
}

// SPI相关处理函数定义如下
void receive_String_Handle(void) // 事件处理，接收数据
{
    if(uart1.uart_Receive_String == SUCCESS) // 上位机：检测是否接收到数据
    {
        uart1.uart_Receive_String = ERROR; // 复位标志位
        // SPI_Write_Flash(uart1.getArray, uart1.contentLength, spi1.addr[spi1.addrNum]); // SPI当场发送数据
//        SPI_Write_Flash(uart1.getArray, uart1.contentLength, 0x0000);
        SPI_Write_zeros_ones(uart1.getArray, uart1.contentLength, 0x0000, 0x00);
        // spi1.addrLength[spi1.addrNum] = uart1.contentLength; // 数据长度，预留多发多读
        // spi1.addrNum++; // 更新地址数
        // spi1.addr[spi1.addrNum] = spi1.addr[spi1.addrNum-1] + uart1.contentLength; // 新地址：原地址后面
        uart1.to_trans.trans32[0] = 0x11015AA5; // 0xA55A0111EE，联合体遵循低字节在前原则，故此这样写
        uart1.to_trans.trans8[4] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5); // 发送反馈
    }
    if(uart2.uart_Receive_String == SUCCESS) // 串口屏：检测是否接收到数据
    {
        uart2.uart_Receive_String = ERROR; // 复位标志位
        SPI_Write_Flash(spi1.devided_data, uart2.devided_data_num, 0x0000);
        // 串口屏需要显示发送内容，对应通信协议见串口屏数据手册
        // 显示英语文字需用ASCII码
        // EE B1 10 00 03 00 06 20 DD DD DD DD DD DD FF FC FF FF
        uart2.to_trans.trans64[0] = 0x200600030010B1EE; // 8字节
        uart2.to_trans.trans16[4] = 0x7830; // ASCII码：0x
        for(int i = 0;i < uart2.devided_data_num; i++)
        {
            uart2.to_trans.trans8[10 + 2 * i] = acsii_translate_front(spi1.devided_data[i]);
            uart2.to_trans.trans8[11 + 2 * i] = acsii_translate_behind(spi1.devided_data[i]);
        }
        // 至此一共有10+2*uart2.devided_data_num字节的内容
        uart2.to_trans.trans8[10 + 2 * uart2.devided_data_num] = 0xFF;
        uart2.to_trans.trans8[11 + 2 * uart2.devided_data_num] = 0xFC;
        uart2.to_trans.trans8[12 + 2 * uart2.devided_data_num] = 0xFF;
        uart2.to_trans.trans8[13 + 2 * uart2.devided_data_num] = 0xFF;
        HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 14 + 2 * uart2.devided_data_num); // 发送反馈
    }
}

// 多发多读测试版
void receive_String_Handle_devided(void) // 事件处理，拆包的接收数据
{
    if(uart1.devided_receive_data == SUCCESS)
    {
        uart1.devided_receive_data = ERROR;
        spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]] = uart1.devided_data_num;
        spi1.devided_addr[uart1.devided_datanum.datanum_16[0]] = spi1.next_addr;
        spi1.next_addr += uart1.devided_data_num;
        SPI_Write_Flash(spi1.devided_data, spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]], spi1.devided_addr[uart1.devided_datanum.datanum_16[0]]);
        uart1.to_trans.trans32[0] = 0x11015AA5; // 0xA55A0111EE，联合体遵循低字节在前原则，故此这样写
        uart1.to_trans.trans8[4] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5); // 发送反馈
    }
}

void to_get_data_devided(void) // 接收数据，拆包版
{
    if(uart1.devided_data_get == SUCCESS)
    {
        uart1.devided_data_get = ERROR;
//        uart1.to_trans.trans8[0] = 0xA5;
//        uart1.to_trans.trans8[1] = 0x5A;
//        uart1.to_trans.trans8[2] = spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]] + 2;
//        uart1.to_trans.trans8[3] = 0x03;
//        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 4);
        uint8_t to_transfer[520];
        to_transfer[0] = 0xA5;
        to_transfer[1] = 0x5A;
        to_transfer[2] = spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]] + 2;
        to_transfer[3] = 0x03;
        SPI_Read_Flash(spi1.spiReader, spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]], spi1.devided_addr[uart1.devided_datanum.datanum_16[0]]);
//        HAL_UART_Transmit_DMA(&huart1, spi1.spiReader, spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]]);
//        uart1.to_trans.trans8[0] = spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]];
//        uart1.to_trans.trans8[1] = 0xEE;
//        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 2);
        memcpy(to_transfer + 4, spi1.spiReader, spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]]);
        to_transfer[spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]] + 4] = spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]];
        to_transfer[spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]] + 5] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, to_transfer, spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]] + 6); // 测试：带指令发送
    }
}

void to_read_spi(void) // 事件处理，读取SPI并检测，只读最新的
{
    if(uart1.uart_to_check_spi == SUCCESS)
    {
        uart1.uart_to_check_spi = ERROR;
        // SPI_Read_Flash(spi1.spiReader, spi1.addrLength[spi1.addrNum -1], spi1.addr[spi1.addrNum -1]); // 读取内容
        SPI_Read_Flash(spi1.spiReader, uart1.contentLength, 0x0000);
        // spi1.addrNum--; // 读取即丢失
        check_data_1();
    }
}

void check_data_1(void) // 检查内容是否相同，特殊函数特别使用
{
    // for(int i = 0; i < spi1.addrLength[spi1.addrNum]; i++)
    uart1.data_check_flag = SUCCESS;
    for(int i = 0; i < uart1.contentLength; i++)
    {
        if(spi1.spiReader[i] != uart1.getArray[i])
        {
            uart1.data_check_flag = ERROR;
            uart1.to_trans.trans32[0] = 0x01015AA5;
            uart1.to_trans.trans8[4] = 0xEE;
            HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5); // DMA发送上位机
            break;
        }
    }
    if(uart1.data_check_flag == SUCCESS) // 读取结果相同
    {
        uart1.to_trans.trans32[0] = 0x00015AA5;
        uart1.to_trans.trans8[4] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5); // DMA发送上位机
    }
}

void check_data_1_Beta(void) // 检查内容是否相同，特殊函数特别使用
{
    for(int i = 4; i < spi1.devided_addrLength[spi1.devided_addrNum] - 1; i++) // 只看中间部分
    {
        if(spi1.spiReader[i] != uart1.getArray[i])
        {
            uart1.compareArray[i] = 0x01;
        }
        else
        {
            uart1.compareArray[i] = 0x00;
        }
    }
    uart1.to_trans.trans8[0] = 0xA5;
    uart1.to_trans.trans8[1] = 0x5A;
    uart1.to_trans.trans8[2] = spi1.devided_addrLength[spi1.devided_addrNum];
    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 3);
    HAL_UART_Transmit_DMA(&huart1, uart1.compareArray, spi1.devided_addrLength[spi1.devided_addrNum] - 5);
    uart1.to_trans.trans8[0] = 0xEE;
    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 1);
}

void receive_forward(void) // 存储转发
{
    if(uart1.receiveforward == SUCCESS)
    {
        uart1.receiveforward = ERROR;
//        SPI_Read_Flash(spi1.spiReader, uart1.contentLength, 0x0000);
        SPI_Read_zeros_ones(spi1.spiReader, uart1.contentLength, 0x0000, 0x00);
        HAL_UART_Transmit_DMA(&huart1, spi1.spiReader, uart1.contentLength);
    }
    if(uart2.receiveforward == SUCCESS)
    {
        uart2.receiveforward = ERROR;
        SPI_Read_Flash(spi1.spiReader, uart2.devided_data_num, 0x0000); // 这两个参数仅在发送数据时更新，可以沿用
        uart2.data_check_flag = SUCCESS;
        for(int i = 0; i < uart2.devided_data_num; i++) // 检测数据
        {
            if(spi1.spiReader[i] != spi1.devided_data[i])
            {
                uart2.data_check_flag = ERROR;
                break;
            }
        }
        if(uart2.data_check_flag == SUCCESS)
        {
            // EE B1 12 00 03 00 07 00 07 20 30 78 46 46 46 46 00 08 00 08 C4 DA C8 DD D5 FD C8 B7 FF FC FF FF ，仅限2字节数据
            
            // EE B1 10 00 03 00 07 30 78 46 46 46 46 FF FC FF FF 
            uart2.to_trans.trans64[0] = 0x300700030010B1EE;
            uart2.to_trans.trans8[8] = 0x78; // ASCII码：0x
            for(int i = 0;i < uart2.devided_data_num; i++)
            {
                uart2.to_trans.trans8[9 + 2 * i] = acsii_translate_front(spi1.devided_data[i]);
                uart2.to_trans.trans8[10 + 2 * i] = acsii_translate_behind(spi1.devided_data[i]);
            }
            // 至此一共有11+2*uart2.devided_data_num字节的内容
            uart2.to_trans.trans8[9 + 2 * uart2.devided_data_num] = 0xFF;
            uart2.to_trans.trans8[10 + 2 * uart2.devided_data_num] = 0xFC;
            uart2.to_trans.trans8[11 + 2 * uart2.devided_data_num] = 0xFF;
            uart2.to_trans.trans8[12 + 2 * uart2.devided_data_num] = 0xFF;
            screen.show_receive = 1;
            screen.to_show_receive = SUCCESS;
        }
        else
        {
            // EE B1 12 00 03 00 07 00 06 30 78 31 32 33 34 00 08 00 08 C4 DA C8 DD B4 ED CE F3 FF FC FF FF 
            
            // EE B1 10 00 03 00 07 30 78 46 46 46 46 FF FC FF FF
            uart2.to_trans.trans64[0] = 0x300700030010B1EE;
            uart2.to_trans.trans8[8] = 0x78; // ASCII码：0x
            for(int i = 0;i < uart2.devided_data_num; i++)
            {
                uart2.to_trans.trans8[9 + 2 * i] = acsii_translate_front(spi1.devided_data[i]);
                uart2.to_trans.trans8[10 + 2 * i] = acsii_translate_behind(spi1.devided_data[i]);
            }
            // 至此一共有9+2*uart2.devided_data_num字节的内容
            uart2.to_trans.trans8[9 + 2 * uart2.devided_data_num] = 0xFF;
            uart2.to_trans.trans8[10 + 2 * uart2.devided_data_num] = 0xFC;
            uart2.to_trans.trans8[11 + 2 * uart2.devided_data_num] = 0xFF;
            uart2.to_trans.trans8[12 + 2 * uart2.devided_data_num] = 0xFF;
            screen.show_receive = 2;
            screen.to_show_receive = SUCCESS;
        }
        HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 13 + 2 * uart2.devided_data_num);
    }
}

void all_in_transmit(void) // 全部发送
{
    if(uart1.all_to_transmit_start == SUCCESS)
    {
        uart1.all_to_transmit_start = ERROR;
        uart1.all_to_transmit = SUCCESS;
        uart1.to_transfer_all[0] = 0xA5;
        uart1.to_transfer_all[1] = 0x5A;
        uart1.to_transfer_all[2] = spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]] + 2;
        uart1.to_transfer_all[3] = 0x03;
        SPI_Read_Flash(spi1.spiReader, spi1.devided_addrLength[uart1.get_data_all_in], spi1.devided_addr[uart1.get_data_all_in]);
        memcpy(uart1.to_transfer_all + 4, spi1.spiReader, spi1.devided_addrLength[uart1.get_data_all_in]);
        uart1.to_transfer_all[spi1.devided_addrLength[uart1.get_data_all_in] + 4] = spi1.devided_addrLength[uart1.get_data_all_in];
        uart1.to_transfer_all[spi1.devided_addrLength[uart1.get_data_all_in] + 5] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_transfer_all, spi1.devided_addrLength[uart1.get_data_all_in] + 6);
    }
}

void to_change_spi(void) // 修改spi传输速率
{
    if(uart1.spi_to_change == SUCCESS)
    {
        uart1.spi_to_change = ERROR;
        uart1.to_trans.trans32[0] = 0x81045AA5;
        uart1.to_trans.trans16[2] = 0x0000;
        switch(uart1.spi_changer)
        {
            case 0x01: // 预分频器：4， 18Mbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_4);
                uart1.to_trans.trans16[2] = 0x0400;
                break;
            case 0x02: // 预分频器：8， 9Mbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_8);
                uart1.to_trans.trans16[2] = 0x0800;
                break;
            case 0x03: // 预分频器：16， 4.5Mbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_16);
                uart1.to_trans.trans16[2] = 0x1000;
                break;
            case 0x04: // 预分频器：32， 2.25Mbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_32);
                uart1.to_trans.trans16[2] = 0x2000;
                break;
            case 0x05: // 预分频器：64， 1.125Mbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_64);
                uart1.to_trans.trans16[2] = 0x4000;
                break;
            case 0x06: // 预分频器：128， 562.5Kbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_128);
                uart1.to_trans.trans16[2] = 0x8000;
                break;
            case 0x07: // 预分频器：256， 281.25Kbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_256);
                uart1.to_trans.trans16[2] = 0x0001;
                break;
        }
        uart1.to_trans.trans16[3] = 0xEE02;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 8);
    }
}

void check_clc(void) // 检查清理spi
{
    if(uart1.clc_order == SUCCESS)
    {
        uart1.clc_order = ERROR;
        clc_array();
    }
}

void to_tired_test(void) // 疲劳测试处理，主函数循环使用
{
    
    if(uart2.tired_test == SUCCESS)
    {
        if(spi1.tired_test_Read == ERROR)
        {
            SPI_Write_Flash(spi1.tired_test_string.tired_test_string_8, 8, 0x0000); // 发送数据
            spi1.tired_test_Read = SUCCESS;
        }
        else
        {
            SPI_Read_Flash(spi1.tired_test_receive.tired_test_receive_8, 8, 0x0000);
            if(spi1.tired_test_string.tired_test_string_64[0] != spi1.tired_test_receive.tired_test_receive_64[0]) // 不同即计数
            {
                uart2.num_of_error.num_of_error_16[0]++;
            }
            spi1.tired_test_Read = ERROR;
        }
        uart2.num_of_test.num_of_test_16[0]++;
        if(uart2.num_of_test.num_of_test_16[0] == uart2.num_of_test_set.num_of_test_set_16[0])
        {
            // EE B1 52 00 02 00 01 30 3B 30 3B FF FC FF FF 
            uart2.to_trans.trans64[0] = 0x300100020052B1EE;
            uart2.to_trans.trans8[8] = 0x3B;
            uint16_t number = uart2.num_of_error.num_of_error_16[0]; // 转换成整型
            uint8_t number_size = get_int_size(number);
            uint8_t num_ascii[3];
            get_ascii_from_int(num_ascii, number, number_size);
            for(int i = 0; i < number_size; i++)
            {
                uart2.to_trans.trans8[9 + i] = num_ascii[i];
            }
            // 至此共9+number_size位
            uart2.to_trans.trans8[9 + number_size] = 0x3B;
            uart2.to_trans.trans8[10 + number_size] = 0xFF;
            uart2.to_trans.trans8[11 + number_size] = 0xFC;
            uart2.to_trans.trans8[12 + number_size] = 0xFF;
            uart2.to_trans.trans8[13 + number_size] = 0xFF;
            uart2.tired_test_by_screen = SUCCESS;
            HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 14 + number_size); // 发送反馈
            uart2.num_of_error.num_of_error_16[0] = 0;
            uart2.num_of_test.num_of_test_16[0] = 0;
        }
    }
}


void clc_array(void) // 清理spi寄存器和数组
{
    for(int i = 0; i < 512; i++)
    {
        spi1.devided_addr[i] = 0;
        spi1.devided_addrLength[i] = 0;
    }
    spi1.next_addr = 0x0000;
}

void clc_screen(void) // 显示屏清屏
{
    if(screen.clear_all == SUCCESS)
    {
        screen.step = 0;
        screen.clear_all = ERROR;
        screen.clear_end = ERROR; // 调用Tx
        // EE B1 53 00 02 00 01 FF FC FF FF   0号：表格清零
        uart2.to_trans.trans64[0] = 0xFF0100020053B1EE;
        uart2.to_trans.trans16[4] =0xFFFC;
        uart2.to_trans.trans8[10] =0xFF;
        HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
        screen.step++;
    }
}

void to_check_flag(void) // 检查校验位，处理操作
{
    if(uart1.check_flag_on == SUCCESS)
    {
        uart1.check_flag_on = ERROR;
        uart1.to_trans.trans32[0] = 0x12015AA5;
        uart1.to_trans.trans8[4] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5);
    }
}

// 通用函数如下定义

uint8_t acsii_translate_front(uint8_t aa) // 十六进制码转ASCII码，前四位
{
    uint8_t mid_one = (aa >> 4) & 0x0F; // 右移四位提取出高四位
    uint8_t trans_result;
    if(mid_one <= 0x09) // 十六进制有0-9，A-F，ASCII码有偏移，分类讨论
    {
        trans_result = mid_one + 0x30; // 数字类ASCII码
    }
    else
    {
        trans_result = mid_one - 10 + 'A'; // 字母类ASCII码（'A' = 0x41）
    }
    return trans_result;
}
uint8_t acsii_translate_behind(uint8_t aa) // 十六进制码转ASCII码，后四位
{
    uint8_t mid_one = aa & 0x0F; // 提取第四位
    uint8_t trans_result;
    if(mid_one <= 0x09) // 十六进制有0-9，A-F，ASCII码有偏移，分类讨论
    {
        trans_result = mid_one + 0x30; // 数字类ASCII码
    }
    else
    {
        trans_result = mid_one - 10 + 'A'; // 字母类ASCII码（'A' = 0x41）
    }
    return trans_result;
}

uint8_t get_int_size(uint16_t num) // 整型位数
{
    uint8_t i = 0;
    if(num == 0)
    {
        return 1;
    }
    while(num != 0)
    {
        num = num / 10;
        i++;
    }
    return i;
}

void get_ascii_from_int(uint8_t* numascii, uint16_t num, uint8_t numsize) // 从整型获取ascii码，高位在前
{
    while(numsize > 0)
    {
        numascii[numsize-1] = (num % 10) + 0x30;
        numsize--;
        num = num / 10;
    }
    // return numascii;
}

// 断电专用函数如下定义
void turn_off_spi_electric(void)
{
    if(spi1.electric_turn_on == SUCCESS)
    {
        spi1.electric_turn_on = ERROR;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); // MOS管执行断电
        spi1.cut_electric = SUCCESS;
        // EE B1 10 00 04 00 06 20 B6 CF B5 E7 FF FC FF FF
        uart2.to_trans.trans64[0] = 0x200600040010B1EE;
        uart2.to_trans.trans64[1] = 0xFFFFFCFFE7B5CFB6; // 16字节
        HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 16); // 显示屏显示状态
    }
}

void turn_on_spi_electric(void)
{
    if(spi1.electric_turn_off == SUCCESS)
    {
        spi1.electric_turn_off = ERROR;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); // MOS管执行断电
        spi1.cut_electric = ERROR;
        // EE B1 10 00 04 00 06 20 D5 FD B3 A3 FF FC FF FF
        uart2.to_trans.trans64[0] = 0x200600040010B1EE;
        uart2.to_trans.trans64[1] = 0xFFFFFCFFA3B3FDD5; // 16字节
        HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 16); // 显示屏显示状态
    }
}

// 全零全一函数（更新）
void zeros_to_trans(void)
{
    if(uart1.zeros_to_test == SUCCESS) // 开始测试信号
    {
        uart1.zeros_to_test = ERROR;
        uart1.zeros_test = SUCCESS;
        uart1.address_spi = 0x0000; // 初始地址
        uart1.address_spi_high = 0x00; // 初始高地址
        uart1.zeros_ones_range_num = 0;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // 错误矩阵更新
        }
    }
    if(uart1.zeros_test == SUCCESS)
    {
        if(uart1.zeros_trans == SUCCESS) // 阴：发送数据
        {
            uart1.zeros_trans = ERROR; // 一共2^17地址，一次发2^12个，共分32模块
            SPI_Write_zeros_ones(uart1.zeros_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI发送
        }
        else // 阳：接收数据
        {
            uart1.zeros_trans = SUCCESS;
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI接收
            
            for(int i = 0; i < 2048; i++) // 比对数据
            {
                if(uart1.spi_rx_buffer[i] != 0x00) // 不全零记录错误点
                {
                    uart1.error_array[uart1.zeros_ones_range_num]++; // 错误数+1
                    if(uart1.error_array[uart1.zeros_ones_range_num] == 0xFF) // 设定原因，最大满了就不算后面的了
                    {
                        break;
                    }
                }
            }
            
            uart1.zeros_ones_range_num++;
            
            if(uart1.address_spi_high == 0x00) // 迭代
            {
                if(uart1.address_spi == 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart1.address_spi = 0; // 从头开始
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
            }
            else // 到底了
            {
                if(uart1.address_spi == 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.zeros_test = ERROR; // 结束迭代
                    uart1.to_trans.trans8[0] = 0xA5;
                    uart1.to_trans.trans8[1] = 0x5A;
                    uart1.to_trans.trans8[2] = 0x42; // 66
                    uart1.to_trans.trans8[3] = 0x15; // 指令号（暂定）
                    for(int i = 0; i < 64; i++)
                    {
                        uart1.to_trans.trans8[4 + i] = uart1.error_array[i]; // 提取到发送数组
                    }
                    uart1.to_trans.trans8[68] = 0x40;
                    uart1.to_trans.trans8[69] = 0xEE;
                    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 70); // 一次发送
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
            }
        }
    }
    
    
    if(uart2.zeros_to_test == SUCCESS) // 开始测试信号
    {
        uart2.zeros_to_test = ERROR;
        uart2.zeros_test = SUCCESS;
        uart2.address_spi = 0x0000; // 初始地址
        uart2.address_spi_high = 0x00; // 初始高地址
        uart2.zeros_ones_range_num = 0;
        uart2.all_correct_normal = SUCCESS;
        for(int i = 0; i < 64; i++)
        {
            uart2.error_array[i] = 0; // 错误矩阵更新
        }
    }
    if(uart2.zeros_test == SUCCESS)
    {
        if(uart2.zeros_trans == SUCCESS) // 阴：发送数据
        {
            uart2.zeros_trans = ERROR; // 一共2^17地址，一次发2^12个，共分32模块
            SPI_Write_zeros_ones(uart2.zeros_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI发送
        }
        else // 阳：接收数据
        {
            uart2.zeros_trans = SUCCESS;
            SPI_Read_zeros_ones(uart2.spi_rx_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI接收
            
            for(int i = 0; i < 2048; i++) // 比对数据
            {
                if(uart2.spi_rx_buffer[i] != 0x00) // 不全零记录错误点
                {
                    uart2.error_array[uart2.zeros_ones_range_num]++; // 错误数+1
                    if(uart2.error_array[uart2.zeros_ones_range_num] == 0xFF) // 设定原因，最大满了就不算后面的了
                    {
                        break;
                    }
                }
                if(uart2.error_array[uart2.zeros_ones_range_num] != 0)
                {
                    uart2.all_correct_normal = ERROR;
                }
            }
            
            uart2.zeros_ones_range_num++;
            
            if(uart2.address_spi_high == 0x00) // 迭代
            {
                if(uart2.address_spi == 63488) // 63488是uint16_t在2048上迭代的临界值，再加2048就超限了
                {
                    uart2.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart2.address_spi = 0; // 从头开始
                }
                else
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // 地址前进
                }
            }
            else // 到底了
            {
                if(uart2.address_spi == 63488) // 63488是uint16_t在2048上迭代的临界值，再加2048就超限了
                {
                    uart2.zeros_test = ERROR; // 结束迭代
                    
                    // EE B1 52 00 00 00 01 30 3B D5 FD B3 A3 3B FF FC FF FF   （正确）
                    
                    // EE B1 52 00 00 00 01 30 3B D2 EC B3 A3 3B FF FC FF FF   （错误）
                    
                    uart2.zeros_ones_report = SUCCESS;
                    uart2.report_step = 0;
                }
                else
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // 地址前进
                }
            }
        }
    }
    
}
    


void ones_to_trans(void)
{
    if(uart1.ones_to_test == SUCCESS) // 开始测试信号
    {
        uart1.ones_to_test = ERROR;
        uart1.ones_test = SUCCESS;
        uart1.address_spi = 0x0000; // 初始地址
        uart1.address_spi_high = 0x00; // 初始高地址
        uart1.zeros_ones_range_num = 0;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // 错误矩阵更新
        }
    }
    if(uart1.ones_test == SUCCESS)
    {
        if(uart1.ones_trans == SUCCESS) // 阴：发送数据
        {
            uart1.ones_trans = ERROR; // 一共2^17地址，一次发2^12个，共分32模块
            SPI_Write_zeros_ones(uart1.ones_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI发送
        }
        else // 阳：接收数据
        {
            uart1.ones_trans = SUCCESS;
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI接收
            
            for(int i = 0; i < 2048; i++) // 比对数据
            {
                if(uart1.spi_rx_buffer[i] != 0xFF) // 不全零记录错误点
                {
                    uart1.error_array[uart1.zeros_ones_range_num]++; // 错误数+1
                    if(uart1.error_array[uart1.zeros_ones_range_num] == 0xFF) // 设定原因，最大满了就不算后面的了
                    {
                        break;
                    }
                }
            }
            
            uart1.zeros_ones_range_num++;
            
            if(uart1.address_spi_high == 0x00) // 迭代
            {
                if(uart1.address_spi == 63488) // 63488是uint16_t在2048上迭代的临界值，再加2048就超限了
                {
                    uart1.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart1.address_spi = 0; // 从头开始
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
            }
            else // 到底了
            {
                if(uart1.address_spi == 63488) // 63488是uint16_t在2048上迭代的临界值，再加2048就超限了
                {
                    uart1.ones_test = ERROR; // 结束迭代
                    uart1.to_trans.trans8[0] = 0xA5;
                    uart1.to_trans.trans8[1] = 0x5A;
                    uart1.to_trans.trans8[2] = 0x42; // 66
                    uart1.to_trans.trans8[3] = 0x16; // 指令号（暂定）
                    for(int i = 0; i < 64; i++)
                    {
                        uart1.to_trans.trans8[4 + i] = uart1.error_array[i]; // 提取到发送数组
                    }
                    uart1.to_trans.trans8[68] = 0x40;
                    uart1.to_trans.trans8[69] = 0xEE;
                    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 70); // 一次发送
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
            }
        }
    }
    
    // 显示屏部分（更新：全域测试）
    if(uart2.ones_to_test == SUCCESS) // 开始测试信号
    {
        uart2.ones_to_test = ERROR;
        uart2.ones_test = SUCCESS;
        uart2.address_spi = 0x0000; // 初始地址
        uart2.address_spi_high = 0x00; // 初始高地址
        uart2.zeros_ones_range_num = 0;
        uart2.all_correct_normal = SUCCESS;
        for(int i = 0; i < 64; i++)
        {
            uart2.error_array[i] = 0; // 错误矩阵更新
        }
    }
    if(uart2.ones_test == SUCCESS)
    {
        if(uart2.ones_trans == SUCCESS) // 阴：发送数据
        {
            uart2.ones_trans = ERROR; // 一共2^17地址，一次发2^12个，共分32模块
            SPI_Write_zeros_ones(uart2.ones_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI发送
        }
        else // 阳：接收数据
        {
            uart2.ones_trans = SUCCESS;
            SPI_Read_zeros_ones(uart2.spi_rx_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI接收
            
            for(int i = 0; i < 2048; i++) // 比对数据
            {
                if(uart2.spi_rx_buffer[i] != 0xFF) // 不全零记录错误点
                {
                    uart2.error_array[uart2.zeros_ones_range_num]++; // 错误数+1
                    if(uart2.error_array[uart2.zeros_ones_range_num] == 0xFF) // 设定原因，最大满了就不算后面的了
                    {
                        break;
                    }
                }
                if(uart2.error_array[uart2.zeros_ones_range_num] != 0)
                {
                    uart2.all_correct_normal = ERROR;
                }
            }
            
            uart2.zeros_ones_range_num++;
            
            if(uart2.address_spi_high == 0x00) // 迭代
            {
                if(uart2.address_spi == 63488) // 63488是uint16_t在2048上迭代的临界值，再加2048就超限了
                {
                    uart2.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart2.address_spi = 0; // 从头开始
                }
                else
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // 地址前进
                }
            }
            else // 到底了
            {
                if(uart2.address_spi == 63488) // 63488是uint16_t在2048上迭代的临界值，再加2048就超限了
                {
                    uart2.ones_test = ERROR; // 结束迭代
                    
                    // EE B1 52 00 00 00 01 30 3B D5 FD B3 A3 3B FF FC FF FF   （正确）
                    
                    // EE B1 52 00 00 00 01 30 3B D2 EC B3 A3 3B FF FC FF FF   （错误）
                    
                    uart2.zeros_ones_report = SUCCESS;
                    uart2.report_step = 0;
                }
                else
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // 地址前进
                }
            }
        }
    }
    
}



void zeros_to_trans_per_frame(void)
{
    if(uart1.zeros_to_test == SUCCESS) // 开始测试信号
    {
        uart1.zeros_to_test = ERROR;
        uart1.zeros_test = SUCCESS;
        uart1.address_spi = 0x0000; // 初始地址
        uart1.address_spi_high = 0x00; // 初始高地址
        uart1.zeros_ones_range_num = 0;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // 错误矩阵更新
        }
    }
    if(uart1.zeros_test == SUCCESS)
    {
        if(uart1.zeros_trans == SUCCESS) // 阴：发送数据
        {
            uart1.zeros_trans = ERROR; // 一共2^17地址，一次发2^12个，共分32模块
            SPI_Write_zeros_ones(uart1.zeros_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI发送
        }
        else // 阳：接收数据
        {
            uart1.zeros_trans = SUCCESS;
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI接收
            
            for(int i = 0; i < 2048; i++) // 比对数据
            {
                if(uart1.spi_rx_buffer[i] != 0x00) // 不全零记录错误点
                {
                    uart1.error_array[uart1.zeros_ones_range_num]++; // 错误数+1
                    if(uart1.error_array[uart1.zeros_ones_range_num] == 0xFF) // 设定原因，最大满了就不算后面的了
                    {
                        break;
                    }
                }
            }
            
            // 发送情况（理应64次）
            uart1.to_trans.trans8[0] = 0xA5;
            uart1.to_trans.trans8[1] = 0x5A;
            uart1.to_trans.trans8[2] = 0x03;
            uart1.to_trans.trans8[3] = 0x17; // 指令：15全零，16全一，17逐帧全零，18逐帧全一
            uart1.to_trans.trans8[4] = uart1.error_array[uart1.zeros_ones_range_num];
            uart1.to_trans.trans8[5] = 0x01;
            uart1.to_trans.trans8[6] = 0xEE;
            HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 7);
            
            uart1.zeros_ones_range_num++;
            
            if(uart1.address_spi_high == 0x00) // 迭代
            {
                if(uart1.address_spi == 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart1.address_spi = 0; // 从头开始
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
            }
            else // 到底了
            {
                if(uart1.address_spi == 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.zeros_test = ERROR; // 结束迭代
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
            }
        }
    }
}

void ones_to_trans_per_frame(void)
{
    if(uart1.ones_to_test == SUCCESS) // 开始测试信号
    {
        uart1.ones_to_test = ERROR;
        uart1.ones_test = SUCCESS;
        uart1.address_spi = 0x0000; // 初始地址
        uart1.address_spi_high = 0x00; // 初始高地址
        uart1.zeros_ones_range_num = 0;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // 错误矩阵更新
        }
    }
    if(uart1.ones_test == SUCCESS)
    {
        if(uart1.ones_trans == SUCCESS) // 阴：发送数据
        {
            uart1.ones_trans = ERROR; // 一共2^17地址，一次发2^12个，共分32模块
            SPI_Write_zeros_ones(uart1.ones_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI发送
        }
        else // 阳：接收数据
        {
            uart1.ones_trans = SUCCESS;
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI接收
            
            for(int i = 0; i < 2048; i++) // 比对数据
            {
                if(uart1.spi_rx_buffer[i] != 0xFF) // 不全零记录错误点
                {
                    uart1.error_array[uart1.zeros_ones_range_num]++; // 错误数+1
                    if(uart1.error_array[uart1.zeros_ones_range_num] == 0xFF) // 设定原因，最大满了就不算后面的了
                    {
                        break;
                    }
                }
            }
            
            // 发送情况（理应64次）
            uart1.to_trans.trans8[0] = 0xA5;
            uart1.to_trans.trans8[1] = 0x5A;
            uart1.to_trans.trans8[2] = 0x03;
            uart1.to_trans.trans8[3] = 0x18; // 指令：15全零，16全一，17逐帧全零，18逐帧全一
            uart1.to_trans.trans8[4] = uart1.error_array[uart1.zeros_ones_range_num];
            uart1.to_trans.trans8[5] = 0x01;
            uart1.to_trans.trans8[6] = 0xEE;
            HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 7);
            
            uart1.zeros_ones_range_num++;
            
            if(uart1.address_spi_high == 0x00) // 迭代
            {
                if(uart1.address_spi == 63488) // 63488是uint16_t在2048上迭代的临界值，再加2048就超限了
                {
                    uart1.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart1.address_spi = 0; // 从头开始
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
            }
            else // 到底了
            {
                if(uart1.address_spi == 63488) // 63488是uint16_t在2048上迭代的临界值，再加2048就超限了
                {
                    uart1.ones_test = ERROR; // 结束迭代
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
            }
        }
    }
}




// 新上位机串口疲劳测试部分
void zeros_to_trans_tired(void)
{
    if(uart1.zeros_to_test_tired == SUCCESS) // 开始测试信号
    {
        uart1.zeros_to_test_tired = ERROR;
        uart1.zeros_test_tired = SUCCESS;
        uart1.address_spi = 0x0000; // 初始地址
        uart1.address_spi_high = 0x00; // 初始高地址
        uart1.zeros_ones_range_num = 0;
    }
    if(uart1.zeros_test_tired == SUCCESS)
    {
        if(uart1.zeros_trans_tired == SUCCESS) // 阴：发送数据
        {
            uart1.zeros_trans_tired = ERROR; // 一共2^17地址，一次发2^12个，共分32模块
            SPI_Write_zeros_ones(uart1.zeros_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI发送
        }
        else // 阳：接收数据
        {
            uart1.zeros_trans_tired = SUCCESS;
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI接收
            
            for(int i = 0; i < 2048; i++) // 比对数据
            {
                if(uart1.spi_rx_buffer[i] != 0x00) // 不全零记录错误点
                {
                    uart1.error_rate_in_tired.error_16[uart1.zeros_ones_range_num]++; // 错误数+1
                    break;
                }
            }
            
            uart1.zeros_ones_range_num++;
            
            if(uart1.address_spi_high == 0x00) // 迭代
            {
                if(uart1.address_spi == 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart1.address_spi = 0; // 从头开始
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
            }
            else // 到底了
            {
                if(uart1.address_spi == 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.zeros_test_tired = ERROR; // 结束迭代
                    uart1.zeros_tired_finish = SUCCESS;
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
            }
        }
    }
}

void ones_to_trans_tired(void)
{
    if(uart1.ones_to_test_tired == SUCCESS) // 开始测试信号
    {
        uart1.ones_to_test_tired = ERROR;
        uart1.ones_test_tired = SUCCESS;
        uart1.address_spi = 0x0000; // 初始地址
        uart1.address_spi_high = 0x00; // 初始高地址
        uart1.zeros_ones_range_num = 0;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // 错误矩阵更新
        }
    }
    if(uart1.ones_test_tired == SUCCESS)
    {
        if(uart1.ones_trans_tired == SUCCESS) // 阴：发送数据
        {
            uart1.ones_trans_tired = ERROR; // 一共2^17地址，一次发2^12个，共分32模块
            SPI_Write_zeros_ones(uart1.ones_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI发送
        }
        else // 阳：接收数据
        {
            uart1.ones_trans_tired = SUCCESS;
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI接收
            
            for(int i = 0; i < 2048; i++) // 比对数据
            {
                if(uart1.spi_rx_buffer[i] != 0xFF) // 不全零记录错误点
                {
                    uart1.error_rate_in_tired.error_16[uart1.zeros_ones_range_num]++; // 错误数+1
                    break;
                }
            }
            
            uart1.zeros_ones_range_num++;
            
            if(uart1.address_spi_high == 0x00) // 迭代
            {
                if(uart1.address_spi == 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart1.address_spi = 0; // 从头开始
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
            }
            else // 到底了
            {
                if(uart1.address_spi == 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.ones_test_tired = ERROR; // 结束迭代
                    uart1.ones_tired_finish = SUCCESS;
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
            }
        }
    }
}


void to_tired_test_zeros_ones(void) // 疲劳测试处理，主函数循环使用
{
    if(uart1.to_tired_test == SUCCESS)
    {
        uart1.to_tired_test = ERROR;
        uart1.tired_test = SUCCESS;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_rate_in_tired.error_16[i] = 0; // 错误矩阵更新
        }
        uart1.zeros_to_test_tired = SUCCESS;
    }
    if(uart1.tired_test == SUCCESS)
        {
            if(uart1.zeros_tired_finish == SUCCESS)
            {
                uart1.zeros_tired_finish = ERROR;
                uart1.ones_to_test_tired = SUCCESS;
            }
            
            if(uart1.ones_tired_finish == SUCCESS)
            {
                uart1.ones_tired_finish = ERROR;
                uart1.num_of_test.num_of_test_16[0]++;
                
                
                
                if(uart1.num_of_test.num_of_test_16[0] == uart1.num_of_test_set.num_of_test_set_16[0])
                {
                    uart1.tired_trans_report = ERROR;
                    uart1.to_trans.trans8[0] = 0xA5;
                    uart1.to_trans.trans8[1] = 0x5A;
                    uart1.to_trans.trans8[2] = 0x82;
                    uart1.to_trans.trans8[3] = 0x05; // 反馈测试结果
                    // HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 4);
                    for(int i = 0; i < 128; i++)
                    {
                        uart1.to_trans.trans8[4 + i] = uart1.error_rate_in_tired.error_8[i];
                    }
                    uart1.to_trans.trans8[132] = 0x80;
                    uart1.to_trans.trans8[133] = 0xEE;
                    for(int i = 0; i < 64; i++)
                    {
                        uart1.error_rate_in_tired.error_16[i] = 0; // 归位
                    }
                    uart1.num_of_test.num_of_test_16[0] = 0;
                    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 134); 
                }
                else
                {
                    uart1.to_trans.trans8[0] = 0xA5;
                    uart1.to_trans.trans8[1] = 0x5A;
                    uart1.to_trans.trans8[2] = 0x01;
                    uart1.to_trans.trans8[3] = 0x31; // 中途
                    uart1.to_trans.trans8[4] = 0xEE;
                    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5); // 发送进行中的反馈
                }
                uart1.zeros_to_test_tired = SUCCESS;
            }
        }
}



// 断电测试遍历
void cut_off_zeros_ones(void)
{
    if(uart1.cut_off_test == SUCCESS)
    {
        uart1.cut_off_test = ERROR;
        uart1.cut_off_continue = SUCCESS;
        uart1.address_spi = 0x0000; // 初始地址
        uart1.address_spi_high = 0x00;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // 错误矩阵更新
            SPI_Write_zeros_ones(uart1.ones_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI发送
            
            
            if(uart1.address_spi_high == 0x00) // 迭代
            {
                if(uart1.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                    
                }
                else
                {
                    uart1.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart1.address_spi = 0; // 从头开始
                }
            }
            else if(uart1.address_spi_high == 0x01) // 到底了
            {
                if(uart1.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
                else
                {
                    break;
                }
            }
        }
        sys.timer_1 = 0;
        uart1.cut_off_time = uart1.cut_down_time.cutdown_16[0]; // 断电时长存储！
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); // MOS管执行断电
    }
    if((uart1.cut_off_time == sys.timer_1) && (uart1.cut_off_continue == SUCCESS)) // 按照时间断电结束：开电
    {
        uart1.cut_off_continue = ERROR;
        uart1.cut_off_time = 0;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); // MOS管执行开电
        uart1.address_spi = 0x0000; // 初始地址
        uart1.address_spi_high = 0x00;
        
        
        for(int j = 0; j < 64; j++)
        {
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI接收
                
            for(int i = 0; i < 2048; i++) // 比对数据
            {
                if(uart1.spi_rx_buffer[i] != 0xFF) // 不全零记录错误点
                {
                    uart1.error_array[uart1.zeros_ones_range_num]++; // 错误数+1
                    if(uart1.error_array[uart1.zeros_ones_range_num] == 0xFF) // 设定原因，最大满了就不算后面的了
                    {
                        break;
                    }
                }
            }
            
            if(uart1.address_spi_high == 0x00) // 迭代
            {
                if(uart1.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
                else
                {
                    uart1.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart1.address_spi = 0; // 从头开始
                }
            }
            else if(uart1.address_spi_high == 0x01) // 到底了
            {
                if(uart1.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
                else
                {
                    uart1.to_trans.trans8[0] = 0xA5;
                    uart1.to_trans.trans8[1] = 0x5A;
                    uart1.to_trans.trans8[2] = 0x42; // 66
                    uart1.to_trans.trans8[3] = 0x45; // 指令号（暂定）
                    for(int i = 0; i < 64; i++)
                    {
                        uart1.to_trans.trans8[4 + i] = uart1.error_array[i]; // 提取到发送数组
                    }
                    uart1.to_trans.trans8[68] = 0x40;
                    uart1.to_trans.trans8[69] = 0xEE;
                    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 70); // 一次发送
                    uart1.cut_off_to_continue_next = SUCCESS;
                }
            }
        }
    }
    
    if(uart1.cut_off_to_continue_next == SUCCESS)
    {
        uart1.cut_off_to_continue_next = ERROR;
        uart1.cut_off_continue_next = SUCCESS;
        uart1.address_spi = 0x0000; // 初始地址
        uart1.address_spi_high = 0x00;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // 错误矩阵更新
            SPI_Write_zeros_ones(uart1.zeros_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI发送
            
            
            if(uart1.address_spi_high == 0x00) // 迭代
            {
                if(uart1.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
                else
                {
                    uart1.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart1.address_spi = 0; // 从头开始
                }
            }
            else if(uart1.address_spi_high == 0x01) // 到底了
            {
                if(uart1.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
                else
                {
                    break;
                }
            }
        }
        sys.timer_1 = 0;
        uart1.cut_off_time = uart1.cut_down_time.cutdown_16[0]; // 断电时长存储！
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); // MOS管执行断电
    }
    if((uart1.cut_off_time == sys.timer_1) && (uart1.cut_off_continue_next == SUCCESS)) // 按照时间断电结束：开电
    {
        uart1.cut_off_continue_next = ERROR;
        uart1.cut_off_time = 0;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); // MOS管执行开电
        uart1.address_spi = 0x0000; // 初始地址
        uart1.address_spi_high = 0x00;
        
        
        for(int j = 0; j < 64; j++)
        {
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI接收
                
            for(int i = 0; i < 2048; i++) // 比对数据
            {
                if(uart1.spi_rx_buffer[i] != 0x00) // 不全零记录错误点
                {
                    uart1.error_array[uart1.zeros_ones_range_num]++; // 错误数+1
                    if(uart1.error_array[uart1.zeros_ones_range_num] == 0xFF) // 设定原因，最大满了就不算后面的了
                    {
                        break;
                    }
                }
            }
            
            if(uart1.address_spi_high == 0x00) // 迭代
            {
                if(uart1.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
                else
                {
                    uart1.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart1.address_spi = 0; // 从头开始
                }
            }
            else if(uart1.address_spi_high == 0x01) // 到底了
            {
                if(uart1.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // 地址前进
                }
                else
                {
                    uart1.to_trans.trans8[0] = 0xA5;
                    uart1.to_trans.trans8[1] = 0x5A;
                    uart1.to_trans.trans8[2] = 0x42; // 66
                    uart1.to_trans.trans8[3] = 0x46; // 指令号（暂定）
                    for(int i = 0; i < 64; i++)
                    {
                        uart1.to_trans.trans8[4 + i] = uart1.error_array[i]; // 提取到发送数组
                    }
                    uart1.to_trans.trans8[68] = 0x40;
                    uart1.to_trans.trans8[69] = 0xEE;
                    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 70); // 一次发送
                }
            }
        }
    }
    
}

void cut_off_test_by_screan_ones(void)
{
    if(uart2.cut_off_to_ones == SUCCESS)
    {
        uart2.all_correct_cut_off = SUCCESS;
        uart2.cut_off_continue = SUCCESS;
        uart2.address_spi = 0x0000; // 初始地址
        uart2.address_spi_high = 0x00;
        for(int i = 0; i < 64; i++)
        {
            uart2.error_array[i] = 0; // 错误矩阵更新
            SPI_Write_zeros_ones(uart2.ones_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI发送
            
            
            if(uart2.address_spi_high == 0x00) // 迭代
            {
                if(uart2.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // 地址前进
                    
                }
                else
                {
                    uart2.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？） 
                    uart2.address_spi = 0; // 从头开始
                }
            }
            else if(uart2.address_spi_high == 0x01) // 到底了
            {
                if(uart2.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // 地址前进
                }
                else
                {
                    break;
                }
            }
        }
        uart2.cut_off_to_zeros = ERROR;
        uart2.cut_off_to_ones = ERROR;
        sys.timer_1 = 0;
        uart2.cut_off_time = 0x00FF; // 断电时长存储！
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); // MOS管执行断电
    }
    if((uart2.cut_off_time == sys.timer_1) && (uart2.cut_off_continue == SUCCESS)) // 按照时间断电结束：开电
    {
        uart2.cut_off_continue = ERROR;
        uart2.cut_off_time = 0;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); // MOS管执行开电
        uart2.address_spi = 0x0000; // 初始地址
        uart2.address_spi_high = 0x00;
        
        
        for(int j = 0; j < 64; j++)
        {
            SPI_Read_zeros_ones(uart2.spi_rx_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI接收
                
            for(int i = 0; i < 2048; i++) // 比对数据
            {
                if(uart2.spi_rx_buffer[i] != 0xFF) // 不全零记录错误点
                {
                    uart2.error_array[uart2.zeros_ones_range_num]++; // 错误数+1
                    if(uart2.error_array[uart2.zeros_ones_range_num] == 0xFF) // 设定原因，最大满了就不算后面的了
                    {
                        break;
                    }
                }
                if(uart2.error_array[uart2.zeros_ones_range_num] != 0) // 出错警告
                {
                    uart2.all_correct_cut_off = ERROR;
                }
            }
            
            if(uart2.address_spi_high == 0x00) // 迭代
            {
                if(uart2.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // 地址前进
                }
                else
                {
                    uart2.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart2.address_spi = 0; // 从头开始
                }
            }
            else if(uart2.address_spi_high == 0x01) // 到底了
            {
                if(uart2.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
//                    uart2.zeros_ones_report_cut = SUCCESS;
//                    uart2.report_step = 0;
                    uart2.address_spi =  uart2.address_spi + 2048; // 地址前进
                    
                }
                else
                {
                    uart2.cut_off_to_zeros = SUCCESS;
                }
            }
        }
    }
}

void cut_off_test_by_screan_zeros(void)
{
    if(uart2.cut_off_to_zeros == SUCCESS)
    {
        //
        uart2.cut_off_continue_next = SUCCESS;
        uart2.address_spi = 0x0000; // 初始地址
        uart2.address_spi_high = 0x00;
        for(int i = 0; i < 64; i++)
        {
//            uart2.error_array[i] = 0; // 错误矩阵更新
            SPI_Write_zeros_ones(uart2.zeros_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI发送
            
            
            if(uart2.address_spi_high == 0x00) // 迭代
            {
                if(uart2.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // 地址前进
                }
                else
                {
                    uart2.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart2.address_spi = 0; // 从头开始
                }
            }
            else if(uart2.address_spi_high == 0x01) // 到底了
            {
                if(uart2.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // 地址前进
                }
                else
                {
                    break;
                }
            }
        }
        uart2.cut_off_to_zeros = ERROR;
        uart2.cut_off_to_ones = ERROR;
        sys.timer_1 = 0;
        uart2.cut_off_time = 0x00FF; // 断电时长存储！
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); // MOS管执行断电
    }
    if((uart2.cut_off_time == sys.timer_1) && (uart2.cut_off_continue_next == SUCCESS)) // 按照时间断电结束：开电
    {
        uart2.cut_off_continue_next = ERROR;
        uart2.cut_off_time = 0;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); // MOS管执行开电
        uart2.address_spi = 0x0000; // 初始地址
        uart2.address_spi_high = 0x00;
        
        
        for(int j = 0; j < 64; j++)
        {
            SPI_Read_zeros_ones(uart2.spi_rx_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI接收
                
            for(int i = 0; i < 2048; i++) // 比对数据
            {
                if(uart2.spi_rx_buffer[i] != 0x00) // 不全零记录错误点
                {
                    uart2.error_array[uart2.zeros_ones_range_num]++; // 错误数+1
                    if(uart2.error_array[uart2.zeros_ones_range_num] == 0xFF) // 设定原因，最大满了就不算后面的了
                    {
                        break;
                    }
                }
                if(uart2.error_array[uart2.zeros_ones_range_num] != 0) // 出错警告
                {
                    uart2.all_correct_cut_off = ERROR;
                }
            }
            
            if(uart2.address_spi_high == 0x00) // 迭代
            {
                if(uart2.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // 地址前进
                }
                else
                {
                    uart2.address_spi_high = 0x01; // 更新到下一区（是0x01还是0x80？）
                    uart2.address_spi = 0; // 从头开始
                }
            }
            else if(uart2.address_spi_high == 0x01) // 到底了
            {
                if(uart2.address_spi < 63488) // 61440是uint16_t在4096上迭代的临界值，再加4096就超限了
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // 地址前进
                }
                else
                {
                    int a = j;
                    uart2.zeros_ones_report_cut = SUCCESS; // 发送（待纠错）
                    uart2.report_step = 0;
                }
            }
        }
    }
}


// 含地址的发送，检错，目前未成功，暂不启用
void trans_to_check_with_addr(void)
{
    if(uart1.trans_with_addr == SUCCESS)
    {
        uart1.trans_with_addr = ERROR;
        SPI_Write_zeros_ones(spi1.devided_data, uart1.devided_data_num, uart1.get_addr_set.addr_16[0], uart1.get_addr_set_high); // SPI发送
        uart1.to_trans.trans32[0] = 0x11015AA5; // 0xA55A0111EE，联合体遵循低字节在前原则，故此这样写
        uart1.to_trans.trans8[4] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5); // 发送反馈
    }
}

void receive_to_check_with_addr(void)
{
    if(uart1.receive_with_addr == SUCCESS)
    {
        uart1.receive_with_addr = ERROR;
        SPI_Read_zeros_ones(spi1.spiReader, uart1.devided_data_num, uart1.get_addr_set.addr_16[0], uart1.get_addr_set_high); // SPI读取
        uart1.to_trans.trans8[0] = 0xA5;
        uart1.to_trans.trans8[1] = 0x5A;
        uart1.to_trans.trans8[2] = uart1.devided_data_num + 2; // 数据、指令与校验
        uart1.to_trans.trans8[3] = 0x25; // 指令号
        for(int i = 0; i < uart1.devided_data_num; i++)
        {
            uart1.to_trans.trans8[4 + i] = spi1.spiReader[i]; // 赋值
        }
        uart1.to_trans.trans8[4 + uart1.devided_data_num] = uart1.devided_data_num;
        uart1.to_trans.trans8[5 + uart1.devided_data_num] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 6 + uart1.devided_data_num);
    }
}




// 主循环扫描执行程序，1ms更新一次，所以里面内容必须在1ms内执行完
// 系统时钟频率是72MHz，须注意
void board_ticks_handle(void) // 主循环里一直扫描执行的内容，每次扫描执行一个
{
    //分类讨论，按照tick_step为基准，每循环一次执行一个内容
    switch(sys.tick_step) // 按照设定每毫秒更新一次，即每毫秒所有做一遍
    {
        case 0:
            break; // 零的时候不做，等sys每1ms加一的时候再做（于是里面的内容执行时长不能大于1ms否则会跳步）
        case 1:
            sys.tick_step = 2; // 执行完这步下一次扫描去下一步，目前定下来是扫描串口中断操作
            to_read_spi(); // 读取SPI
            break;
        case 2:
            sys.tick_step = 3;
            to_tired_test();
            to_tired_test_zeros_ones();
            break;
        case 3:
            sys.tick_step = 4;
            receive_forward();
            break;
        case 4:
            to_get_data_devided();
            sys.tick_step = 5;
            break;
        case 5:
            sys.tick_step = 6;
            check_clc(); // 会不会太长了？相信CPU！
            break;
        case 6:
//            to_cut_off();
            sys.tick_step = 7;
            break;
        case 7:
            to_check_flag();
            sys.tick_step = 8;
            break;
        case 8:
            all_in_transmit();
            sys.tick_step = 9;
            break;
        case 9:
            to_change_spi();
            sys.tick_step = 10;
            break;
        case 10:
            clc_screen();
            sys.tick_step = 11;
            break;
        case 11:
//            tired_to_trans_report();
            cut_off_zeros_ones();
            sys.tick_step = 12;
            break;
        case 12:
            zeros_to_trans();
            zeros_to_trans_tired();
            sys.tick_step = 13;
            break;
        case 13:
            ones_to_trans();
            ones_to_trans_tired();
            sys.tick_step = 14;
            break;
        case 14:
            trans_to_check_with_addr();
            sys.tick_step = 15;
            break;
        case 15:
            receive_to_check_with_addr();
            sys.tick_step = 16;
            break;
        case 16:
            zeros_ones_trans_report();
            sys.tick_step = 17;
            break;
        case 17:
            cut_off_test_by_screan_zeros();
            sys.tick_step = 18;
            break;
        case 18:
            cut_off_test_by_screan_ones();
            sys.tick_step = 0;
            break;
    }
}

void board_quick(void)  // 每次循环都调用，串口操作，接收转发
{
    receive_String_Handle(); // 每次循环都调用：看看传输数据 
    receive_String_Handle_devided();
    screen_trans();
}

void board_slow(void)
{
    if(sys.sys_slow > sys.sys_time)
    {
        return;
    }
    sys.sys_slow = sys.sys_time + 100; // 中断函数每1ms触发一次，触发时sys_time+1，所以以下内容100ms执行一次
    // HAL_UART_Transmit_DMA(&huart1, uart1.RXBuffer, uart1.Rx_Count);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
}



// 串口屏：报告发送者
void zeros_ones_trans_report(void)
{
     // EE B1 52 00 09 00 01 30 3B D5 FD B3 A3 3B FF FC FF FF   （正确）
                    
     // EE B1 52 00 09 00 01 30 3B D2 EC B3 A3 3B FF FC FF FF   （错误）
    
    ErrorStatus judge = SUCCESS;
    
    if(uart2.zeros_ones_report == SUCCESS)
    {
        //
        uart2.to_trans.trans64[0] = 0x300100090052B1EE;
        uart2.to_trans.trans8[8] = 0x3B;
        if(uart2.error_array[uart2.report_step] != 0) // 当前错误值
        {
            judge = ERROR;
        }
        if(judge == SUCCESS)
        {
            uart2.to_trans.trans8[9] = 0xD5;
            uart2.to_trans.trans8[10] = 0xFD;
            uart2.to_trans.trans8[11] = 0xB3;
            uart2.to_trans.trans8[12] = 0xA3;
        }
        else
        {
            uart2.to_trans.trans8[9] = 0xD2;
            uart2.to_trans.trans8[10] = 0xEC;
            uart2.to_trans.trans8[11] = 0xB3;
            uart2.to_trans.trans8[12] = 0xA3;
        }
        uart2.to_trans.trans8[13] = 0x3B;
        uart2.to_trans.trans8[14] = 0xFF;
        uart2.to_trans.trans8[15] = 0xFC;
        uart2.to_trans.trans8[16] = 0xFF;
        uart2.to_trans.trans8[17] = 0xFF;
        HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 18); // 发送更新
        while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
        {
            // 判断是否发完，可行性存疑
        }
        uart2.report_step++;
        if(uart2.report_step == 64) // 满溢
        {
            uart2.zeros_ones_report = ERROR;
            
            // 收官检查：是否全对
            if(uart2.all_correct_normal == SUCCESS)
            {
                // EE B1 12 00 0A 00 06 00 08 B4 E6 B4 A2 D5 FD B3 A3 00 07 00 08 B4 E6 B4 A2 D5 FD B3 A3 FF FC FF FF 
                // EE B1 12 00 09 00 06 00 08 B2 E2 CA D4 D5 FD B3 A3 00 07 00 08 B2 E2 CA D4 D5 FD B3 A3 FF FC FF FF 
                uart2.to_trans.trans64[0] = 0x000600090012B1EE; // 8位
                uart2.to_trans.trans64[1] = 0xB3FDD5A2B4E6B408; // 8位
                uart2.to_trans.trans64[2] = 0xB4E6B408000700A3; // 8位
                uart2.to_trans.trans64[3] = 0xFFFCFFA3B3FDD5A2;
                uart2.to_trans.trans8[32] = 0xFF;
                HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 33);
                while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
                {
                    // 判断是否发完，可行性存疑
                }
            }
            else
            {
                // EE B1 12 00 0A 00 06 00 08 B3 F6 CF D6 B4 ED CE F3 00 07 00 08 B3 F6 CF D6 B4 ED CE F3 FF FC FF FF 
                uart2.to_trans.trans64[0] = 0x000600090012B1EE; // 8位
                uart2.to_trans.trans64[1] = 0xCEEDB4D6CFF6B308; // 8位
                uart2.to_trans.trans64[2] = 0xCFF6B308000700F3; // 8位
                uart2.to_trans.trans64[3] = 0xFFFCFFF3CEEDB4D6; // 8位
                uart2.to_trans.trans8[32] = 0xFF;
                HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 33);
                while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
                {
                    // 判断是否发完，可行性存疑
                }
                uart2.all_correct_normal = SUCCESS;
            }
        }
    }
    
    
    // EE B1 52 00 0A 00 01 30 3B D5 FD B3 A3 3B FF FC FF FF   （正确）
                    
    // EE B1 52 00 0A 00 01 30 3B D2 EC B3 A3 3B FF FC FF FF   （错误）
    
    
     if(uart2.zeros_ones_report_cut == SUCCESS)
    {
        //
        uart2.to_trans.trans64[0] = 0x3001000A0052B1EE;
        uart2.to_trans.trans8[8] = 0x3B;
        if(uart2.error_array[uart2.report_step] != 0) // 当前错误值
        {
            judge = ERROR;
        }
        if(judge == SUCCESS)
        {
            uart2.to_trans.trans8[9] = 0xD5;
            uart2.to_trans.trans8[10] = 0xFD;
            uart2.to_trans.trans8[11] = 0xB3;
            uart2.to_trans.trans8[12] = 0xA3;
        }
        else
        {
            uart2.to_trans.trans8[9] = 0xD2;
            uart2.to_trans.trans8[10] = 0xEC;
            uart2.to_trans.trans8[11] = 0xB3;
            uart2.to_trans.trans8[12] = 0xA3;
        }
        uart2.to_trans.trans8[13] = 0x3B;
        uart2.to_trans.trans8[14] = 0xFF;
        uart2.to_trans.trans8[15] = 0xFC;
        uart2.to_trans.trans8[16] = 0xFF;
        uart2.to_trans.trans8[17] = 0xFF;
        HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 18); // 发送更新
        while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
        {
            // 判断是否发完，可行性存疑
        }
        uart2.report_step++;
        if(uart2.report_step == 64) // 满溢
        {
            uart2.zeros_ones_report_cut = ERROR;
            
            // 收官检查：是否全对
            if(uart2.all_correct_cut_off == SUCCESS)
            {
                // EE B1 12 00 0A 00 06 00 08 B4 E6 B4 A2 D5 FD B3 A3 00 07 00 08 B4 E6 B4 A2 D5 FD B3 A3 FF FC FF FF 
                uart2.to_trans.trans64[0] = 0x0006000A0012B1EE; // 8位
                uart2.to_trans.trans64[1] = 0xB3FDD5A2B4E6B408; // 8位
                uart2.to_trans.trans64[2] = 0xB4E6B408000700A3; // 8位
                uart2.to_trans.trans64[3] = 0xFFFCFFA3B3FDD5A2;
                uart2.to_trans.trans8[32] = 0xFF;
                HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 33);
                while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
                {
                    // 判断是否发完，可行性存疑
                }
            }
            else
            {
                // EE B1 12 00 0A 00 06 00 08 B3 F6 CF D6 B4 ED CE F3 00 07 00 08 B3 F6 CF D6 B4 ED CE F3 FF FC FF FF 
                uart2.to_trans.trans64[0] = 0x0006000A0012B1EE; // 8位
                uart2.to_trans.trans64[1] = 0xCEEDB4D6CFF6B308; // 8位
                uart2.to_trans.trans64[2] = 0xCFF6B308000700F3; // 8位
                uart2.to_trans.trans64[3] = 0xFFFCFFF3CEEDB4D6;
                uart2.to_trans.trans8[32] = 0xFF;
                HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 33);
                while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
                {
                    // 判断是否发完，可行性存疑
                }
                uart2.all_correct_cut_off = SUCCESS;
            }
        }
    }
    
}
















