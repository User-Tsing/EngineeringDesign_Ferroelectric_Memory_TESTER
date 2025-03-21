#include <stdio.h>
#include <string.h>
#include "board.h"

/* board.c�ļ�����������ʱ�ָ��ö��߳� */

// ��ֲ��֪����stm32f3xx_it���ж���

// ������ڴ�

sys_type sys; // �ṹ��ʵ�����ڴ˶��壬��ʼ��ֻ��ʼ��ֵ

void board_Init(void) // ��ʼ����������������ѭ����ʼǰ����
{
    sys.sys_time = 0x00000000; // ��ʼ��Ϊ�㣬sysʱ���жϼ�һ��ϵͳʱ��
    sys.tick_step = 0x00;
}

// SPI��ش�������������
void receive_String_Handle(void) // �¼�������������
{
    if(uart1.uart_Receive_String == SUCCESS) // ��λ��������Ƿ���յ�����
    {
        uart1.uart_Receive_String = ERROR; // ��λ��־λ
        // SPI_Write_Flash(uart1.getArray, uart1.contentLength, spi1.addr[spi1.addrNum]); // SPI������������
//        SPI_Write_Flash(uart1.getArray, uart1.contentLength, 0x0000);
        SPI_Write_zeros_ones(uart1.getArray, uart1.contentLength, 0x0000, 0x00);
        // spi1.addrLength[spi1.addrNum] = uart1.contentLength; // ���ݳ��ȣ�Ԥ���෢���
        // spi1.addrNum++; // ���µ�ַ��
        // spi1.addr[spi1.addrNum] = spi1.addr[spi1.addrNum-1] + uart1.contentLength; // �µ�ַ��ԭ��ַ����
        uart1.to_trans.trans32[0] = 0x11015AA5; // 0xA55A0111EE����������ѭ���ֽ���ǰԭ�򣬹ʴ�����д
        uart1.to_trans.trans8[4] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5); // ���ͷ���
    }
    if(uart2.uart_Receive_String == SUCCESS) // ������������Ƿ���յ�����
    {
        uart2.uart_Receive_String = ERROR; // ��λ��־λ
        SPI_Write_Flash(spi1.devided_data, uart2.devided_data_num, 0x0000);
        // ��������Ҫ��ʾ�������ݣ���Ӧͨ��Э��������������ֲ�
        // ��ʾӢ����������ASCII��
        // EE B1 10 00 03 00 06 20 DD DD DD DD DD DD FF FC FF FF
        uart2.to_trans.trans64[0] = 0x200600030010B1EE; // 8�ֽ�
        uart2.to_trans.trans16[4] = 0x7830; // ASCII�룺0x
        for(int i = 0;i < uart2.devided_data_num; i++)
        {
            uart2.to_trans.trans8[10 + 2 * i] = acsii_translate_front(spi1.devided_data[i]);
            uart2.to_trans.trans8[11 + 2 * i] = acsii_translate_behind(spi1.devided_data[i]);
        }
        // ����һ����10+2*uart2.devided_data_num�ֽڵ�����
        uart2.to_trans.trans8[10 + 2 * uart2.devided_data_num] = 0xFF;
        uart2.to_trans.trans8[11 + 2 * uart2.devided_data_num] = 0xFC;
        uart2.to_trans.trans8[12 + 2 * uart2.devided_data_num] = 0xFF;
        uart2.to_trans.trans8[13 + 2 * uart2.devided_data_num] = 0xFF;
        HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 14 + 2 * uart2.devided_data_num); // ���ͷ���
    }
}

// �෢������԰�
void receive_String_Handle_devided(void) // �¼���������Ľ�������
{
    if(uart1.devided_receive_data == SUCCESS)
    {
        uart1.devided_receive_data = ERROR;
        spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]] = uart1.devided_data_num;
        spi1.devided_addr[uart1.devided_datanum.datanum_16[0]] = spi1.next_addr;
        spi1.next_addr += uart1.devided_data_num;
        SPI_Write_Flash(spi1.devided_data, spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]], spi1.devided_addr[uart1.devided_datanum.datanum_16[0]]);
        uart1.to_trans.trans32[0] = 0x11015AA5; // 0xA55A0111EE����������ѭ���ֽ���ǰԭ�򣬹ʴ�����д
        uart1.to_trans.trans8[4] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5); // ���ͷ���
    }
}

void to_get_data_devided(void) // �������ݣ������
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
        HAL_UART_Transmit_DMA(&huart1, to_transfer, spi1.devided_addrLength[uart1.devided_datanum.datanum_16[0]] + 6); // ���ԣ���ָ���
    }
}

void to_read_spi(void) // �¼�������ȡSPI����⣬ֻ�����µ�
{
    if(uart1.uart_to_check_spi == SUCCESS)
    {
        uart1.uart_to_check_spi = ERROR;
        // SPI_Read_Flash(spi1.spiReader, spi1.addrLength[spi1.addrNum -1], spi1.addr[spi1.addrNum -1]); // ��ȡ����
        SPI_Read_Flash(spi1.spiReader, uart1.contentLength, 0x0000);
        // spi1.addrNum--; // ��ȡ����ʧ
        check_data_1();
    }
}

void check_data_1(void) // ��������Ƿ���ͬ�����⺯���ر�ʹ��
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
            HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5); // DMA������λ��
            break;
        }
    }
    if(uart1.data_check_flag == SUCCESS) // ��ȡ�����ͬ
    {
        uart1.to_trans.trans32[0] = 0x00015AA5;
        uart1.to_trans.trans8[4] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5); // DMA������λ��
    }
}

