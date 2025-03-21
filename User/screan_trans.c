#include <stdio.h>
#include <string.h>
#include "uasrtuser.h"
#include "board.h"
#include "spiuser.h"



// 串口屏：报告发送者
void zeros_ones_trans_report(void)
{
     // EE B1 52 00 00 00 01 30 3B D5 FD B3 A3 3B FF FC FF FF   （正确）
                    
     // EE B1 52 00 00 00 01 30 3B D2 EC B3 A3 3B FF FC FF FF   （错误）
    
    ErrorStatus judge = SUCCESS;
    
    if(uart2.zeros_ones_report == SUCCESS)
    {
        switch(uart2.report_step)
        {
            case 0: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 1: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 2: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 3: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 4: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 5: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 6: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 7: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 8: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 9: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 10: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 11: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 12: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 13: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 14: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 15: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 16: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 17: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 18: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 19: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 20: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 21: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 22: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 23: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 24: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 25: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 26: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 27: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 28: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 29: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 30: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
            case 31: // 逐次发送
                uart2.to_trans.trans64[0] = 0x300100020052B1EE;
                uart2.to_trans.trans8[8] = 0x3B;
                for(int i = 0; i < 2048; i++)
                {
                    if(uart2.error_array[i] != 0)
                    {
                        judge = ERROR;
                        break;
                    }
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
                uart2.report_step++;
                break;
        }
    }
}