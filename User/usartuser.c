#include <stdio.h>
#include "usartuser.h"

/* usartuser.c�ļ������ڲ���������usart.c��usart.h������������CubeMX���ɣ��Զ��庯���������������Ż� */

usart_type uart1; // ʵ����
usart_type uart2; // ʵ����
screen_type screen;

void usartuser_Init(void) // �Զ��崮�ں�����ʼ��
{
    uart1.uart_Receive_String = ERROR;
    uart1.uart_to_check_spi = ERROR;
    uart1.uart_time = 0;
    uart1.tired_test = ERROR;
    uart1.num_of_error.num_of_error_16[0] = 0;
    uart1.num_of_test.num_of_test_16[0] = 0;
    uart1.data_check_flag = SUCCESS;
    uart1.cut_off_test = ERROR;
    uart1.receiveforward = ERROR;
    HAL_UART_Receive_IT(&huart1, uart1.Rx_step_buf, 1);
    uart1.devided_data_get = ERROR;
    uart1.devided_receive_data = ERROR;
    uart1.clc_order = ERROR;
    uart1.cut_off_continue = ERROR;
    uart1.check_flag_on = ERROR;
    uart1.all_to_transmit = ERROR;
    uart1.all_to_transmit_start = ERROR;
    uart1.spi_to_change = ERROR;
    uart1.zeros_trans = ERROR;
    uart1.ones_trans = ERROR;
    uart1.ones_test = ERROR;
    uart1.zeros_test = ERROR;
    uart1.ones_to_test = ERROR;
    uart1.zeros_to_test = ERROR;
    uart1.zeros_trans_tired = ERROR;
    uart1.ones_trans_tired = ERROR;
    uart1.ones_test_tired = ERROR;
    uart1.zeros_test_tired = ERROR;
    uart1.ones_to_test_tired = ERROR;
    uart1.zeros_to_test_tired = ERROR;
    uart1.to_tired_test = ERROR;
    uart1.zeros_tired_finish = ERROR;
    uart1.ones_tired_finish = ERROR;
    uart1.tired_trans_report = ERROR;
    uart1.trans_with_addr = ERROR;
    uart1.receive_with_addr = ERROR;
    uart1.cut_off_continue_next = ERROR;
    uart1.cut_off_to_continue_next = ERROR;
    uart1.zeros_ones_report = ERROR;
    uart1.cut_off_to_ones = ERROR;
    uart1.cut_off_to_zeros = ERROR;
    uart1.zeros_ones_report_cut = ERROR;
    uart1.all_correct_cut_off = ERROR;
    uart1.all_correct_normal = ERROR;
    
    uart2.uart_Receive_String = ERROR;
    uart2.uart_to_check_spi = ERROR;
    uart2.uart_time = 0;
    uart2.tired_test = ERROR;
    uart2.num_of_error.num_of_error_16[0] = 0;
    uart2.num_of_test.num_of_test_16[0] = 0;
    uart2.data_check_flag = SUCCESS;
    uart2.cut_off_test = ERROR;
    uart2.receiveforward = ERROR;
    HAL_UART_Receive_IT(&huart2, uart2.Rx_step_buf, 1);
    uart2.devided_data_get = ERROR;
    uart2.devided_receive_data = ERROR;
    uart2.clc_order = ERROR;
    uart2.cut_off_continue = ERROR;
    uart2.check_flag_on = ERROR;
    uart2.all_to_transmit = ERROR;
    uart2.all_to_transmit_start = ERROR;
    uart2.spi_to_change = ERROR;
    uart2.tired_test_by_screen = ERROR;
    uart2.zeros_trans = ERROR;
    uart2.ones_trans = ERROR;
    uart2.ones_test = ERROR;
    uart2.zeros_test = ERROR;
    uart2.ones_to_test = ERROR;
    uart2.zeros_to_test = ERROR;
    uart2.zeros_trans_tired = ERROR;
    uart2.ones_trans_tired = ERROR;
    uart2.ones_test_tired = ERROR;
    uart2.zeros_test_tired = ERROR;
    uart2.ones_to_test_tired = ERROR;
    uart2.zeros_to_test_tired = ERROR;
    uart2.to_tired_test = ERROR;
    uart2.tired_trans_report = ERROR;
    uart2.trans_with_addr = ERROR;
    uart2.receive_with_addr = ERROR;
    uart2.cut_off_continue_next = ERROR;
    uart2.cut_off_to_continue_next = ERROR;
    uart2.zeros_ones_report = ERROR;
    uart2.cut_off_to_ones = ERROR;
    uart2.cut_off_to_zeros = ERROR;
    uart2.zeros_ones_report_cut = ERROR;
    uart2.all_correct_cut_off = SUCCESS;
    uart2.all_correct_normal = SUCCESS;
    
    
    screen.clear_all = ERROR;
    screen.clear_end = SUCCESS;
    screen.step = 0;
    screen.trans = ERROR;
    screen.show_receive = 0;
    screen.to_show_receive = ERROR;
    screen.to_show = ERROR;
    
    for(int i = 0; i < 2048; i++)
    {
        uart1.zeros_buffer[i] = 0x00;
        uart1.ones_buffer[i] = 0xFF; // ����ȫ��ȫһ����
        uart2.zeros_buffer[i] = 0x00;
        uart2.ones_buffer[i] = 0xFF; // ����ȫ��ȫһ����
    }
    
    uart1.zeros_ones_range_num = 0;
    uart2.zeros_ones_range_num = 0;
    
}