void check_data_1_Beta(void) // ��������Ƿ���ͬ�����⺯���ر�ʹ��
{
    for(int i = 4; i < spi1.devided_addrLength[spi1.devided_addrNum] - 1; i++) // ֻ���м䲿��
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

void receive_forward(void) // �洢ת��
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
        SPI_Read_Flash(spi1.spiReader, uart2.devided_data_num, 0x0000); // �������������ڷ�������ʱ���£���������
        uart2.data_check_flag = SUCCESS;
        for(int i = 0; i < uart2.devided_data_num; i++) // �������
        {
            if(spi1.spiReader[i] != spi1.devided_data[i])
            {
                uart2.data_check_flag = ERROR;
                break;
            }
        }
        if(uart2.data_check_flag == SUCCESS)
        {
            // EE B1 12 00 03 00 07 00 07 20 30 78 46 46 46 46 00 08 00 08 C4 DA C8 DD D5 FD C8 B7 FF FC FF FF ������2�ֽ�����
            
            // EE B1 10 00 03 00 07 30 78 46 46 46 46 FF FC FF FF 
            uart2.to_trans.trans64[0] = 0x300700030010B1EE;
            uart2.to_trans.trans8[8] = 0x78; // ASCII�룺0x
            for(int i = 0;i < uart2.devided_data_num; i++)
            {
                uart2.to_trans.trans8[9 + 2 * i] = acsii_translate_front(spi1.devided_data[i]);
                uart2.to_trans.trans8[10 + 2 * i] = acsii_translate_behind(spi1.devided_data[i]);
            }
            // ����һ����11+2*uart2.devided_data_num�ֽڵ�����
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
            uart2.to_trans.trans8[8] = 0x78; // ASCII�룺0x
            for(int i = 0;i < uart2.devided_data_num; i++)
            {
                uart2.to_trans.trans8[9 + 2 * i] = acsii_translate_front(spi1.devided_data[i]);
                uart2.to_trans.trans8[10 + 2 * i] = acsii_translate_behind(spi1.devided_data[i]);
            }
            // ����һ����9+2*uart2.devided_data_num�ֽڵ�����
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

void all_in_transmit(void) // ȫ������
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

void to_change_spi(void) // �޸�spi��������
{
    if(uart1.spi_to_change == SUCCESS)
    {
        uart1.spi_to_change = ERROR;
        uart1.to_trans.trans32[0] = 0x81045AA5;
        uart1.to_trans.trans16[2] = 0x0000;
        switch(uart1.spi_changer)
        {
            case 0x01: // Ԥ��Ƶ����4�� 18Mbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_4);
                uart1.to_trans.trans16[2] = 0x0400;
                break;
            case 0x02: // Ԥ��Ƶ����8�� 9Mbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_8);
                uart1.to_trans.trans16[2] = 0x0800;
                break;
            case 0x03: // Ԥ��Ƶ����16�� 4.5Mbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_16);
                uart1.to_trans.trans16[2] = 0x1000;
                break;
            case 0x04: // Ԥ��Ƶ����32�� 2.25Mbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_32);
                uart1.to_trans.trans16[2] = 0x2000;
                break;
            case 0x05: // Ԥ��Ƶ����64�� 1.125Mbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_64);
                uart1.to_trans.trans16[2] = 0x4000;
                break;
            case 0x06: // Ԥ��Ƶ����128�� 562.5Kbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_128);
                uart1.to_trans.trans16[2] = 0x8000;
                break;
            case 0x07: // Ԥ��Ƶ����256�� 281.25Kbps
                SPI_Rate_changer(SPI_BAUDRATEPRESCALER_256);
                uart1.to_trans.trans16[2] = 0x0001;
                break;
        }
        uart1.to_trans.trans16[3] = 0xEE02;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 8);
    }
}

void check_clc(void) // �������spi
{
    if(uart1.clc_order == SUCCESS)
    {
        uart1.clc_order = ERROR;
        clc_array();
    }
}

void to_tired_test(void) // ƣ�Ͳ��Դ���������ѭ��ʹ��
{
    
    if(uart2.tired_test == SUCCESS)
    {
        if(spi1.tired_test_Read == ERROR)
        {
            SPI_Write_Flash(spi1.tired_test_string.tired_test_string_8, 8, 0x0000); // ��������
            spi1.tired_test_Read = SUCCESS;
        }
        else
        {
            SPI_Read_Flash(spi1.tired_test_receive.tired_test_receive_8, 8, 0x0000);
            if(spi1.tired_test_string.tired_test_string_64[0] != spi1.tired_test_receive.tired_test_receive_64[0]) // ��ͬ������
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
            uint16_t number = uart2.num_of_error.num_of_error_16[0]; // ת��������
            uint8_t number_size = get_int_size(number);
            uint8_t num_ascii[3];
            get_ascii_from_int(num_ascii, number, number_size);
            for(int i = 0; i < number_size; i++)
            {
                uart2.to_trans.trans8[9 + i] = num_ascii[i];
            }
            // ���˹�9+number_sizeλ
            uart2.to_trans.trans8[9 + number_size] = 0x3B;
            uart2.to_trans.trans8[10 + number_size] = 0xFF;
            uart2.to_trans.trans8[11 + number_size] = 0xFC;
            uart2.to_trans.trans8[12 + number_size] = 0xFF;
            uart2.to_trans.trans8[13 + number_size] = 0xFF;
            uart2.tired_test_by_screen = SUCCESS;
            HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 14 + number_size); // ���ͷ���
            uart2.num_of_error.num_of_error_16[0] = 0;
            uart2.num_of_test.num_of_test_16[0] = 0;
        }
    }
}


void clc_array(void) // ����spi�Ĵ���������
{
    for(int i = 0; i < 512; i++)
    {
        spi1.devided_addr[i] = 0;
        spi1.devided_addrLength[i] = 0;
    }
    spi1.next_addr = 0x0000;
}

void clc_screen(void) // ��ʾ������
{
    if(screen.clear_all == SUCCESS)
    {
        screen.step = 0;
        screen.clear_all = ERROR;
        screen.clear_end = ERROR; // ����Tx
        // EE B1 53 00 02 00 01 FF FC FF FF   0�ţ��������
        uart2.to_trans.trans64[0] = 0xFF0100020053B1EE;
        uart2.to_trans.trans16[4] =0xFFFC;
        uart2.to_trans.trans8[10] =0xFF;
        HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
        screen.step++;
    }
}

void to_check_flag(void) // ���У��λ���������
{
    if(uart1.check_flag_on == SUCCESS)
    {
        uart1.check_flag_on = ERROR;
        uart1.to_trans.trans32[0] = 0x12015AA5;
        uart1.to_trans.trans8[4] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5);
    }
}

// ͨ�ú������¶���

uint8_t acsii_translate_front(uint8_t aa) // ʮ��������תASCII�룬ǰ��λ
{
    uint8_t mid_one = (aa >> 4) & 0x0F; // ������λ��ȡ������λ
    uint8_t trans_result;
    if(mid_one <= 0x09) // ʮ��������0-9��A-F��ASCII����ƫ�ƣ���������
    {
        trans_result = mid_one + 0x30; // ������ASCII��
    }
    else
    {
        trans_result = mid_one - 10 + 'A'; // ��ĸ��ASCII�루'A' = 0x41��
    }
    return trans_result;
}
uint8_t acsii_translate_behind(uint8_t aa) // ʮ��������תASCII�룬����λ
{
    uint8_t mid_one = aa & 0x0F; // ��ȡ����λ
    uint8_t trans_result;
    if(mid_one <= 0x09) // ʮ��������0-9��A-F��ASCII����ƫ�ƣ���������
    {
        trans_result = mid_one + 0x30; // ������ASCII��
    }
    else
    {
        trans_result = mid_one - 10 + 'A'; // ��ĸ��ASCII�루'A' = 0x41��
    }
    return trans_result;
}

uint8_t get_int_size(uint16_t num) // ����λ��
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

