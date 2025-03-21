#include "spiuser.h"

// SPI铁电芯片数据传输部分。铁电芯片共有8个引脚：
// CS片选信号（1号位，低电平有效），CLK串行时钟信号（6号位，时序逻辑时钟，ARM有对应输出），（片选应常低或单片机自定义引脚，再议）
// SI串行输入（5号位，输入信号，ARM有对应输出），SO串行输出（2号位，输出信号，ARM有对应输入），
// WP写保护（3号位，低电平禁写入高电平导通），HOLD保持信号（7号位，高电平禁用低电平暂停操作保持状态），（本行两信号或应设为常高）
// VDD高电平供电（8号位，3.3V/5V），VSS低电平供电（4号位，3.3V/5V）
// 注：相传圆孔对应位置是1号位，以此类推

spi_type spi1;

void spiuser_Init(void)
{
    spi1.transmit = ERROR;
    spi1.receive = ERROR;
    spi1.devided_addrNum = 0;
    spi1.devided_addrLength[0] = 0;
    spi1.devided_addr[0] = 0x0000;
    spi1.pres = 8;
    spi1.tired_test_Read = ERROR;
    spi1.tired_test_string.tired_test_string_64[0] = 0xFFFFFFFFFFFFFFFF;
    spi1.tired_test_receive.tired_test_receive_64[0] = 0x0000000000000000;
    SPI_WRSR();
    spi1.next_addr = 0x0000;
    spi1.cut_electric = ERROR;
    spi1.electric_turn_on = ERROR;
    spi1.electric_turn_off = ERROR;
}

void enable_SPI(void) // 使能铁电芯片
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET); // PA4引脚被设定为推挽输出CS片选信号，拉低以使能
}


void disable_SPI(void) // 关闭铁电芯片
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET); // PA4引脚被设定为推挽输出CS片选信号，拉低以使能
}

// SPI写入数据和读取数据：
// SPI口对接的是铁电存储器，目标是从铁电存储器读写数据
// 按照要求SPI读写应该先发送“读/写”命令，然后再是写入的内容
// SPI发送数据：铁电存储器先发读写标志（了解到“写”是0x02），然后输入写入铁电存储器的起始地址
// 在此铁电存储器写入地址作为函数形参，同样作为形参的是缓存区地址（存入数据，存满即发）和长度
void SPI_Write_Flash(uint8_t* buffer, uint16_t length, uint16_t addr)
{
    enable_SPI();

    // 时序逻辑：SI：06 02 AD AD AD DD DD DD ...
    
    // uint16_t i;
    uint8_t cmd[4]; // 一期发射数组
    cmd[0] = 0x06;
    
    HAL_SPI_Transmit(&hspi1, cmd, 1, HAL_MAX_DELAY);
    
    disable_SPI();
    
    union 
    {
        uint16_t add16[1];
        uint8_t add8[2];
    }get_addr;
    get_addr.add16[0] = addr;
    
    cmd[0] = 0x02; // 收发提示号
    cmd[1] = 0x00; // 地址高八位
//    cmd[2] = (addr >> 8) & 0xFF; // 取出地址高八位，发送地址中八位
//    cmd[3] = addr & 0xFF; // 取出地址低八位
    cmd[2] = get_addr.add8[1]; // 1是高八位，0是低八位
    cmd[3] = get_addr.add8[0];
    
    enable_SPI();
    
    uint8_t sRx[512];
    
    HAL_SPI_TransmitReceive(&hspi1, cmd, sRx, 4, HAL_MAX_DELAY);
    
    HAL_SPI_Transmit(&hspi1, buffer, length, HAL_MAX_DELAY);
    // 至此应该一段发送完成

    disable_SPI();
    
    while((ReadSR()&0x01) == 0x01) // 发完关闭写使能
    {
        enable_SPI();
        cmd[0] = 0x04;
        HAL_SPI_TransmitReceive(&hspi1, cmd, sRx, 1, HAL_MAX_DELAY);
        disable_SPI();
    }
}