void arrayCopy(uint8_t* aa, uint8_t* bb, uint16_t num) // �����ַ���ʱ�临�Ӷ�O(num)����֪���ʲ��ʺ����ж���ִ��
{
    for(int i = 0; i < num; i++)
    {
        aa[i] = bb[i];
    }
}

// ���ڽ��ջص��������������ڴ��ڲ�ֹ��һ�����ܣ�����Ӳ��ģ�塣
// ��ǰָ���趨����ʼA5 5A����βEE��ָ������ʼ���棺
// 0x20���������ݣ�0x10���������ݣ�0x30������ƣ�Ͳ��ԣ�0x50���ر�ƣ�Ͳ���
// �շ���ƣ�Ͳ��ԡ��ϵ���ԡ���SPI����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) // �Դ����������"stm32f3xx_hal_uart.h"һǧ�İٶ�������
{
    if(huart->Instance == USART1) // �ж��ĸ��������ģ��˴��Ǵ���1��PC����
    {
        // �����ݣ�A5 5A NN DD DD DD CC CC EE��N�������������ɴ��ж������Ƿ�����������������趨�����ж�
        // A55A�ǿ�ʼEOH��EE�ǽ�����DD�������ݣ�NN��ȫ��������CC������������У��ʹ�ã�
        // ���ݴ��䲿�֣���λ���봫�䣺0xA55A + �������� + �������� + ���������� + 0xEE
        if(uart1.uart_time < sys.sys_time) // ����ʱ��
        {
            uart1.Rx_current_step = 0x00; // ʧ�ڵ�ն
        }
        uart1.uart_time = sys.sys_time + 500; // ����500ms�����һ�ν��գ�ʧ�ڷ���ն
        switch(uart1.Rx_current_step) // ���ݽ��������ж����
        {
            case 0x00: // 0��λ����ʼ̬������������
                if(uart1.Rx_step_buf[0] == 0xA5) // ������ʼλ�����յ�����ȷ�����ٿ�����
                {
                    uart1.Rx_current_step = 0x01; // ��һ��
                    uart1.Rx_Count = 0x00; // ��������λ׼����ʼ����
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // ������һλ������
                    uart1.Rx_Count++; // ��������++��ȷ���ж�ʱ���鵱λΪ��
                }
                // �ڴ˴�Ԥ�������������趨����else if��
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x01: // 1��λ�����յ��������ݵ�һ����ʼλ����������û�еڶ���
                if(uart1.Rx_step_buf[0] == 0x5A) // �ڶ�����ʼ��Ҳ�ԣ��ǾͿ�ʼ׼�����հɡ�
                {
                    uart1.Rx_current_step = 0x02; // ��һ��
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                    // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
                }
                else
                {
                    uart1.Rx_current_step = 0x00; // �ϴ����ˣ���ȥ����
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                // HAL_UART_Transmit_DMA(&huart1, &uart1.Rx_current_step, 1);
                break;
            case 0x02: // 2��λ��������ʼλ���յ��ˣ�ָ��ȷ�ϣ���������м������ݣ�
                //������λ���ƣ��������256�ֽڣ��������������ܷ���չ��65536�ֽڣ����Ǿ��������ֽ�����������
                //��չ65536�ֽ���64KB��ARM�ڴ���ܲ������������SPI���ձ߷���Ԥ���ù��ܡ�
                uart1.Rx_current_step = 0x03; // ��һ��
                uart1.RX_Num.Rx_Num_8[0] = uart1.Rx_step_buf[0] + 3; // +3����Ϊǰ�������ֽڣ�����չ��Ϊ+4����0��ʼ������ʵ��Ӧ����0xEE��һ�ֽ�
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0];  
                uart1.Rx_Count++;       
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                // HAL_UART_Transmit_DMA(&huart1, &uart1.Rx_current_step, 1);
                // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
                break;                    
            case 0x03: // 3��λ������ָ���������
                if(uart1.Rx_step_buf[0] == 0x10) // ָ���������
                {
                    uart1.Rx_current_step = 0x04; // ��һ��
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x11) // ָ�ȫ�����
                {
                    uart1.Rx_current_step = 0x1B; // ������������ 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x12) // ָ�ȫһ����
                {
                    uart1.Rx_current_step = 0x1C; // ������������ 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x13) // ָ��Զ����ַ���ͺ���
                {
                    uart1.Rx_current_step = 0x1D; // ������������ 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x20) // ָ���ȡ����
                {
                    uart1.Rx_current_step = 0x05; // ������������ 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x21) // ָ���ȡ����2
                {
                    uart1.Rx_current_step = 0x0C; // ������������ 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x22) // ָ������ȡ����
                {
                    uart1.Rx_current_step = 0x0E; // ���ղ���������� 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                } 
                else if(uart1.Rx_step_buf[0] == 0x23) // ָ������ȡ���ݣ�������
                {
                    uart1.Rx_current_step = 0x16; // ���ղ���������� 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x24) // ָ��Զ����ַ��������
                {
                    uart1.Rx_current_step = 0x21; // �Զ����ַ�������� 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x30) // ָ�����ƣ�Ͳ��ԣ��������������Ͷ�ȡͳ�ƴ�����
                {
                    uart1.Rx_current_step = 0x06;   
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x40) // ָ��ϵ����
                {
                    uart1.Rx_current_step = 0x0B;
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x50) // ָ��ر�ƣ�Ͳ���
                {
                    uart1.Rx_current_step = 0x07;
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x60) // ָ�������
                {
                    uart1.Rx_current_step = 0x0F;
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x70) // ָ����SPIת������
                {
                    uart1.Rx_current_step = 0x14;
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x80) // ָ��޸�SPI�����ʣ�Beta��
                {
                    uart1.Rx_current_step = 0x19;
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                
                break;
            case 0x04: // 4��λ�����ݽ��գ�ֱ������,�����������Ǵ��ڷ��͵����һ���жϣ����Գ�һ�㣿
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                // uart1.Rx_Count++;
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.RX_Num_16[0])) // �����ˣ�
                {
                    // ���У��λ��
                    if(uart1.RXBuffer[uart1.Rx_Count - 1] == uart1.RX_Num.RX_Num_16[0] - 5) // У��λ��ȷ����
                    {
                        uart1.Rx_Count++;
                        uart1.Rx_current_step = 0x00;// ��������
                        uart1.contentLength = uart1.Rx_Count; // �������ݳ���
                        arrayCopy(uart1.getArray, uart1.RXBuffer, uart1.Rx_Count);
                        uart1.uart_Receive_String = SUCCESS; // ��־λ�趨
                    }
                    else // У��λ����ȷ�����д��󷵻ز���
                    {
                        uart1.check_flag_on = SUCCESS;
                        uart1.Rx_current_step = 0x00;// ��������
                    }
                }
                else
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x04; // ��������
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x05: // 5��λ����ȡ���ݣ��趨��־λ��Ŀǰ��֧��һ��һ����Ԥ���෢������������趨
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.Rx_Num_8[0]))
                {   
                    uart1.uart_to_check_spi = SUCCESS; // ��־λ�趨��û����λ����
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x00; // ����
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x06: // 6��λ��ƣ�Ͳ������ݽ���1
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.num_of_error.num_of_error_16[0] = 0;
                uart1.num_of_test.num_of_test_16[0] = 0;
                uart1.num_of_test_set.num_of_test_set_8[1] = uart1.Rx_step_buf[0]; // ��/�Ͱ�λ
                uart1.Rx_current_step = 0x08;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x07: // 7��λ����ֹƣ�Ͳ���
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.Rx_Num_8[0]))
                {
                    uart1.num_of_error.num_of_error_16[0] = 0;
                    uart1.num_of_test.num_of_test_16[0] = 0;
                    uart1.tired_test = ERROR;
                    spi1.tired_test_Read = ERROR;
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x00; // ����
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x08: // 8��λ��ƣ�Ͳ������ֽ���2
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.num_of_test_set.num_of_test_set_8[0] = uart1.Rx_step_buf[0]; // ����ģ���/�Ͱ�λ
                spi1.tired_test_Read = ERROR;
                // uart1.tired_test = ERROR; // ��־λ�趨��û����λ������û�к�������               
                uart1.Rx_current_step = 0x0A;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x09: // 9��λ���ȴ�����
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.Rx_Num_8[0]))
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x00; // ����
                }
                else
                {
                    usartuser_Init(); // û����λ��ȫ��һ��
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x0A: // 10��λ��ƣ�Ͳ��Եȴ�У��λ
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                if(uart1.Rx_step_buf[0] == uart1.RX_Num.RX_Num_16[0] - 5) // ���㹫ʽ
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x09;
                    uart1.tired_test = SUCCESS; // ��־λ�趨��û����λ������û�к�������
                    uart1.to_tired_test = SUCCESS;
                }
                else // У��λ����ȷ�����д��󷵻ز���
                {
                    uart1.check_flag_on = SUCCESS;
                    uart1.Rx_current_step = 0x00;// ��������
                }                                         
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x0B: // 11��λ���ϵ���ԣ����նϵ�ʱ��1
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.cut_down_time.cutdown_8[1] = uart1.Rx_step_buf[0];
                uart1.Rx_Count++;
                uart1.Rx_current_step= 0x0D;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x0C: // 12��λ����ȡ����ֱ�Ӷ�     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.Rx_Num_8[0]))
                {
                    uart1.receiveforward = SUCCESS; // ��־λ�趨��û����λ����
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x00; // ����
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x0D: // 13��λ���ϵ���ԣ����նϵ�ʱ��2
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.cut_down_time.cutdown_8[0] = uart1.Rx_step_buf[0];
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x15; // ȥУ��
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x0E: // 14��λ�����ʽ�������ݣ��˴��������1
                uart1.devided_datanum.datanum_8[1] = uart1.Rx_step_buf[0];     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x10;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x0F: // 15��λ�����ʽ�������ݣ��˴��������1
                uart1.devided_datanum.datanum_8[1] = uart1.Rx_step_buf[0];     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x11;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x10: // 16��λ�����ʽ�������ݣ��˴��������2
                uart1.devided_datanum.datanum_8[0] = uart1.Rx_step_buf[0];     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x12; // У��λ
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x11: // 17��λ�����ʽ�������ݣ��˴��������2
                uart1.devided_datanum.datanum_8[0] = uart1.Rx_step_buf[0];     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x13; // ���ݽ���
                uart1.devided_data_num = 0;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x12: // 18��λ����ȡ����У��λ
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                if(uart1.Rx_step_buf[0] == uart1.RX_Num.RX_Num_16[0] - 5) // ���㹫ʽ
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x09;
                    uart1.devided_data_get = SUCCESS;
                }                                             
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x13: // 19��λ��������ݰ���������
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.devided_data[uart1.devided_data_num] = uart1.Rx_step_buf[0];
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.RX_Num_16[0])) // �����ˣ�
                {
                    // ���У��λ��
                    if(uart1.RXBuffer[uart1.Rx_Count - 1] == uart1.RX_Num.RX_Num_16[0] - 5) // У��λ��ȷ����
                    {
                        uart1.Rx_Count++;
                        uart1.devided_data_num++;
                        uart1.devided_data_num -= 2; // ȥ�������λ��У��ͽ�β��
                        uart1.Rx_current_step = 0x00;// ��������
                        uart1.contentLength = uart1.Rx_Count; // �������ݳ���
                        arrayCopy(uart1.getArray, uart1.RXBuffer, uart1.Rx_Count);
                        arrayCopy(spi1.devided_data, uart1.devided_data, uart1.devided_data_num);
                        uart1.devided_receive_data = SUCCESS; // ��־λ�趨
                    }
                    else // У��λ����ȷ�����д��󷵻ز���
                    {
                        uart1.check_flag_on = SUCCESS;
                        uart1.Rx_current_step = 0x00;// ��������
                    }
                }
                else
                {
                    uart1.Rx_Count++;
                    uart1.devided_data_num++;
                    uart1.Rx_current_step = 0x13; // ��������
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x14: // 20��λ���������
                uart1.clc_order = SUCCESS;
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x09;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x15: // 21��λ���ϵ�У��λ
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                if(uart1.Rx_step_buf[0] == uart1.RX_Num.RX_Num_16[0] - 5) // ���㹫ʽ
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x09;
                    uart1.cut_off_test = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x16: // 22��λ��������ȡ���1
                uart1.devided_datanum.datanum_8[1] = uart1.Rx_step_buf[0];     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x17;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x17: // 23��λ��������ȡ���2
                uart1.devided_datanum.datanum_8[0] = uart1.Rx_step_buf[0];     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x18; // У��λ
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x18: // 24��λ��������ȡУ��λ
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                if(uart1.Rx_step_buf[0] == uart1.RX_Num.RX_Num_16[0] - 5) // ���㹫ʽ
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x09;
                    uart1.all_to_transmit_start = SUCCESS;
                    uart1.get_data_all_in = 0x0000;
                }     
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x19: // 25��λ���޸�SPI�����ʲ������գ�8��ѡ�񣬽�һ�ֽڣ�
                uart1.spi_changer = uart1.Rx_step_buf[0]; // �ò���ΪԤ��Ƶ����С������
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x1A; // У��λ
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x1A: // 26��λ���޸�SPI������У��λ
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                if(uart1.Rx_step_buf[0] == uart1.RX_Num.RX_Num_16[0] - 5) // ���㹫ʽ
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x09;
                    uart1.spi_to_change = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x1B: // 27��λ��ȫ����ԣ��޲�����ֱ�ӽ��ս���λ
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.Rx_Num_8[0]))
                {
                    uart1.zeros_to_test = SUCCESS; // ��־λ�趨��û����λ����
                    uart1.zeros_trans = SUCCESS;
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x00; // ����
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x1C: // 28��λ��ȫһ���ԣ��޲�����ֱ�ӽ��ս���λ
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.Rx_Num_8[0]))
                {
                    uart1.ones_to_test = SUCCESS; // ��־λ�趨��û����λ����
                    uart1.ones_trans = SUCCESS;
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x00; // ����
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x1D: // 29��λ���Զ������ݷ��ͣ���ַ��λ���޶�0x00,0x01
                uart1.get_addr_set_high = uart1.Rx_step_buf[0];
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x1E;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x1E: // 30��λ���Զ������ݷ��ͣ���ַ��λ
                uart1.get_addr_set.addr_8[1] = uart1.Rx_step_buf[0];
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x1F;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x1F: // 31��λ���Զ������ݷ��ͣ���ַ��λ
                uart1.get_addr_set.addr_8[0] = uart1.Rx_step_buf[0];
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x20;
                uart1.devided_data_num = 0;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x20: // 32��λ�����ݽ��գ�ֱ�������������������Ǵ��ڷ��͵����һ���жϣ����Գ�һ�㣿
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.devided_data[uart1.devided_data_num] = uart1.Rx_step_buf[0];
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.RX_Num_16[0])) // �����ˣ�
                {
                    // ���У��λ��
                    if(uart1.RXBuffer[uart1.Rx_Count - 1] == uart1.RX_Num.RX_Num_16[0] - 5) // У��λ��ȷ����
                    {
                        uart1.Rx_Count++;
                        uart1.devided_data_num++;
                        uart1.devided_data_num -= 2; // ȥ�������λ��У��ͽ�β��
                        uart1.Rx_current_step = 0x00;// ��������
                        uart1.contentLength = uart1.Rx_Count; // �������ݳ���
                        arrayCopy(uart1.getArray, uart1.RXBuffer, uart1.Rx_Count);
                        arrayCopy(spi1.devided_data, uart1.devided_data, uart1.devided_data_num);
                        uart1.trans_with_addr = SUCCESS; // ��־λ�趨
                    }
                    else // У��λ����ȷ�����д��󷵻ز���
                    {
                        uart1.check_flag_on = SUCCESS;
                        uart1.Rx_current_step = 0x00;// ��������
                    }
                }
                else
                {
                    uart1.Rx_Count++;
                    uart1.devided_data_num++;
                    uart1.Rx_current_step = 0x13; // ��������
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x21: // 33��λ���Զ������ݽ��գ���ַ��λ���޶�0x00,0x01
                uart1.get_addr_set_high = uart1.Rx_step_buf[0];
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x22;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x22: // 34��λ���Զ������ݽ��գ���ַ��λ
                uart1.get_addr_set.addr_8[1] = uart1.Rx_step_buf[0];
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x23;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x23: // 35��λ���Զ������ݽ��գ���ַ��λ
                uart1.get_addr_set.addr_8[0] = uart1.Rx_step_buf[0];
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x24;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x24: // 36��λ���Զ������ݽ��գ��ֳ�
                uart1.devided_data_num = uart1.Rx_step_buf[0]; // �ֳ��洢
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x25;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
            case 0x25: // 37��λ���Զ������ݽ��յ�У��λ
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // �ȱ�����˵
                if(uart1.Rx_step_buf[0] == uart1.RX_Num.RX_Num_16[0] - 5) // ���㹫ʽ
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x09; // ȥ����
                    uart1.receive_with_addr = SUCCESS;
                    uart1.get_data_all_in = 0x0000;
                }     
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // �������ý���
                break;
        }
    }
    
    // ����Ϊ�������������
    if(huart->Instance == USART2) // ����������
    {
        // 20241223��ǰ���ȣ���ѧ������ô��������ͽ��մ�����Ϣ����ʾ�����ˣ����ҵĺ���Ϣ������
        if(uart2.uart_time < sys.sys_time) // ����ʱ��
        {
            uart2.Rx_current_step = 0x00; // ʧ�ڵ�ն
        }
        uart2.uart_time = sys.sys_time + 500; // ����500ms�����һ�ν��գ�ʧ�ڷ���ն
        switch(uart2.Rx_current_step) // ���ݽ��������ж����
        {
            case 0x00: // 0��λ����ʼ̬������������
                if(uart2.Rx_step_buf[0] == 0xA5) // ������ʼλ�����յ�����ȷ�����ٿ�����
                {
                    uart2.Rx_current_step = 0x01; // ��һ��
                    uart2.Rx_Count = 0x00; // ��������λ׼����ʼ����
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // ������һλ������
                    uart2.Rx_Count++; // ��������++��ȷ���ж�ʱ���鵱λΪ��
                }
                // �ڴ˴�Ԥ�������������趨����else if��
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // �������ý���
                break;
            case 0x01: // 1��λ�����յ��������ݵ�һ����ʼλ����������û�еڶ���
                if(uart2.Rx_step_buf[0] == 0x5A) // �ڶ�����ʼ��Ҳ�ԣ��ǾͿ�ʼ׼�����հɡ�
                {
                    uart2.Rx_current_step = 0x02; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                    // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
                }
                else
                {
                    uart2.Rx_current_step = 0x00; // �ϴ����ˣ���ȥ����
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // �������ý���
                // HAL_UART_Transmit_DMA(&huart1, &uart1.Rx_current_step, 1);
                break;
            case 0x02: // 2��λ��������ʼλ���յ��ˣ�ָ��ȷ�ϣ���������м������ݣ�
                //������λ���ƣ��������256�ֽڣ��������������ܷ���չ��65536�ֽڣ����Ǿ��������ֽ�����������
                //��չ65536�ֽ���64KB��ARM�ڴ���ܲ������������SPI���ձ߷���Ԥ���ù��ܡ�
                uart2.Rx_current_step = 0x03; // ��һ��
                uart2.RX_Num.Rx_Num_8[0] = uart2.Rx_step_buf[0] + 3; // +3����Ϊǰ�������ֽڣ�����չ��Ϊ+4����0��ʼ������ʵ��Ӧ����0xEE��һ�ֽ�
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0];  
                uart2.Rx_Count++;       
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // �������ý���
                // HAL_UART_Transmit_DMA(&huart1, &uart1.Rx_current_step, 1);
                // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
                break;                    
            case 0x03: // 3��λ������ָ���������
                if(uart2.Rx_step_buf[0] == 0x10) // ָ���������
                {
                    uart2.Rx_current_step = 0x04; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                    uart2.devided_data_num = 0x00;
                }
                else if(uart2.Rx_step_buf[0] == 0x11) // ָ�ȫ���������
                {
                    uart2.Rx_current_step = 0x0D; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x12) // ָ�ȫһ��������
                {
                    uart2.Rx_current_step = 0x0E; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x21) // ָ���������
                {
                    uart2.Rx_current_step = 0x05; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x30) // ָ�ƣ�Ͳ���
                {
                    uart2.Rx_current_step = 0x06; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x40) // ָ��ϵ����1
                {
                    uart2.Rx_current_step = 0x08; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x41) // ָ��ϵ���ԣ�����
                {
                    uart2.Rx_current_step = 0x0A; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x42) // ָ��ϵ���ԣ��ϵ�
                {
                    uart2.Rx_current_step = 0x09; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x43) // ָ��ϵ����2
                {
                    uart2.Rx_current_step = 0x0B; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x45) // ָ��ϵ����ȫ��
                {
                    uart2.Rx_current_step = 0x0F; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x46) // ָ��ϵ����ȫһ���£�ȫ�ϵ�ָ��
                {
                    uart2.Rx_current_step = 0x10; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x50) // ָ�ƣ�Ͳ��Խ���
                {
                    uart2.Rx_current_step = 0x07; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x60) // ָ��������
                {
                    uart2.Rx_current_step = 0x0C; // ��һ��
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // �������ý���
                break;
            case 0x04: // 4��λ����������
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // �ȱ�����˵
                uart2.devided_data[uart2.devided_data_num] = uart2.Rx_step_buf[0];
                // uart1.Rx_Count++;
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.RX_Num_16[0])) // �����ˣ�
                {
                    // ���У��λ��
                    if(uart2.RXBuffer[uart2.Rx_Count - 1] == uart2.RX_Num.RX_Num_16[0] - 5) // У��λ��ȷ����
                    {
                        uart2.Rx_Count++;
                        uart2.devided_data_num++;
                        uart2.devided_data_num -= 2;
                        uart2.Rx_current_step = 0x00;// ��������
                        uart2.contentLength = uart2.Rx_Count; // �������ݳ���
                        arrayCopy(uart2.getArray, uart2.RXBuffer, uart2.Rx_Count);
                        arrayCopy(spi1.devided_data, uart2.devided_data, uart2.devided_data_num);
                        uart2.uart_Receive_String = SUCCESS; // ��־λ�趨
                    }
                    else // У��λ����ȷ�����д��󷵻ز���
                    {
                        uart2.check_flag_on = SUCCESS;
                        uart2.Rx_current_step = 0x00;// ��������
                    }
                }
                else
                {
                    uart2.Rx_Count++;
                    uart2.devided_data_num++;
                    uart2.Rx_current_step = 0x04; // ��������
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // �������ý���
                break;
            case 0x05: // 5��λ����ȡ����
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // �ȱ�����˵
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00; // ����
                    uart2.receiveforward = SUCCESS; // ��־λ�趨��û����λ����
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // �������ý���
                break;
            case 0x06: // 6��λ��ƣ�Ͳ���
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // �ȱ�����˵
                uart2.Rx_Count++;
                uart2.num_of_error.num_of_error_16[0] = 0;
                uart2.num_of_test.num_of_test_16[0] = 0;
                uart2.num_of_test_set.num_of_test_set_16[0] = 500;
                uart2.Rx_current_step = 0x07;
                spi1.tired_test_Read = ERROR;
                uart2.tired_test = SUCCESS;
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1);
                break;
            case 0x07: // 7��λ���ر�ƣ�Ͳ���
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // �ȱ�����˵
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.num_of_error.num_of_error_16[0] = 0;
                    uart2.num_of_test.num_of_test_16[0] = 0;
                    uart2.tired_test = ERROR;
                    spi1.tired_test_Read = ERROR;
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00; // ����
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // �������ý���
                break;
            case 0x08: // 8��λ���ϵ����1
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // �ȱ�����˵
                uart2.cut_down_time.cutdown_16[0] = 5000; // Ĭ��ֵ5000
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00;
                    uart2.cut_off_test = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1);
                break;
            case 0x09: // 9��λ���ϵ���Թص�
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // �ȱ�����˵
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00;
                    spi1.electric_turn_off = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1);
                break;
            case 0x0A: // 10��λ���ϵ���Կ���
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // �ȱ�����˵
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00;
                    spi1.electric_turn_on = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1);
                break;
            case 0x0B: // 11��λ���ϵ����2
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // �ȱ�����˵
                uart2.cut_down_time.cutdown_16[0] = 500; // Ĭ��ֵ500
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00;
                    uart2.cut_off_test = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1);
                break;
            case 0x0C:
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // �ȱ�����˵
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00;
                    screen.clear_all = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1);
                break;
            case 0x0D: // 13��λ��ȫ����ԣ��޲�����ֱ�ӽ��ս���λ
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // �ȱ�����˵
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.zeros_to_test = SUCCESS; // ��־λ�趨��û����λ����
                    uart2.zeros_trans = SUCCESS;
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00; // ����
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // �������ý���
                break;
            case 0x0E: // 14��λ��ȫһ���ԣ��޲�����ֱ�ӽ��ս���λ
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // �ȱ�����˵
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.ones_to_test = SUCCESS; // ��־λ�趨��û����λ����
                    uart2.ones_trans = SUCCESS;
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00; // ����
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // �������ý���
                break;
            case 0x0F: // 15��λ���ϵ�ȫ����ԣ��޲�����ֱ�ӽ��ս���λ
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // �ȱ�����˵
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.cut_off_to_zeros = SUCCESS; // ��־λ�趨��û����λ����
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00; // ����
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // �������ý���
                break;
            case 0x10: // 16��λ���ϵ�ȫһ���ԣ��޲�����ֱ�ӽ��ս���λ
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // �ȱ�����˵
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.cut_off_to_ones = SUCCESS; // ��־λ�趨��û����λ����
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00; // ����
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // �������ý���
                break;
        }
    }
}