void get_ascii_from_int(uint8_t* numascii, uint16_t num, uint8_t numsize) // �����ͻ�ȡascii�룬��λ��ǰ
{
    while(numsize > 0)
    {
        numascii[numsize-1] = (num % 10) + 0x30;
        numsize--;
        num = num / 10;
    }
    // return numascii;
}

// �ϵ�ר�ú������¶���
void turn_off_spi_electric(void)
{
    if(spi1.electric_turn_on == SUCCESS)
    {
        spi1.electric_turn_on = ERROR;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); // MOS��ִ�жϵ�
        spi1.cut_electric = SUCCESS;
        // EE B1 10 00 04 00 06 20 B6 CF B5 E7 FF FC FF FF
        uart2.to_trans.trans64[0] = 0x200600040010B1EE;
        uart2.to_trans.trans64[1] = 0xFFFFFCFFE7B5CFB6; // 16�ֽ�
        HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 16); // ��ʾ����ʾ״̬
    }
}

void turn_on_spi_electric(void)
{
    if(spi1.electric_turn_off == SUCCESS)
    {
        spi1.electric_turn_off = ERROR;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); // MOS��ִ�жϵ�
        spi1.cut_electric = ERROR;
        // EE B1 10 00 04 00 06 20 D5 FD B3 A3 FF FC FF FF
        uart2.to_trans.trans64[0] = 0x200600040010B1EE;
        uart2.to_trans.trans64[1] = 0xFFFFFCFFA3B3FDD5; // 16�ֽ�
        HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 16); // ��ʾ����ʾ״̬
    }
}

