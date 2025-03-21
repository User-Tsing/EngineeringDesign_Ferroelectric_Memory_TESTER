#include <stdio.h>
#include "usartuser.h"

/* usartuser.c文件，串口操作，基于usart.c和usart.h，上述两者由CubeMX生成，自定义函数单独出来自立门户 */

usart_type uart1; // 实例化
usart_type uart2; // 实例化
screen_type screen;

void usartuser_Init(void) // 自定义串口函数初始化
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
        uart1.ones_buffer[i] = 0xFF; // 构建全零全一向量
        uart2.zeros_buffer[i] = 0x00;
        uart2.ones_buffer[i] = 0xFF; // 构建全零全一向量
    }
    
    uart1.zeros_ones_range_num = 0;
    uart2.zeros_ones_range_num = 0;
    
}

void arrayCopy(uint8_t* aa, uint8_t* bb, uint16_t num) // 复制字符，时间复杂度O(num)，不知道适不适合在中断里执行
{
    for(int i = 0; i < num; i++)
    {
        aa[i] = bb[i];
    }
}

// 串口接收回调函数，问题在于串口不止那一个功能，不能硬套模板。
// 当前指令设定：起始A5 5A，结尾EE，指令在起始后面：
// 0x20：接收数据；0x10：发送数据；0x30：开启疲劳测试；0x50：关闭疲劳测试
// 收发、疲劳测试、断电测试、改SPI速率
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) // 自带函数，详见"stm32f3xx_hal_uart.h"一千四百多行左右
{
    if(huart->Instance == USART1) // 判断哪个串口来的，此处是串口1（PC？）
    {
        // 传数据：A5 5A NN DD DD DD CC CC EE，N是数据数量，由此判定传输是否结束，避免了另外设定空闲中断
        // A55A是开始EOH，EE是结束，DD是真数据，NN是全数据量，CC是真数据量（校验使用）
        // 数据传输部分，上位机请传输：0xA55A + 数据总量 + 传送数据 + 传送数据量 + 0xEE
        if(uart1.uart_time < sys.sys_time) // 检验时间
        {
            uart1.Rx_current_step = 0x00; // 失期当斩
        }
        uart1.uart_time = sys.sys_time + 500; // 须在500ms内完成一次接收，失期法皆斩
        switch(uart1.Rx_current_step) // 根据接收内容判定情况
        {
            case 0x00: // 0号位，初始态，检查接收内容
                if(uart1.Rx_step_buf[0] == 0xA5) // 接收起始位，已收到，不确定，再看看？
                {
                    uart1.Rx_current_step = 0x01; // 下一步
                    uart1.Rx_Count = 0x00; // 计数器复位准备开始计数
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 保存这一位进数组
                    uart1.Rx_Count++; // 保存完请++，确保中断时数组当位为空
                }
                // 在此处预留其他操作的设定！（else if）
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x01: // 1号位，已收到接收数据第一个起始位，继续看有没有第二个
                if(uart1.Rx_step_buf[0] == 0x5A) // 第二个起始符也对？那就开始准备接收吧。
                {
                    uart1.Rx_current_step = 0x02; // 下一步
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                    // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
                }
                else
                {
                    uart1.Rx_current_step = 0x00; // 认错人了，回去重认
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                // HAL_UART_Transmit_DMA(&huart1, &uart1.Rx_current_step, 1);
                break;
            case 0x02: // 2号位，两个起始位都收到了，指令确认，看看这次有几个数据？
                //由于线位限制，数据最多256字节，难评，后续看能否扩展到65536字节？（那就是两个字节是数据量）
                //扩展65536字节是64KB，ARM内存可能不够，必须配合SPI边收边发。预留该功能。
                uart1.Rx_current_step = 0x03; // 下一步
                uart1.RX_Num.Rx_Num_8[0] = uart1.Rx_step_buf[0] + 3; // +3是因为前面有三字节，如扩展改为+4，从0开始所以其实对应的是0xEE那一字节
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0];  
                uart1.Rx_Count++;       
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                // HAL_UART_Transmit_DMA(&huart1, &uart1.Rx_current_step, 1);
                // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
                break;                    
            case 0x03: // 3号位，检验指令，分类讨论
                if(uart1.Rx_step_buf[0] == 0x10) // 指令：发送数据
                {
                    uart1.Rx_current_step = 0x04; // 下一步
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x11) // 指令：全零测试
                {
                    uart1.Rx_current_step = 0x1B; // 接收数据提醒 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x12) // 指令：全一测试
                {
                    uart1.Rx_current_step = 0x1C; // 接收数据提醒 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x13) // 指令：自定义地址发送函数
                {
                    uart1.Rx_current_step = 0x1D; // 接收数据提醒 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x20) // 指令：读取数据
                {
                    uart1.Rx_current_step = 0x05; // 接收数据提醒 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x21) // 指令：读取数据2
                {
                    uart1.Rx_current_step = 0x0C; // 接收数据提醒 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x22) // 指令：拆包读取数据
                {
                    uart1.Rx_current_step = 0x0E; // 接收拆包数据提醒 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                } 
                else if(uart1.Rx_step_buf[0] == 0x23) // 指令：拆包读取数据（连续）
                {
                    uart1.Rx_current_step = 0x16; // 接收拆包数据提醒 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x24) // 指令：自定义地址接收数据
                {
                    uart1.Rx_current_step = 0x21; // 自定义地址数据提醒 
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x30) // 指令：开启疲劳测试（即连续连续发送读取统计次数）
                {
                    uart1.Rx_current_step = 0x06;   
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x40) // 指令：断电测试
                {
                    uart1.Rx_current_step = 0x0B;
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x50) // 指令：关闭疲劳测试
                {
                    uart1.Rx_current_step = 0x07;
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x60) // 指令：发数据
                {
                    uart1.Rx_current_step = 0x0F;
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x70) // 指令：清空SPI转发数组
                {
                    uart1.Rx_current_step = 0x14;
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                else if(uart1.Rx_step_buf[0] == 0x80) // 指令：修改SPI波特率（Beta）
                {
                    uart1.Rx_current_step = 0x19;
                    uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; 
                    uart1.Rx_Count++;
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                
                break;
            case 0x04: // 4号位，数据接收，直到填满,理论上这里是串口发送的最后一次中断，可以长一点？
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                // uart1.Rx_Count++;
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.RX_Num_16[0])) // 结束了？
                {
                    // 检查校验位！
                    if(uart1.RXBuffer[uart1.Rx_Count - 1] == uart1.RX_Num.RX_Num_16[0] - 5) // 校验位正确继续
                    {
                        uart1.Rx_Count++;
                        uart1.Rx_current_step = 0x00;// 接收置零
                        uart1.contentLength = uart1.Rx_Count; // 存留数据长度
                        arrayCopy(uart1.getArray, uart1.RXBuffer, uart1.Rx_Count);
                        uart1.uart_Receive_String = SUCCESS; // 标志位设定
                    }
                    else // 校验位不正确：进行错误返回操作
                    {
                        uart1.check_flag_on = SUCCESS;
                        uart1.Rx_current_step = 0x00;// 接收置零
                    }
                }
                else
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x04; // 继续接收
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x05: // 5号位，读取数据，设定标志位，目前仅支持一发一读，预留多发多读，待后续设定
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.Rx_Num_8[0]))
                {   
                    uart1.uart_to_check_spi = SUCCESS; // 标志位设定，没结束位不读
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x00; // 归零
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x06: // 6号位，疲劳测试数据接收1
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.num_of_error.num_of_error_16[0] = 0;
                uart1.num_of_test.num_of_test_16[0] = 0;
                uart1.num_of_test_set.num_of_test_set_8[1] = uart1.Rx_step_buf[0]; // 高/低八位
                uart1.Rx_current_step = 0x08;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x07: // 7号位，终止疲劳测试
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.Rx_Num_8[0]))
                {
                    uart1.num_of_error.num_of_error_16[0] = 0;
                    uart1.num_of_test.num_of_test_16[0] = 0;
                    uart1.tired_test = ERROR;
                    spi1.tired_test_Read = ERROR;
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x00; // 归零
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x08: // 8号位，疲劳测试数字接收2
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.num_of_test_set.num_of_test_set_8[0] = uart1.Rx_step_buf[0]; // 另外的：高/低八位
                spi1.tired_test_Read = ERROR;
                // uart1.tired_test = ERROR; // 标志位设定，没结束位不来，没有后续操作               
                uart1.Rx_current_step = 0x0A;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x09: // 9号位，等待结束
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.Rx_Num_8[0]))
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x00; // 归零
                }
                else
                {
                    usartuser_Init(); // 没结束位就全清一遍
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x0A: // 10号位，疲劳测试等待校验位
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                if(uart1.Rx_step_buf[0] == uart1.RX_Num.RX_Num_16[0] - 5) // 计算公式
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x09;
                    uart1.tired_test = SUCCESS; // 标志位设定，没结束位不来，没有后续操作
                    uart1.to_tired_test = SUCCESS;
                }
                else // 校验位不正确：进行错误返回操作
                {
                    uart1.check_flag_on = SUCCESS;
                    uart1.Rx_current_step = 0x00;// 接收置零
                }                                         
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x0B: // 11号位，断电测试？接收断电时长1
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.cut_down_time.cutdown_8[1] = uart1.Rx_step_buf[0];
                uart1.Rx_Count++;
                uart1.Rx_current_step= 0x0D;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x0C: // 12号位，读取数据直接读     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.Rx_Num_8[0]))
                {
                    uart1.receiveforward = SUCCESS; // 标志位设定，没结束位不读
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x00; // 归零
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x0D: // 13号位，断电测试？接收断电时长2
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.cut_down_time.cutdown_8[0] = uart1.Rx_step_buf[0];
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x15; // 去校验
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x0E: // 14号位，拆包式接收数据，此处接收序号1
                uart1.devided_datanum.datanum_8[1] = uart1.Rx_step_buf[0];     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x10;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x0F: // 15号位，拆包式发送数据，此处发送序号1
                uart1.devided_datanum.datanum_8[1] = uart1.Rx_step_buf[0];     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x11;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x10: // 16号位，拆包式接收数据，此处接收序号2
                uart1.devided_datanum.datanum_8[0] = uart1.Rx_step_buf[0];     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x12; // 校验位
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x11: // 17号位，拆包式发送数据，此处发送序号2
                uart1.devided_datanum.datanum_8[0] = uart1.Rx_step_buf[0];     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x13; // 数据接收
                uart1.devided_data_num = 0;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x12: // 18号位，读取接收校验位
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                if(uart1.Rx_step_buf[0] == uart1.RX_Num.RX_Num_16[0] - 5) // 计算公式
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x09;
                    uart1.devided_data_get = SUCCESS;
                }                                             
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x13: // 19号位，拆分数据包接收数据
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.devided_data[uart1.devided_data_num] = uart1.Rx_step_buf[0];
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.RX_Num_16[0])) // 结束了？
                {
                    // 检查校验位！
                    if(uart1.RXBuffer[uart1.Rx_Count - 1] == uart1.RX_Num.RX_Num_16[0] - 5) // 校验位正确继续
                    {
                        uart1.Rx_Count++;
                        uart1.devided_data_num++;
                        uart1.devided_data_num -= 2; // 去除最后两位（校验和结尾）
                        uart1.Rx_current_step = 0x00;// 接收置零
                        uart1.contentLength = uart1.Rx_Count; // 存留数据长度
                        arrayCopy(uart1.getArray, uart1.RXBuffer, uart1.Rx_Count);
                        arrayCopy(spi1.devided_data, uart1.devided_data, uart1.devided_data_num);
                        uart1.devided_receive_data = SUCCESS; // 标志位设定
                    }
                    else // 校验位不正确：进行错误返回操作
                    {
                        uart1.check_flag_on = SUCCESS;
                        uart1.Rx_current_step = 0x00;// 接收置零
                    }
                }
                else
                {
                    uart1.Rx_Count++;
                    uart1.devided_data_num++;
                    uart1.Rx_current_step = 0x13; // 继续接收
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x14: // 20号位，清空数组
                uart1.clc_order = SUCCESS;
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x09;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x15: // 21号位，断电校验位
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                if(uart1.Rx_step_buf[0] == uart1.RX_Num.RX_Num_16[0] - 5) // 计算公式
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x09;
                    uart1.cut_off_test = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x16: // 22号位，连续读取序号1
                uart1.devided_datanum.datanum_8[1] = uart1.Rx_step_buf[0];     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x17;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x17: // 23号位，连续读取序号2
                uart1.devided_datanum.datanum_8[0] = uart1.Rx_step_buf[0];     
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x18; // 校验位
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x18: // 24号位，连续读取校验位
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                if(uart1.Rx_step_buf[0] == uart1.RX_Num.RX_Num_16[0] - 5) // 计算公式
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x09;
                    uart1.all_to_transmit_start = SUCCESS;
                    uart1.get_data_all_in = 0x0000;
                }     
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x19: // 25号位，修改SPI波特率参数接收（8个选择，仅一字节）
                uart1.spi_changer = uart1.Rx_step_buf[0]; // 该参数为预分频器大小，仅限
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x1A; // 校验位
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x1A: // 26号位，修改SPI波特率校验位
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                if(uart1.Rx_step_buf[0] == uart1.RX_Num.RX_Num_16[0] - 5) // 计算公式
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x09;
                    uart1.spi_to_change = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x1B: // 27号位，全零测试，无参数，直接接收结束位
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.Rx_Num_8[0]))
                {
                    uart1.zeros_to_test = SUCCESS; // 标志位设定，没结束位不读
                    uart1.zeros_trans = SUCCESS;
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x00; // 归零
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x1C: // 28号位，全一测试，无参数，直接接收结束位
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.Rx_Num_8[0]))
                {
                    uart1.ones_to_test = SUCCESS; // 标志位设定，没结束位不读
                    uart1.ones_trans = SUCCESS;
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x00; // 归零
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x1D: // 29号位，自定义数据发送：地址高位，限定0x00,0x01
                uart1.get_addr_set_high = uart1.Rx_step_buf[0];
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x1E;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x1E: // 30号位，自定义数据发送：地址中位
                uart1.get_addr_set.addr_8[1] = uart1.Rx_step_buf[0];
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x1F;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x1F: // 31号位，自定义数据发送：地址低位
                uart1.get_addr_set.addr_8[0] = uart1.Rx_step_buf[0];
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x20;
                uart1.devided_data_num = 0;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x20: // 32号位，数据接收，直到填满，理论上这里是串口发送的最后一次中断，可以长一点？
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.devided_data[uart1.devided_data_num] = uart1.Rx_step_buf[0];
                if((uart1.Rx_step_buf[0] == 0xEE) && (uart1.Rx_Count == uart1.RX_Num.RX_Num_16[0])) // 结束了？
                {
                    // 检查校验位！
                    if(uart1.RXBuffer[uart1.Rx_Count - 1] == uart1.RX_Num.RX_Num_16[0] - 5) // 校验位正确继续
                    {
                        uart1.Rx_Count++;
                        uart1.devided_data_num++;
                        uart1.devided_data_num -= 2; // 去除最后两位（校验和结尾）
                        uart1.Rx_current_step = 0x00;// 接收置零
                        uart1.contentLength = uart1.Rx_Count; // 存留数据长度
                        arrayCopy(uart1.getArray, uart1.RXBuffer, uart1.Rx_Count);
                        arrayCopy(spi1.devided_data, uart1.devided_data, uart1.devided_data_num);
                        uart1.trans_with_addr = SUCCESS; // 标志位设定
                    }
                    else // 校验位不正确：进行错误返回操作
                    {
                        uart1.check_flag_on = SUCCESS;
                        uart1.Rx_current_step = 0x00;// 接收置零
                    }
                }
                else
                {
                    uart1.Rx_Count++;
                    uart1.devided_data_num++;
                    uart1.Rx_current_step = 0x13; // 继续接收
                }
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x21: // 33号位，自定义数据接收：地址高位，限定0x00,0x01
                uart1.get_addr_set_high = uart1.Rx_step_buf[0];
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x22;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x22: // 34号位，自定义数据接收：地址中位
                uart1.get_addr_set.addr_8[1] = uart1.Rx_step_buf[0];
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x23;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x23: // 35号位，自定义数据接收：地址低位
                uart1.get_addr_set.addr_8[0] = uart1.Rx_step_buf[0];
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x24;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x24: // 36号位：自定义数据接收：字长
                uart1.devided_data_num = uart1.Rx_step_buf[0]; // 字长存储
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                uart1.Rx_Count++;
                uart1.Rx_current_step = 0x25;
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x25: // 37号位：自定义数据接收的校验位
                uart1.RXBuffer[uart1.Rx_Count] = uart1.Rx_step_buf[0]; // 先保存再说
                if(uart1.Rx_step_buf[0] == uart1.RX_Num.RX_Num_16[0] - 5) // 计算公式
                {
                    uart1.Rx_Count++;
                    uart1.Rx_current_step = 0x09; // 去结束
                    uart1.receive_with_addr = SUCCESS;
                    uart1.get_data_all_in = 0x0000;
                }     
                HAL_UART_Receive_IT(huart, uart1.Rx_step_buf, 1); // 重新设置接收
                break;
        }
    }
    
    // 以下为串口屏输入输出
    if(huart->Instance == USART2) // 串口屏回信
    {
        // 20241223当前进度：已学会如何让串口屏发送接收串口消息并显示文字了，等我的好消息（？）
        if(uart2.uart_time < sys.sys_time) // 检验时间
        {
            uart2.Rx_current_step = 0x00; // 失期当斩
        }
        uart2.uart_time = sys.sys_time + 500; // 须在500ms内完成一次接收，失期法皆斩
        switch(uart2.Rx_current_step) // 根据接收内容判定情况
        {
            case 0x00: // 0号位，初始态，检查接收内容
                if(uart2.Rx_step_buf[0] == 0xA5) // 接收起始位，已收到，不确定，再看看？
                {
                    uart2.Rx_current_step = 0x01; // 下一步
                    uart2.Rx_Count = 0x00; // 计数器复位准备开始计数
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 保存这一位进数组
                    uart2.Rx_Count++; // 保存完请++，确保中断时数组当位为空
                }
                // 在此处预留其他操作的设定！（else if）
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x01: // 1号位，已收到接收数据第一个起始位，继续看有没有第二个
                if(uart2.Rx_step_buf[0] == 0x5A) // 第二个起始符也对？那就开始准备接收吧。
                {
                    uart2.Rx_current_step = 0x02; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                    // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
                }
                else
                {
                    uart2.Rx_current_step = 0x00; // 认错人了，回去重认
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // 重新设置接收
                // HAL_UART_Transmit_DMA(&huart1, &uart1.Rx_current_step, 1);
                break;
            case 0x02: // 2号位，两个起始位都收到了，指令确认，看看这次有几个数据？
                //由于线位限制，数据最多256字节，难评，后续看能否扩展到65536字节？（那就是两个字节是数据量）
                //扩展65536字节是64KB，ARM内存可能不够，必须配合SPI边收边发。预留该功能。
                uart2.Rx_current_step = 0x03; // 下一步
                uart2.RX_Num.Rx_Num_8[0] = uart2.Rx_step_buf[0] + 3; // +3是因为前面有三字节，如扩展改为+4，从0开始所以其实对应的是0xEE那一字节
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0];  
                uart2.Rx_Count++;       
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // 重新设置接收
                // HAL_UART_Transmit_DMA(&huart1, &uart1.Rx_current_step, 1);
                // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
                break;                    
            case 0x03: // 3号位，检验指令，分类讨论
                if(uart2.Rx_step_buf[0] == 0x10) // 指令：发送数据
                {
                    uart2.Rx_current_step = 0x04; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                    uart2.devided_data_num = 0x00;
                }
                else if(uart2.Rx_step_buf[0] == 0x11) // 指令：全零测试数据
                {
                    uart2.Rx_current_step = 0x0D; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x12) // 指令：全一测试数据
                {
                    uart2.Rx_current_step = 0x0E; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x21) // 指令：接收数据
                {
                    uart2.Rx_current_step = 0x05; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x30) // 指令：疲劳测试
                {
                    uart2.Rx_current_step = 0x06; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x40) // 指令：断电测试1
                {
                    uart2.Rx_current_step = 0x08; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x41) // 指令：断电测试：开电
                {
                    uart2.Rx_current_step = 0x0A; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x42) // 指令：断电测试：断电
                {
                    uart2.Rx_current_step = 0x09; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x43) // 指令：断电测试2
                {
                    uart2.Rx_current_step = 0x0B; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x45) // 指令：断电测试全零
                {
                    uart2.Rx_current_step = 0x0F; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x46) // 指令：断电测试全一，新：全断电指令
                {
                    uart2.Rx_current_step = 0x10; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x50) // 指令：疲劳测试结束
                {
                    uart2.Rx_current_step = 0x07; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                else if(uart2.Rx_step_buf[0] == 0x60) // 指令：清除数据
                {
                    uart2.Rx_current_step = 0x0C; // 下一步
                    uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; 
                    uart2.Rx_Count++;
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x04: // 4号位，发送数据
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 先保存再说
                uart2.devided_data[uart2.devided_data_num] = uart2.Rx_step_buf[0];
                // uart1.Rx_Count++;
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.RX_Num_16[0])) // 结束了？
                {
                    // 检查校验位！
                    if(uart2.RXBuffer[uart2.Rx_Count - 1] == uart2.RX_Num.RX_Num_16[0] - 5) // 校验位正确继续
                    {
                        uart2.Rx_Count++;
                        uart2.devided_data_num++;
                        uart2.devided_data_num -= 2;
                        uart2.Rx_current_step = 0x00;// 接收置零
                        uart2.contentLength = uart2.Rx_Count; // 存留数据长度
                        arrayCopy(uart2.getArray, uart2.RXBuffer, uart2.Rx_Count);
                        arrayCopy(spi1.devided_data, uart2.devided_data, uart2.devided_data_num);
                        uart2.uart_Receive_String = SUCCESS; // 标志位设定
                    }
                    else // 校验位不正确：进行错误返回操作
                    {
                        uart2.check_flag_on = SUCCESS;
                        uart2.Rx_current_step = 0x00;// 接收置零
                    }
                }
                else
                {
                    uart2.Rx_Count++;
                    uart2.devided_data_num++;
                    uart2.Rx_current_step = 0x04; // 继续接收
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x05: // 5号位，读取数据
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 先保存再说
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00; // 归零
                    uart2.receiveforward = SUCCESS; // 标志位设定，没结束位不读
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x06: // 6号位，疲劳测试
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 先保存再说
                uart2.Rx_Count++;
                uart2.num_of_error.num_of_error_16[0] = 0;
                uart2.num_of_test.num_of_test_16[0] = 0;
                uart2.num_of_test_set.num_of_test_set_16[0] = 500;
                uart2.Rx_current_step = 0x07;
                spi1.tired_test_Read = ERROR;
                uart2.tired_test = SUCCESS;
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1);
                break;
            case 0x07: // 7号位，关闭疲劳测试
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 先保存再说
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.num_of_error.num_of_error_16[0] = 0;
                    uart2.num_of_test.num_of_test_16[0] = 0;
                    uart2.tired_test = ERROR;
                    spi1.tired_test_Read = ERROR;
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00; // 归零
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x08: // 8号位，断电测试1
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 先保存再说
                uart2.cut_down_time.cutdown_16[0] = 5000; // 默认值5000
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00;
                    uart2.cut_off_test = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1);
                break;
            case 0x09: // 9号位，断电测试关电
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 先保存再说
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00;
                    spi1.electric_turn_off = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1);
                break;
            case 0x0A: // 10号位，断电测试开电
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 先保存再说
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00;
                    spi1.electric_turn_on = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1);
                break;
            case 0x0B: // 11号位，断电测试2
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 先保存再说
                uart2.cut_down_time.cutdown_16[0] = 500; // 默认值500
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00;
                    uart2.cut_off_test = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1);
                break;
            case 0x0C:
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 先保存再说
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00;
                    screen.clear_all = SUCCESS;
                }     
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1);
                break;
            case 0x0D: // 13号位，全零测试，无参数，直接接收结束位
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 先保存再说
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.zeros_to_test = SUCCESS; // 标志位设定，没结束位不读
                    uart2.zeros_trans = SUCCESS;
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00; // 归零
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x0E: // 14号位，全一测试，无参数，直接接收结束位
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 先保存再说
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.ones_to_test = SUCCESS; // 标志位设定，没结束位不读
                    uart2.ones_trans = SUCCESS;
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00; // 归零
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x0F: // 15号位，断电全零测试，无参数，直接接收结束位
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 先保存再说
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.cut_off_to_zeros = SUCCESS; // 标志位设定，没结束位不读
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00; // 归零
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // 重新设置接收
                break;
            case 0x10: // 16号位，断电全一测试，无参数，直接接收结束位
                uart2.RXBuffer[uart2.Rx_Count] = uart2.Rx_step_buf[0]; // 先保存再说
                if((uart2.Rx_step_buf[0] == 0xEE) && (uart2.Rx_Count == uart2.RX_Num.Rx_Num_8[0]))
                {
                    uart2.cut_off_to_ones = SUCCESS; // 标志位设定，没结束位不读
                    uart2.Rx_Count++;
                    uart2.Rx_current_step = 0x00; // 归零
                }
                HAL_UART_Receive_IT(huart, uart2.Rx_step_buf, 1); // 重新设置接收
                break;
        }
    }
}