void SPI_Write_zeros_ones(uint8_t* buffer, uint16_t length, uint16_t addr, uint8_t addr_high) // 全零全一测试用，添加了地址最高位
{
    enable_SPI();

    // 时序逻辑：SI：06 02 AD AD AD DD DD DD ...
    
    // uint16_t i;
    uint8_t cmd[4]; // 一期发射数组
    cmd[0] = 0x06;
    
    HAL_SPI_Transmit(&hspi1, cmd, 1, HAL_MAX_DELAY);
    
    disable_SPI();
    
    union 
    {
        uint16_t add16[1];
        uint8_t add8[2];
    }get_addr;
    get_addr.add16[0] = addr;
    
    cmd[0] = 0x02; // 收发提示号
    cmd[1] = addr_high; // 地址高八位
//    cmd[2] = (addr >> 8) & 0xFF; // 取出地址高八位，发送地址中八位
//    cmd[3] = addr & 0xFF; // 取出地址低八位
    cmd[2] = get_addr.add8[1]; // 1是高八位，0是低八位
    cmd[3] = get_addr.add8[0];
    
    enable_SPI();
    
    uint8_t sRx[512];
    
    HAL_SPI_TransmitReceive(&hspi1, cmd, sRx, 4, HAL_MAX_DELAY);
    
    HAL_SPI_Transmit(&hspi1, buffer, length, HAL_MAX_DELAY);
    // 至此应该一段发送完成

    disable_SPI();
    
    while((ReadSR()&0x01) == 0x01) // 发完关闭写使能
    {
        enable_SPI();
        cmd[0] = 0x04;
        HAL_SPI_TransmitReceive(&hspi1, cmd, sRx, 1, HAL_MAX_DELAY);
        disable_SPI();
    }
}

// 经由SPI从铁电存储器读取数据：
// 同理，先发送读取代号，然后发送读取的首地址
// 由此引出一个问题，我们怎么看到底从哪里开始读
// 大致思路：每次记录发送过去的字节/位数，然后发送的时候发送地址加上去（指不止一次发送的情况）
// 然后记录总字节/位数，读取的时候地址减去字节/位数，就是起始读数
void SPI_Read_Flash(uint8_t* inbuffer, uint16_t length, uint16_t addr)
{
    enable_SPI(); // 硬件控制，片选拉低（？）
    
    // 时序逻辑： SI：03 AD AD AD； SO：DD DD DD ...
    
    union 
    {
        uint16_t add16[1];
        uint8_t add8[2];
    }get_addr;
    get_addr.add16[0] = addr;
    uint8_t sRx[5];

    uint8_t cmd[4]; // 预发送数组
    cmd[0] = 0x03; // 接收代号，待定，看铁电存储器型号
    cmd[1] = 0x00;
//    cmd[2] = (addr >> 8) & 0xFF;
//    cmd[3] = addr & 0xFF;
    cmd[2] = get_addr.add8[1];
    cmd[3] = get_addr.add8[0];
    
    HAL_SPI_TransmitReceive(&hspi1, cmd, sRx, 4, HAL_MAX_DELAY);
    HAL_SPI_TransmitReceive(&hspi1, inbuffer, inbuffer, length, HAL_MAX_DELAY);

    // HAL_SPI_TransmitReceive(&hspi1, cmd, inbuffer, 4, HAL_MAX_DELAY);
    
    disable_SPI(); // 硬件控制，片选拉高结束通信（？）
    
}

void SPI_Read_zeros_ones(uint8_t* inbuffer, uint16_t length, uint16_t addr, uint8_t addr_high)
{
    enable_SPI(); // 硬件控制，片选拉低（？）
    
    // 时序逻辑： SI：03 AD AD AD； SO：DD DD DD ...
    
    union 
    {
        uint16_t add16[1];
        uint8_t add8[2];
    }get_addr;
    get_addr.add16[0] = addr;
    uint8_t sRx[5];

    uint8_t cmd[4]; // 预发送数组
    cmd[0] = 0x03; // 接收代号，待定，看铁电存储器型号
    cmd[1] = addr_high;
//    cmd[2] = (addr >> 8) & 0xFF;
//    cmd[3] = addr & 0xFF;
    cmd[2] = get_addr.add8[1];
    cmd[3] = get_addr.add8[0];
    
    HAL_SPI_TransmitReceive(&hspi1, cmd, sRx, 4, HAL_MAX_DELAY);
    HAL_SPI_TransmitReceive(&hspi1, inbuffer, inbuffer, length, HAL_MAX_DELAY);

    // HAL_SPI_TransmitReceive(&hspi1, cmd, inbuffer, 4, HAL_MAX_DELAY);
    
    disable_SPI(); // 硬件控制，片选拉高结束通信（？）
    
}