void usart_finish_ithandle(UART_HandleTypeDef *huart) // �����ж�
{
     if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE)) // �ж����������Ƿ��ǿ����ж�?
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        uart1.Rx_current_step = 0; // �����жϣ�����ǿ�Ƹ�λ����ֹ��������
        uint8_t aa = uart1.Rx_Count;
        HAL_UART_Transmit_DMA(&huart1, &aa, 1);
    }
}

// �����жϻص���������������ʱ��ȷ����һ�����������˵��DMA�����Բ��ÿ�IT
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(uart1.all_to_transmit == SUCCESS)
    {
        uart1.get_data_all_in++;
        if(uart1.get_data_all_in < uart1.devided_datanum.datanum_16[0])
        {
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
        else
        {
            uart1.all_to_transmit = ERROR;
        }
    }
    if(huart->Instance == USART2)
    {
        if(uart2.tired_test_by_screen == SUCCESS) // ���ȷ���
        {
            uart2.tired_test_by_screen = ERROR;
            // EE B1 32 00 02 00 02 00 00 02 NN NN FF FC FF FF 
            uart2.to_trans.trans64[0] = 0x000200020032B1EE;
            uart2.to_trans.trans16[4] = 0x0200;
            uart2.to_trans.trans8[10] = uart2.num_of_error.num_of_error_8[1];
            uart2.to_trans.trans8[11] = uart2.num_of_error.num_of_error_8[0];
            uart2.to_trans.trans16[6] = 0xFCFF;
            uart2.to_trans.trans16[7] = 0xFFFF;
            HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 16);
        }
        
        if(screen.clear_end == ERROR)
        {
            screen.trans = SUCCESS;
            if(screen.step == 10)
            {
                screen.clear_end = SUCCESS;
            }
        }
        if(screen.to_show_receive == SUCCESS)
        {
            screen.to_show = SUCCESS;
            screen.to_show_receive = ERROR;
        }
    }
}