void usart_finish_ithandle(UART_HandleTypeDef *huart) // 空闲中断
{
     if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE)) // 判定：触发的是否是空闲中断?
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        uart1.Rx_current_step = 0; // 空闲中断，步骤强制复位，防止后续出错
        uint8_t aa = uart1.Rx_Count;
        HAL_UART_Transmit_DMA(&huart1, &aa, 1);
    }
}

// 发送中断回调函数，连续发送时以确定下一步操作，这个说是DMA的所以不用开IT
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
        if(uart2.tired_test_by_screen == SUCCESS) // 两度发送
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
                case 1: // 1：清空曲线
                    // EE B1 33 00 02 00 02 00 FF FC FF FF  (12)
                    uart2.to_trans.trans64[0] = 0x000200020033B1EE;
                    uart2.to_trans.trans32[2] = 0xFFFFFCFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 12);
                    break;
                case 2: // 2：清空3界面所有文本
                    // EE B1 12 00 03 00 06 00 01 20 00 07 00 01 20 00 08 00 01 20 FF FC FF FF  (24)
                    // EE B1 12 00 04 00 06 00 01 20 00 07 00 01 20 00 08 00 01 20 FF FC FF FF 
                    // EE B1 10 00 03 00 06 FF FC FF FF 
                    uart2.to_trans.trans64[0] = 0xFF0600030010B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
                case 3: // 2：清空3界面所有文本
                    uart2.to_trans.trans64[0] = 0xFF0700030010B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
                case 4: // 2：清空3界面所有文本
                    uart2.to_trans.trans64[0] = 0xFF0800030010B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
                case 5: // 3：清空4界面所有文本
                    // EE B1 12 00 04 00 06 00 01 20 00 07 00 01 20 00 08 00 01 20 FF FC FF FF  (24)
                    uart2.to_trans.trans64[0] = 0xFF0700040010B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
                case 6: // 3：清空4界面所有文本
                    // EE B1 12 00 04 00 06 00 01 20 00 07 00 01 20 00 08 00 01 20 FF FC FF FF  (24)
                    uart2.to_trans.trans64[0] = 0xFF0800040010B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
                case 7: // 3：清空4界面所有文本
                    // EE B1 12 00 04 00 06 00 01 20 00 07 00 01 20 00 08 00 01 20 FF FC FF FF  (24)
                    uart2.to_trans.trans64[0] = 0xFF0900040010B1EE;
                    uart2.to_trans.trans16[4] = 0xFFFC;
                    uart2.to_trans.trans8[10] = 0xFF;
                    screen.step++;
                    HAL_UART_Transmit_DMA(&huart2, uart2.to_trans.trans8, 11);
                    break;
                case 8: // 9号表格清空
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