// ȫ��ȫһ���������£�
void zeros_to_trans(void)
{
    if(uart1.zeros_to_test == SUCCESS) // ��ʼ�����ź�
    {
        uart1.zeros_to_test = ERROR;
        uart1.zeros_test = SUCCESS;
        uart1.address_spi = 0x0000; // ��ʼ��ַ
        uart1.address_spi_high = 0x00; // ��ʼ�ߵ�ַ
        uart1.zeros_ones_range_num = 0;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // ����������
        }
    }
    if(uart1.zeros_test == SUCCESS)
    {
        if(uart1.zeros_trans == SUCCESS) // ������������
        {
            uart1.zeros_trans = ERROR; // һ��2^17��ַ��һ�η�2^12��������32ģ��
            SPI_Write_zeros_ones(uart1.zeros_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
        }
        else // ������������
        {
            uart1.zeros_trans = SUCCESS;
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
            
            for(int i = 0; i < 2048; i++) // �ȶ�����
            {
                if(uart1.spi_rx_buffer[i] != 0x00) // ��ȫ���¼�����
                {
                    uart1.error_array[uart1.zeros_ones_range_num]++; // ������+1
                    if(uart1.error_array[uart1.zeros_ones_range_num] == 0xFF) // �趨ԭ��������˾Ͳ���������
                    {
                        break;
                    }
                }
            }
            
            uart1.zeros_ones_range_num++;
            
            if(uart1.address_spi_high == 0x00) // ����
            {
                if(uart1.address_spi == 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart1.address_spi = 0; // ��ͷ��ʼ
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
            }
            else // ������
            {
                if(uart1.address_spi == 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.zeros_test = ERROR; // ��������
                    uart1.to_trans.trans8[0] = 0xA5;
                    uart1.to_trans.trans8[1] = 0x5A;
                    uart1.to_trans.trans8[2] = 0x42; // 66
                    uart1.to_trans.trans8[3] = 0x15; // ָ��ţ��ݶ���
                    for(int i = 0; i < 64; i++)
                    {
                        uart1.to_trans.trans8[4 + i] = uart1.error_array[i]; // ��ȡ����������
                    }
                    uart1.to_trans.trans8[68] = 0x40;
                    uart1.to_trans.trans8[69] = 0xEE;
                    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 70); // һ�η���
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
            }
        }
    }
    
    
    if(uart2.zeros_to_test == SUCCESS) // ��ʼ�����ź�
    {
        uart2.zeros_to_test = ERROR;
        uart2.zeros_test = SUCCESS;
        uart2.address_spi = 0x0000; // ��ʼ��ַ
        uart2.address_spi_high = 0x00; // ��ʼ�ߵ�ַ
        uart2.zeros_ones_range_num = 0;
        uart2.all_correct_normal = SUCCESS;
        for(int i = 0; i < 64; i++)
        {
            uart2.error_array[i] = 0; // ����������
        }
    }
    if(uart2.zeros_test == SUCCESS)
    {
        if(uart2.zeros_trans == SUCCESS) // ������������
        {
            uart2.zeros_trans = ERROR; // һ��2^17��ַ��һ�η�2^12��������32ģ��
            SPI_Write_zeros_ones(uart2.zeros_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI����
        }
        else // ������������
        {
            uart2.zeros_trans = SUCCESS;
            SPI_Read_zeros_ones(uart2.spi_rx_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI����
            
            for(int i = 0; i < 2048; i++) // �ȶ�����
            {
                if(uart2.spi_rx_buffer[i] != 0x00) // ��ȫ���¼�����
                {
                    uart2.error_array[uart2.zeros_ones_range_num]++; // ������+1
                    if(uart2.error_array[uart2.zeros_ones_range_num] == 0xFF) // �趨ԭ��������˾Ͳ���������
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
            
            if(uart2.address_spi_high == 0x00) // ����
            {
                if(uart2.address_spi == 63488) // 63488��uint16_t��2048�ϵ������ٽ�ֵ���ټ�2048�ͳ�����
                {
                    uart2.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart2.address_spi = 0; // ��ͷ��ʼ
                }
                else
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // ��ַǰ��
                }
            }
            else // ������
            {
                if(uart2.address_spi == 63488) // 63488��uint16_t��2048�ϵ������ٽ�ֵ���ټ�2048�ͳ�����
                {
                    uart2.zeros_test = ERROR; // ��������
                    
                    // EE B1 52 00 00 00 01 30 3B D5 FD B3 A3 3B FF FC FF FF   ����ȷ��
                    
                    // EE B1 52 00 00 00 01 30 3B D2 EC B3 A3 3B FF FC FF FF   ������
                    
                    uart2.zeros_ones_report = SUCCESS;
                    uart2.report_step = 0;
                }
                else
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // ��ַǰ��
                }
            }
        }
    }
    
}
    


void ones_to_trans(void)
{
    if(uart1.ones_to_test == SUCCESS) // ��ʼ�����ź�
    {
        uart1.ones_to_test = ERROR;
        uart1.ones_test = SUCCESS;
        uart1.address_spi = 0x0000; // ��ʼ��ַ
        uart1.address_spi_high = 0x00; // ��ʼ�ߵ�ַ
        uart1.zeros_ones_range_num = 0;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // ����������
        }
    }
    if(uart1.ones_test == SUCCESS)
    {
        if(uart1.ones_trans == SUCCESS) // ������������
        {
            uart1.ones_trans = ERROR; // һ��2^17��ַ��һ�η�2^12��������32ģ��
            SPI_Write_zeros_ones(uart1.ones_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
        }
        else // ������������
        {
            uart1.ones_trans = SUCCESS;
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
            
            for(int i = 0; i < 2048; i++) // �ȶ�����
            {
                if(uart1.spi_rx_buffer[i] != 0xFF) // ��ȫ���¼�����
                {
                    uart1.error_array[uart1.zeros_ones_range_num]++; // ������+1
                    if(uart1.error_array[uart1.zeros_ones_range_num] == 0xFF) // �趨ԭ��������˾Ͳ���������
                    {
                        break;
                    }
                }
            }
            
            uart1.zeros_ones_range_num++;
            
            if(uart1.address_spi_high == 0x00) // ����
            {
                if(uart1.address_spi == 63488) // 63488��uint16_t��2048�ϵ������ٽ�ֵ���ټ�2048�ͳ�����
                {
                    uart1.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart1.address_spi = 0; // ��ͷ��ʼ
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
            }
            else // ������
            {
                if(uart1.address_spi == 63488) // 63488��uint16_t��2048�ϵ������ٽ�ֵ���ټ�2048�ͳ�����
                {
                    uart1.ones_test = ERROR; // ��������
                    uart1.to_trans.trans8[0] = 0xA5;
                    uart1.to_trans.trans8[1] = 0x5A;
                    uart1.to_trans.trans8[2] = 0x42; // 66
                    uart1.to_trans.trans8[3] = 0x16; // ָ��ţ��ݶ���
                    for(int i = 0; i < 64; i++)
                    {
                        uart1.to_trans.trans8[4 + i] = uart1.error_array[i]; // ��ȡ����������
                    }
                    uart1.to_trans.trans8[68] = 0x40;
                    uart1.to_trans.trans8[69] = 0xEE;
                    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 70); // һ�η���
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
            }
        }
    }
    
    // ��ʾ�����֣����£�ȫ����ԣ�
    if(uart2.ones_to_test == SUCCESS) // ��ʼ�����ź�
    {
        uart2.ones_to_test = ERROR;
        uart2.ones_test = SUCCESS;
        uart2.address_spi = 0x0000; // ��ʼ��ַ
        uart2.address_spi_high = 0x00; // ��ʼ�ߵ�ַ
        uart2.zeros_ones_range_num = 0;
        uart2.all_correct_normal = SUCCESS;
        for(int i = 0; i < 64; i++)
        {
            uart2.error_array[i] = 0; // ����������
        }
    }
    if(uart2.ones_test == SUCCESS)
    {
        if(uart2.ones_trans == SUCCESS) // ������������
        {
            uart2.ones_trans = ERROR; // һ��2^17��ַ��һ�η�2^12��������32ģ��
            SPI_Write_zeros_ones(uart2.ones_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI����
        }
        else // ������������
        {
            uart2.ones_trans = SUCCESS;
            SPI_Read_zeros_ones(uart2.spi_rx_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI����
            
            for(int i = 0; i < 2048; i++) // �ȶ�����
            {
                if(uart2.spi_rx_buffer[i] != 0xFF) // ��ȫ���¼�����
                {
                    uart2.error_array[uart2.zeros_ones_range_num]++; // ������+1
                    if(uart2.error_array[uart2.zeros_ones_range_num] == 0xFF) // �趨ԭ��������˾Ͳ���������
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
            
            if(uart2.address_spi_high == 0x00) // ����
            {
                if(uart2.address_spi == 63488) // 63488��uint16_t��2048�ϵ������ٽ�ֵ���ټ�2048�ͳ�����
                {
                    uart2.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart2.address_spi = 0; // ��ͷ��ʼ
                }
                else
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // ��ַǰ��
                }
            }
            else // ������
            {
                if(uart2.address_spi == 63488) // 63488��uint16_t��2048�ϵ������ٽ�ֵ���ټ�2048�ͳ�����
                {
                    uart2.ones_test = ERROR; // ��������
                    
                    // EE B1 52 00 00 00 01 30 3B D5 FD B3 A3 3B FF FC FF FF   ����ȷ��
                    
                    // EE B1 52 00 00 00 01 30 3B D2 EC B3 A3 3B FF FC FF FF   ������
                    
                    uart2.zeros_ones_report = SUCCESS;
                    uart2.report_step = 0;
                }
                else
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // ��ַǰ��
                }
            }
        }
    }
    
}



void zeros_to_trans_per_frame(void)
{
    if(uart1.zeros_to_test == SUCCESS) // ��ʼ�����ź�
    {
        uart1.zeros_to_test = ERROR;
        uart1.zeros_test = SUCCESS;
        uart1.address_spi = 0x0000; // ��ʼ��ַ
        uart1.address_spi_high = 0x00; // ��ʼ�ߵ�ַ
        uart1.zeros_ones_range_num = 0;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // ����������
        }
    }
    if(uart1.zeros_test == SUCCESS)
    {
        if(uart1.zeros_trans == SUCCESS) // ������������
        {
            uart1.zeros_trans = ERROR; // һ��2^17��ַ��һ�η�2^12��������32ģ��
            SPI_Write_zeros_ones(uart1.zeros_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
        }
        else // ������������
        {
            uart1.zeros_trans = SUCCESS;
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
            
            for(int i = 0; i < 2048; i++) // �ȶ�����
            {
                if(uart1.spi_rx_buffer[i] != 0x00) // ��ȫ���¼�����
                {
                    uart1.error_array[uart1.zeros_ones_range_num]++; // ������+1
                    if(uart1.error_array[uart1.zeros_ones_range_num] == 0xFF) // �趨ԭ��������˾Ͳ���������
                    {
                        break;
                    }
                }
            }
            
            // �����������Ӧ64�Σ�
            uart1.to_trans.trans8[0] = 0xA5;
            uart1.to_trans.trans8[1] = 0x5A;
            uart1.to_trans.trans8[2] = 0x03;
            uart1.to_trans.trans8[3] = 0x17; // ָ�15ȫ�㣬16ȫһ��17��֡ȫ�㣬18��֡ȫһ
            uart1.to_trans.trans8[4] = uart1.error_array[uart1.zeros_ones_range_num];
            uart1.to_trans.trans8[5] = 0x01;
            uart1.to_trans.trans8[6] = 0xEE;
            HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 7);
            
            uart1.zeros_ones_range_num++;
            
            if(uart1.address_spi_high == 0x00) // ����
            {
                if(uart1.address_spi == 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart1.address_spi = 0; // ��ͷ��ʼ
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
            }
            else // ������
            {
                if(uart1.address_spi == 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.zeros_test = ERROR; // ��������
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
            }
        }
    }
}

void ones_to_trans_per_frame(void)
{
    if(uart1.ones_to_test == SUCCESS) // ��ʼ�����ź�
    {
        uart1.ones_to_test = ERROR;
        uart1.ones_test = SUCCESS;
        uart1.address_spi = 0x0000; // ��ʼ��ַ
        uart1.address_spi_high = 0x00; // ��ʼ�ߵ�ַ
        uart1.zeros_ones_range_num = 0;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // ����������
        }
    }
    if(uart1.ones_test == SUCCESS)
    {
        if(uart1.ones_trans == SUCCESS) // ������������
        {
            uart1.ones_trans = ERROR; // һ��2^17��ַ��һ�η�2^12��������32ģ��
            SPI_Write_zeros_ones(uart1.ones_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
        }
        else // ������������
        {
            uart1.ones_trans = SUCCESS;
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
            
            for(int i = 0; i < 2048; i++) // �ȶ�����
            {
                if(uart1.spi_rx_buffer[i] != 0xFF) // ��ȫ���¼�����
                {
                    uart1.error_array[uart1.zeros_ones_range_num]++; // ������+1
                    if(uart1.error_array[uart1.zeros_ones_range_num] == 0xFF) // �趨ԭ��������˾Ͳ���������
                    {
                        break;
                    }
                }
            }
            
            // �����������Ӧ64�Σ�
            uart1.to_trans.trans8[0] = 0xA5;
            uart1.to_trans.trans8[1] = 0x5A;
            uart1.to_trans.trans8[2] = 0x03;
            uart1.to_trans.trans8[3] = 0x18; // ָ�15ȫ�㣬16ȫһ��17��֡ȫ�㣬18��֡ȫһ
            uart1.to_trans.trans8[4] = uart1.error_array[uart1.zeros_ones_range_num];
            uart1.to_trans.trans8[5] = 0x01;
            uart1.to_trans.trans8[6] = 0xEE;
            HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 7);
            
            uart1.zeros_ones_range_num++;
            
            if(uart1.address_spi_high == 0x00) // ����
            {
                if(uart1.address_spi == 63488) // 63488��uint16_t��2048�ϵ������ٽ�ֵ���ټ�2048�ͳ�����
                {
                    uart1.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart1.address_spi = 0; // ��ͷ��ʼ
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
            }
            else // ������
            {
                if(uart1.address_spi == 63488) // 63488��uint16_t��2048�ϵ������ٽ�ֵ���ټ�2048�ͳ�����
                {
                    uart1.ones_test = ERROR; // ��������
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
            }
        }
    }
}




// ����λ������ƣ�Ͳ��Բ���
void zeros_to_trans_tired(void)
{
    if(uart1.zeros_to_test_tired == SUCCESS) // ��ʼ�����ź�
    {
        uart1.zeros_to_test_tired = ERROR;
        uart1.zeros_test_tired = SUCCESS;
        uart1.address_spi = 0x0000; // ��ʼ��ַ
        uart1.address_spi_high = 0x00; // ��ʼ�ߵ�ַ
        uart1.zeros_ones_range_num = 0;
    }
    if(uart1.zeros_test_tired == SUCCESS)
    {
        if(uart1.zeros_trans_tired == SUCCESS) // ������������
        {
            uart1.zeros_trans_tired = ERROR; // һ��2^17��ַ��һ�η�2^12��������32ģ��
            SPI_Write_zeros_ones(uart1.zeros_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
        }
        else // ������������
        {
            uart1.zeros_trans_tired = SUCCESS;
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
            
            for(int i = 0; i < 2048; i++) // �ȶ�����
            {
                if(uart1.spi_rx_buffer[i] != 0x00) // ��ȫ���¼�����
                {
                    uart1.error_rate_in_tired.error_16[uart1.zeros_ones_range_num]++; // ������+1
                    break;
                }
            }
            
            uart1.zeros_ones_range_num++;
            
            if(uart1.address_spi_high == 0x00) // ����
            {
                if(uart1.address_spi == 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart1.address_spi = 0; // ��ͷ��ʼ
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
            }
            else // ������
            {
                if(uart1.address_spi == 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.zeros_test_tired = ERROR; // ��������
                    uart1.zeros_tired_finish = SUCCESS;
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
            }
        }
    }
}

void ones_to_trans_tired(void)
{
    if(uart1.ones_to_test_tired == SUCCESS) // ��ʼ�����ź�
    {
        uart1.ones_to_test_tired = ERROR;
        uart1.ones_test_tired = SUCCESS;
        uart1.address_spi = 0x0000; // ��ʼ��ַ
        uart1.address_spi_high = 0x00; // ��ʼ�ߵ�ַ
        uart1.zeros_ones_range_num = 0;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // ����������
        }
    }
    if(uart1.ones_test_tired == SUCCESS)
    {
        if(uart1.ones_trans_tired == SUCCESS) // ������������
        {
            uart1.ones_trans_tired = ERROR; // һ��2^17��ַ��һ�η�2^12��������32ģ��
            SPI_Write_zeros_ones(uart1.ones_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
        }
        else // ������������
        {
            uart1.ones_trans_tired = SUCCESS;
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
            
            for(int i = 0; i < 2048; i++) // �ȶ�����
            {
                if(uart1.spi_rx_buffer[i] != 0xFF) // ��ȫ���¼�����
                {
                    uart1.error_rate_in_tired.error_16[uart1.zeros_ones_range_num]++; // ������+1
                    break;
                }
            }
            
            uart1.zeros_ones_range_num++;
            
            if(uart1.address_spi_high == 0x00) // ����
            {
                if(uart1.address_spi == 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart1.address_spi = 0; // ��ͷ��ʼ
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
            }
            else // ������
            {
                if(uart1.address_spi == 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.ones_test_tired = ERROR; // ��������
                    uart1.ones_tired_finish = SUCCESS;
                }
                else
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
            }
        }
    }
}


void to_tired_test_zeros_ones(void) // ƣ�Ͳ��Դ���������ѭ��ʹ��
{
    if(uart1.to_tired_test == SUCCESS)
    {
        uart1.to_tired_test = ERROR;
        uart1.tired_test = SUCCESS;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_rate_in_tired.error_16[i] = 0; // ����������
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
                    uart1.to_trans.trans8[3] = 0x05; // �������Խ��
                    // HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 4);
                    for(int i = 0; i < 128; i++)
                    {
                        uart1.to_trans.trans8[4 + i] = uart1.error_rate_in_tired.error_8[i];
                    }
                    uart1.to_trans.trans8[132] = 0x80;
                    uart1.to_trans.trans8[133] = 0xEE;
                    for(int i = 0; i < 64; i++)
                    {
                        uart1.error_rate_in_tired.error_16[i] = 0; // ��λ
                    }
                    uart1.num_of_test.num_of_test_16[0] = 0;
                    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 134); 
                }
                else
                {
                    uart1.to_trans.trans8[0] = 0xA5;
                    uart1.to_trans.trans8[1] = 0x5A;
                    uart1.to_trans.trans8[2] = 0x01;
                    uart1.to_trans.trans8[3] = 0x31; // ��;
                    uart1.to_trans.trans8[4] = 0xEE;
                    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5); // ���ͽ����еķ���
                }
                uart1.zeros_to_test_tired = SUCCESS;
            }
        }
}



// �ϵ���Ա���
void cut_off_zeros_ones(void)
{
    if(uart1.cut_off_test == SUCCESS)
    {
        uart1.cut_off_test = ERROR;
        uart1.cut_off_continue = SUCCESS;
        uart1.address_spi = 0x0000; // ��ʼ��ַ
        uart1.address_spi_high = 0x00;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // ����������
            SPI_Write_zeros_ones(uart1.ones_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
            
            
            if(uart1.address_spi_high == 0x00) // ����
            {
                if(uart1.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                    
                }
                else
                {
                    uart1.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart1.address_spi = 0; // ��ͷ��ʼ
                }
            }
            else if(uart1.address_spi_high == 0x01) // ������
            {
                if(uart1.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
                else
                {
                    break;
                }
            }
        }
        sys.timer_1 = 0;
        uart1.cut_off_time = uart1.cut_down_time.cutdown_16[0]; // �ϵ�ʱ���洢��
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); // MOS��ִ�жϵ�
    }
    if((uart1.cut_off_time == sys.timer_1) && (uart1.cut_off_continue == SUCCESS)) // ����ʱ��ϵ����������
    {
        uart1.cut_off_continue = ERROR;
        uart1.cut_off_time = 0;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); // MOS��ִ�п���
        uart1.address_spi = 0x0000; // ��ʼ��ַ
        uart1.address_spi_high = 0x00;
        
        
        for(int j = 0; j < 64; j++)
        {
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
                
            for(int i = 0; i < 2048; i++) // �ȶ�����
            {
                if(uart1.spi_rx_buffer[i] != 0xFF) // ��ȫ���¼�����
                {
                    uart1.error_array[uart1.zeros_ones_range_num]++; // ������+1
                    if(uart1.error_array[uart1.zeros_ones_range_num] == 0xFF) // �趨ԭ��������˾Ͳ���������
                    {
                        break;
                    }
                }
            }
            
            if(uart1.address_spi_high == 0x00) // ����
            {
                if(uart1.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
                else
                {
                    uart1.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart1.address_spi = 0; // ��ͷ��ʼ
                }
            }
            else if(uart1.address_spi_high == 0x01) // ������
            {
                if(uart1.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
                else
                {
                    uart1.to_trans.trans8[0] = 0xA5;
                    uart1.to_trans.trans8[1] = 0x5A;
                    uart1.to_trans.trans8[2] = 0x42; // 66
                    uart1.to_trans.trans8[3] = 0x45; // ָ��ţ��ݶ���
                    for(int i = 0; i < 64; i++)
                    {
                        uart1.to_trans.trans8[4 + i] = uart1.error_array[i]; // ��ȡ����������
                    }
                    uart1.to_trans.trans8[68] = 0x40;
                    uart1.to_trans.trans8[69] = 0xEE;
                    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 70); // һ�η���
                    uart1.cut_off_to_continue_next = SUCCESS;
                }
            }
        }
    }
    
    if(uart1.cut_off_to_continue_next == SUCCESS)
    {
        uart1.cut_off_to_continue_next = ERROR;
        uart1.cut_off_continue_next = SUCCESS;
        uart1.address_spi = 0x0000; // ��ʼ��ַ
        uart1.address_spi_high = 0x00;
        for(int i = 0; i < 64; i++)
        {
            uart1.error_array[i] = 0; // ����������
            SPI_Write_zeros_ones(uart1.zeros_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
            
            
            if(uart1.address_spi_high == 0x00) // ����
            {
                if(uart1.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
                else
                {
                    uart1.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart1.address_spi = 0; // ��ͷ��ʼ
                }
            }
            else if(uart1.address_spi_high == 0x01) // ������
            {
                if(uart1.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
                else
                {
                    break;
                }
            }
        }
        sys.timer_1 = 0;
        uart1.cut_off_time = uart1.cut_down_time.cutdown_16[0]; // �ϵ�ʱ���洢��
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); // MOS��ִ�жϵ�
    }
    if((uart1.cut_off_time == sys.timer_1) && (uart1.cut_off_continue_next == SUCCESS)) // ����ʱ��ϵ����������
    {
        uart1.cut_off_continue_next = ERROR;
        uart1.cut_off_time = 0;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); // MOS��ִ�п���
        uart1.address_spi = 0x0000; // ��ʼ��ַ
        uart1.address_spi_high = 0x00;
        
        
        for(int j = 0; j < 64; j++)
        {
            SPI_Read_zeros_ones(uart1.spi_rx_buffer, 2048, uart1.address_spi, uart1.address_spi_high); // SPI����
                
            for(int i = 0; i < 2048; i++) // �ȶ�����
            {
                if(uart1.spi_rx_buffer[i] != 0x00) // ��ȫ���¼�����
                {
                    uart1.error_array[uart1.zeros_ones_range_num]++; // ������+1
                    if(uart1.error_array[uart1.zeros_ones_range_num] == 0xFF) // �趨ԭ��������˾Ͳ���������
                    {
                        break;
                    }
                }
            }
            
            if(uart1.address_spi_high == 0x00) // ����
            {
                if(uart1.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
                else
                {
                    uart1.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart1.address_spi = 0; // ��ͷ��ʼ
                }
            }
            else if(uart1.address_spi_high == 0x01) // ������
            {
                if(uart1.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart1.address_spi =  uart1.address_spi + 2048; // ��ַǰ��
                }
                else
                {
                    uart1.to_trans.trans8[0] = 0xA5;
                    uart1.to_trans.trans8[1] = 0x5A;
                    uart1.to_trans.trans8[2] = 0x42; // 66
                    uart1.to_trans.trans8[3] = 0x46; // ָ��ţ��ݶ���
                    for(int i = 0; i < 64; i++)
                    {
                        uart1.to_trans.trans8[4 + i] = uart1.error_array[i]; // ��ȡ����������
                    }
                    uart1.to_trans.trans8[68] = 0x40;
                    uart1.to_trans.trans8[69] = 0xEE;
                    HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 70); // һ�η���
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
        uart2.address_spi = 0x0000; // ��ʼ��ַ
        uart2.address_spi_high = 0x00;
        for(int i = 0; i < 64; i++)
        {
            uart2.error_array[i] = 0; // ����������
            SPI_Write_zeros_ones(uart2.ones_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI����
            
            
            if(uart2.address_spi_high == 0x00) // ����
            {
                if(uart2.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // ��ַǰ��
                    
                }
                else
                {
                    uart2.address_spi_high = 0x01; // ���µ���һ������0x01����0x80���� 
                    uart2.address_spi = 0; // ��ͷ��ʼ
                }
            }
            else if(uart2.address_spi_high == 0x01) // ������
            {
                if(uart2.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // ��ַǰ��
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
        uart2.cut_off_time = 0x00FF; // �ϵ�ʱ���洢��
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); // MOS��ִ�жϵ�
    }
    if((uart2.cut_off_time == sys.timer_1) && (uart2.cut_off_continue == SUCCESS)) // ����ʱ��ϵ����������
    {
        uart2.cut_off_continue = ERROR;
        uart2.cut_off_time = 0;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); // MOS��ִ�п���
        uart2.address_spi = 0x0000; // ��ʼ��ַ
        uart2.address_spi_high = 0x00;
        
        
        for(int j = 0; j < 64; j++)
        {
            SPI_Read_zeros_ones(uart2.spi_rx_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI����
                
            for(int i = 0; i < 2048; i++) // �ȶ�����
            {
                if(uart2.spi_rx_buffer[i] != 0xFF) // ��ȫ���¼�����
                {
                    uart2.error_array[uart2.zeros_ones_range_num]++; // ������+1
                    if(uart2.error_array[uart2.zeros_ones_range_num] == 0xFF) // �趨ԭ��������˾Ͳ���������
                    {
                        break;
                    }
                }
                if(uart2.error_array[uart2.zeros_ones_range_num] != 0) // ������
                {
                    uart2.all_correct_cut_off = ERROR;
                }
            }
            
            if(uart2.address_spi_high == 0x00) // ����
            {
                if(uart2.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // ��ַǰ��
                }
                else
                {
                    uart2.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart2.address_spi = 0; // ��ͷ��ʼ
                }
            }
            else if(uart2.address_spi_high == 0x01) // ������
            {
                if(uart2.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
//                    uart2.zeros_ones_report_cut = SUCCESS;
//                    uart2.report_step = 0;
                    uart2.address_spi =  uart2.address_spi + 2048; // ��ַǰ��
                    
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
        uart2.address_spi = 0x0000; // ��ʼ��ַ
        uart2.address_spi_high = 0x00;
        for(int i = 0; i < 64; i++)
        {
//            uart2.error_array[i] = 0; // ����������
            SPI_Write_zeros_ones(uart2.zeros_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI����
            
            
            if(uart2.address_spi_high == 0x00) // ����
            {
                if(uart2.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // ��ַǰ��
                }
                else
                {
                    uart2.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart2.address_spi = 0; // ��ͷ��ʼ
                }
            }
            else if(uart2.address_spi_high == 0x01) // ������
            {
                if(uart2.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // ��ַǰ��
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
        uart2.cut_off_time = 0x00FF; // �ϵ�ʱ���洢��
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); // MOS��ִ�жϵ�
    }
    if((uart2.cut_off_time == sys.timer_1) && (uart2.cut_off_continue_next == SUCCESS)) // ����ʱ��ϵ����������
    {
        uart2.cut_off_continue_next = ERROR;
        uart2.cut_off_time = 0;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); // MOS��ִ�п���
        uart2.address_spi = 0x0000; // ��ʼ��ַ
        uart2.address_spi_high = 0x00;
        
        
        for(int j = 0; j < 64; j++)
        {
            SPI_Read_zeros_ones(uart2.spi_rx_buffer, 2048, uart2.address_spi, uart2.address_spi_high); // SPI����
                
            for(int i = 0; i < 2048; i++) // �ȶ�����
            {
                if(uart2.spi_rx_buffer[i] != 0x00) // ��ȫ���¼�����
                {
                    uart2.error_array[uart2.zeros_ones_range_num]++; // ������+1
                    if(uart2.error_array[uart2.zeros_ones_range_num] == 0xFF) // �趨ԭ��������˾Ͳ���������
                    {
                        break;
                    }
                }
                if(uart2.error_array[uart2.zeros_ones_range_num] != 0) // ������
                {
                    uart2.all_correct_cut_off = ERROR;
                }
            }
            
            if(uart2.address_spi_high == 0x00) // ����
            {
                if(uart2.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // ��ַǰ��
                }
                else
                {
                    uart2.address_spi_high = 0x01; // ���µ���һ������0x01����0x80����
                    uart2.address_spi = 0; // ��ͷ��ʼ
                }
            }
            else if(uart2.address_spi_high == 0x01) // ������
            {
                if(uart2.address_spi < 63488) // 61440��uint16_t��4096�ϵ������ٽ�ֵ���ټ�4096�ͳ�����
                {
                    uart2.address_spi =  uart2.address_spi + 2048; // ��ַǰ��
                }
                else
                {
                    int a = j;
                    uart2.zeros_ones_report_cut = SUCCESS; // ���ͣ�������
                    uart2.report_step = 0;
                }
            }
        }
    }
}


// ����ַ�ķ��ͣ����Ŀǰδ�ɹ����ݲ�����
void trans_to_check_with_addr(void)
{
    if(uart1.trans_with_addr == SUCCESS)
    {
        uart1.trans_with_addr = ERROR;
        SPI_Write_zeros_ones(spi1.devided_data, uart1.devided_data_num, uart1.get_addr_set.addr_16[0], uart1.get_addr_set_high); // SPI����
        uart1.to_trans.trans32[0] = 0x11015AA5; // 0xA55A0111EE����������ѭ���ֽ���ǰԭ�򣬹ʴ�����д
        uart1.to_trans.trans8[4] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 5); // ���ͷ���
    }
}

void receive_to_check_with_addr(void)
{
    if(uart1.receive_with_addr == SUCCESS)
    {
        uart1.receive_with_addr = ERROR;
        SPI_Read_zeros_ones(spi1.spiReader, uart1.devided_data_num, uart1.get_addr_set.addr_16[0], uart1.get_addr_set_high); // SPI��ȡ
        uart1.to_trans.trans8[0] = 0xA5;
        uart1.to_trans.trans8[1] = 0x5A;
        uart1.to_trans.trans8[2] = uart1.devided_data_num + 2; // ���ݡ�ָ����У��
        uart1.to_trans.trans8[3] = 0x25; // ָ���
        for(int i = 0; i < uart1.devided_data_num; i++)
        {
            uart1.to_trans.trans8[4 + i] = spi1.spiReader[i]; // ��ֵ
        }
        uart1.to_trans.trans8[4 + uart1.devided_data_num] = uart1.devided_data_num;
        uart1.to_trans.trans8[5 + uart1.devided_data_num] = 0xEE;
        HAL_UART_Transmit_DMA(&huart1, uart1.to_trans.trans8, 6 + uart1.devided_data_num);
    }
}




// ��ѭ��ɨ��ִ�г���1ms����һ�Σ������������ݱ�����1ms��ִ����
// ϵͳʱ��Ƶ����72MHz����ע��
void board_ticks_handle(void) // ��ѭ����һֱɨ��ִ�е����ݣ�ÿ��ɨ��ִ��һ��
{
    //�������ۣ�����tick_stepΪ��׼��ÿѭ��һ��ִ��һ������
    switch(sys.tick_step) // �����趨ÿ�������һ�Σ���ÿ����������һ��
    {
        case 0:
            break; // ���ʱ��������sysÿ1ms��һ��ʱ���������������������ִ��ʱ�����ܴ���1ms�����������
        case 1:
            sys.tick_step = 2; // ִ�����ⲽ��һ��ɨ��ȥ��һ����Ŀǰ��������ɨ�贮���жϲ���
            to_read_spi(); // ��ȡSPI
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
            check_clc(); // �᲻��̫���ˣ�����CPU��
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

void board_quick(void)  // ÿ��ѭ�������ã����ڲ���������ת��
{
    receive_String_Handle(); // ÿ��ѭ�������ã������������� 
    receive_String_Handle_devided();
    screen_trans();
}

void board_slow(void)
{
    if(sys.sys_slow > sys.sys_time)
    {
        return;
    }
    sys.sys_slow = sys.sys_time + 100; // �жϺ���ÿ1ms����һ�Σ�����ʱsys_time+1��������������100msִ��һ��
    // HAL_UART_Transmit_DMA(&huart1, uart1.RXBuffer, uart1.Rx_Count);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
}



// �����������淢����
void zeros_ones_trans_report(void)
{
     // EE B1 52 00 09 00 01 30 3B D5 FD B3 A3 3B FF FC FF FF   ����ȷ��
                    
     // EE B1 52 00 09 00 01 30 3B D2 EC B3 A3 3B FF FC FF FF   ������
    
    ErrorStatus judge = SUCCESS;
    
    if(uart2.zeros_ones_report == SUCCESS)
    {
        //
        uart2.to_trans.trans64[0] = 0x300100090052B1EE;
        uart2.to_trans.trans8[8] = 0x3B;
        if(uart2.error_array[uart2.report_step] != 0) // ��ǰ����ֵ
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
        HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 18); // ���͸���
        while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
        {
            // �ж��Ƿ��꣬�����Դ���
        }
        uart2.report_step++;
        if(uart2.report_step == 64) // ����
        {
            uart2.zeros_ones_report = ERROR;
            
            // �չټ�飺�Ƿ�ȫ��
            if(uart2.all_correct_normal == SUCCESS)
            {
                // EE B1 12 00 0A 00 06 00 08 B4 E6 B4 A2 D5 FD B3 A3 00 07 00 08 B4 E6 B4 A2 D5 FD B3 A3 FF FC FF FF 
                // EE B1 12 00 09 00 06 00 08 B2 E2 CA D4 D5 FD B3 A3 00 07 00 08 B2 E2 CA D4 D5 FD B3 A3 FF FC FF FF 
                uart2.to_trans.trans64[0] = 0x000600090012B1EE; // 8λ
                uart2.to_trans.trans64[1] = 0xB3FDD5A2B4E6B408; // 8λ
                uart2.to_trans.trans64[2] = 0xB4E6B408000700A3; // 8λ
                uart2.to_trans.trans64[3] = 0xFFFCFFA3B3FDD5A2;
                uart2.to_trans.trans8[32] = 0xFF;
                HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 33);
                while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
                {
                    // �ж��Ƿ��꣬�����Դ���
                }
            }
            else
            {
                // EE B1 12 00 0A 00 06 00 08 B3 F6 CF D6 B4 ED CE F3 00 07 00 08 B3 F6 CF D6 B4 ED CE F3 FF FC FF FF 
                uart2.to_trans.trans64[0] = 0x000600090012B1EE; // 8λ
                uart2.to_trans.trans64[1] = 0xCEEDB4D6CFF6B308; // 8λ
                uart2.to_trans.trans64[2] = 0xCFF6B308000700F3; // 8λ
                uart2.to_trans.trans64[3] = 0xFFFCFFF3CEEDB4D6; // 8λ
                uart2.to_trans.trans8[32] = 0xFF;
                HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 33);
                while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
                {
                    // �ж��Ƿ��꣬�����Դ���
                }
                uart2.all_correct_normal = SUCCESS;
            }
        }
    }
    
    
    // EE B1 52 00 0A 00 01 30 3B D5 FD B3 A3 3B FF FC FF FF   ����ȷ��
                    
    // EE B1 52 00 0A 00 01 30 3B D2 EC B3 A3 3B FF FC FF FF   ������
    
    
     if(uart2.zeros_ones_report_cut == SUCCESS)
    {
        //
        uart2.to_trans.trans64[0] = 0x3001000A0052B1EE;
        uart2.to_trans.trans8[8] = 0x3B;
        if(uart2.error_array[uart2.report_step] != 0) // ��ǰ����ֵ
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
        HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 18); // ���͸���
        while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
        {
            // �ж��Ƿ��꣬�����Դ���
        }
        uart2.report_step++;
        if(uart2.report_step == 64) // ����
        {
            uart2.zeros_ones_report_cut = ERROR;
            
            // �չټ�飺�Ƿ�ȫ��
            if(uart2.all_correct_cut_off == SUCCESS)
            {
                // EE B1 12 00 0A 00 06 00 08 B4 E6 B4 A2 D5 FD B3 A3 00 07 00 08 B4 E6 B4 A2 D5 FD B3 A3 FF FC FF FF 
                uart2.to_trans.trans64[0] = 0x0006000A0012B1EE; // 8λ
                uart2.to_trans.trans64[1] = 0xB3FDD5A2B4E6B408; // 8λ
                uart2.to_trans.trans64[2] = 0xB4E6B408000700A3; // 8λ
                uart2.to_trans.trans64[3] = 0xFFFCFFA3B3FDD5A2;
                uart2.to_trans.trans8[32] = 0xFF;
                HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 33);
                while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
                {
                    // �ж��Ƿ��꣬�����Դ���
                }
            }
            else
            {
                // EE B1 12 00 0A 00 06 00 08 B3 F6 CF D6 B4 ED CE F3 00 07 00 08 B3 F6 CF D6 B4 ED CE F3 FF FC FF FF 
                uart2.to_trans.trans64[0] = 0x0006000A0012B1EE; // 8λ
                uart2.to_trans.trans64[1] = 0xCEEDB4D6CFF6B308; // 8λ
                uart2.to_trans.trans64[2] = 0xCFF6B308000700F3; // 8λ
                uart2.to_trans.trans64[3] = 0xFFFCFFF3CEEDB4D6;
                uart2.to_trans.trans8[32] = 0xFF;
                HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 33);
                while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
                {
                    // �ж��Ƿ��꣬�����Դ���
                }
                uart2.all_correct_cut_off = SUCCESS;
            }
        }
    }
    
}
