void screen_trans(void)
{
    if(screen.trans == SUCCESS)
    {
        screen.trans = ERROR;
        switch(screen.step)
        {
                case 1: // 1���������
                    // EE B1 33 00 02 00 02 00 FF FC FF FF  (12)
                    uart2.to_trans.trans64[0] = 0x000200020033B1EE;
                    uart2.to_trans.trans32[2] = 0xFFFFFCFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 12);
                    break;
                case 2: // 2�����3���������ı�
                    // EE B1 12 00 03 00 06 00 01 20 00 07 00 01 20 00 08 00 01 20 FF FC FF FF  (24)
                    // EE B1 12 00 04 00 06 00 01 20 00 07 00 01 20 00 08 00 01 20 FF FC FF FF 
                    // EE B1 10 00 03 00 06 FF FC FF FF 
                    uart2.to_trans.trans64[0] = 0xFF0600030010B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
                case 3: // 2�����3���������ı�
                    uart2.to_trans.trans64[0] = 0xFF0700030010B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
                case 4: // 2�����3���������ı�
                    uart2.to_trans.trans64[0] = 0xFF0800030010B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
                case 5: // 3�����4���������ı�
                    // EE B1 12 00 04 00 06 00 01 20 00 07 00 01 20 00 08 00 01 20 FF FC FF FF  (24)
                    uart2.to_trans.trans64[0] = 0xFF0700040010B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
                case 6: // 3�����4���������ı�
                    // EE B1 12 00 04 00 06 00 01 20 00 07 00 01 20 00 08 00 01 20 FF FC FF FF  (24)
                    uart2.to_trans.trans64[0] = 0xFF0800040010B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
                case 7: // 3�����4���������ı�
                    // EE B1 12 00 04 00 06 00 01 20 00 07 00 01 20 00 08 00 01 20 FF FC FF FF  (24)
                    uart2.to_trans.trans64[0] = 0xFF0900040010B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
                case 8: // 9�ű�����
                    // EE B1 53 00 09 00 01 FF FC FF FF (11)
                    uart2.to_trans.trans64[0] = 0xFF0100090053B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
                case 9:
                    // EE B1 53 00 0A 00 01 FF FC FF FF (11)
                    uart2.to_trans.trans64[0] = 0xFF01000A0053B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
        }
    }
    if(screen.to_show == SUCCESS)
    {
        screen.to_show = ERROR;
        switch(screen.show_receive)
        {
            case 1:
                // EE B1 10 00 03 00 08 D5 FD C8 B7 FF FC FF FF   (15)
                uart2.to_trans.trans64[0] = 0xD50800030010B1EE;
                uart2.to_trans.trans32[2] = 0xFFB7C8FD;
                uart2.to_trans.trans16[6] = 0xFFFC;
                uart2.to_trans.trans8[14] = 0xFF;
                HAL_UART_Transmit_DMA(&huart2,uart2.to_trans.trans8, 15);
                break;
            case 2:
                // EE B1 10 00 03 00 08 B4 ED CE F3 FF FC FF FF   (15)
                uart2.to_trans.trans64[0] = 0xB40800030010B1EE;
                uart2.to_trans.trans32[2] = 0xFFF3CEED;
                uart2.to_trans.trans16[6] = 0xFFFC;
                uart2.to_trans.trans8[14] = 0xFF;
                HAL_UART_Transmit_DMA(&huart2,uart2.to_trans.trans8, 15);
                break;
        }
        screen.show_receive = 0;
    }
}