// 关键步骤：SPI传输速率修改函数，软硬结合（测试）
void setSPIRate(uint32_t prescaler) // 函数定义：改波特率
{
    // SPI波特率由外部时钟+预分频器构成，不能随心所欲设定，在此由给定的波特率计算预分频器数值
    // uint32_t pclk = HAL_RCC_GetPCLK2Freq(); // 外部时钟频率获取（1？2？）
    // uint32_t prescaler = 0; // 预分频器，需要换算，详见工程教育指导书
    // prescaler = pclk / (2 * baudRate); // 计算预分频器
    // 预分频器只能选取：4，8，16，32，64，128，256
    // 具体波特率可自行计算
    SPI1->CR1 &= ~SPI_CR1_BR; // 注意，SPI->CR1寄存器中有一个BR位（3位），可以控制波特率，在此清除之（位与运算）
    // BR共3位，可以选取8个预分频器值
    SPI1->CR1 |= (prescaler << SPI_CR1_BR_Pos); // 波特率预分频器设定
    SPI1->CR1 |= SPI_CR1_SPE; // 重启SPI
}

void SPI_Rate_changer(uint32_t prescaler) // 改波特率的另一种函数（测试）
{
    hspi1.Init.BaudRatePrescaler = prescaler;
    HAL_SPI_Init(&hspi1);
}

void multi_infor_relay(UART_HandleTypeDef *huart, uint16_t length, uint16_t address) // 上位机串口的转发
{
    // 大规模数据读取，目前只有64KB以下才可以因为长度和地址都是16位
    // 小型的好像也可以，有对应算法
    // 数据发送没限制大小，存满即发，考虑数据读取能不能也来个这样的
    // 只是构想，没有测试和实装
    uint16_t to_get = 1024; // 一次1KB？
    uint16_t rest_length = length;
    uint8_t relay_buffer[1024]; // 转存数组
    uint8_t cmd[3]; // 预发送数组
    while(rest_length > 0) // 一直读取直到读完
    {
        //目前了解到的情况是，发送一次接收命令只能接收一次，不需要停止接受命令（？）
        cmd[0] = 0x03; // 接收代号，待定，看铁电存储器型号
        cmd[1] = (address >> 8) & 0xFF;
        cmd[2] = address & 0xFF;
        if(HAL_SPI_Transmit_DMA(&hspi1, cmd, 3) != HAL_OK) // 发送预处理内容，DMA不一定好使，先用着
        {
            Error_Handler();
        }

        if(rest_length < to_get)
        {
            to_get = rest_length; // 最后一次读取设定
        }

        //去从SPI读取数据
        if(HAL_SPI_Receive_DMA(&hspi1, relay_buffer, to_get) != HAL_OK) // DMA数据读取，在if里也会执行
        {
            Error_Handler(); // 错误处理
        }
        address = address + to_get; // 地址偏移，下一次从新地址开始读取
        rest_length = rest_length - to_get; // 剩余长度重新计算

        //读取完数据当场串口给上位机发回去
        HAL_UART_Transmit_DMA(&huart1, relay_buffer, to_get);
    }
}


void read_ID(void)
{
    enable_SPI();
    uint8_t cmd[4];
    cmd[0] = 0x9F;
    uint8_t read[9];
    HAL_SPI_Transmit(&hspi1, cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, read, 9, HAL_MAX_DELAY);
    // HAL_SPI_TransmitReceive(&hspi1, cmd, read, 4, HAL_MAX_DELAY);
    HAL_UART_Transmit_DMA(&huart1, read, 9);
    disable_SPI();
}


void SPI_FastRead(uint8_t* inbuffer, uint16_t length, uint16_t addr) // 快速读取
{
    enable_SPI();
    
    uint8_t cmd[4]; // 预发送数组
    cmd[0] = 0x0B; // 接收代号，待定，看铁电存储器型号
    cmd[1] = 0x00;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;
    
    HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY); // 发送指令
    HAL_SPI_Receive(&hspi1, spi1.spiReader, length, HAL_MAX_DELAY); // 接收数据
    
    disable_SPI();
}

void SPI_WRSR(void)
{
    uint8_t cmd[2];
    cmd[0] = 0x01;
    cmd[1] = 0x00;
    HAL_SPI_Transmit(&hspi1, cmd, 2, HAL_MAX_DELAY);
}


uint8_t ReadSR()
{
    enable_SPI();
    uint8_t cmd[1];
    cmd[0] = 0x05;
    uint8_t sRx[3];
    HAL_SPI_TransmitReceive(&hspi1, cmd, sRx,2,100);
    disable_SPI();
    return sRx[1];
}