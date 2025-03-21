#include "spiuser.h"

// SPI����оƬ���ݴ��䲿�֡�����оƬ����8�����ţ�
// CSƬѡ�źţ�1��λ���͵�ƽ��Ч����CLK����ʱ���źţ�6��λ��ʱ���߼�ʱ�ӣ�ARM�ж�Ӧ���������ƬѡӦ���ͻ�Ƭ���Զ������ţ����飩
// SI�������루5��λ�������źţ�ARM�ж�Ӧ�������SO���������2��λ������źţ�ARM�ж�Ӧ���룩��
// WPд������3��λ���͵�ƽ��д��ߵ�ƽ��ͨ����HOLD�����źţ�7��λ���ߵ�ƽ���õ͵�ƽ��ͣ��������״̬�������������źŻ�Ӧ��Ϊ���ߣ�
// VDD�ߵ�ƽ���磨8��λ��3.3V/5V����VSS�͵�ƽ���磨4��λ��3.3V/5V��
// ע���ഫԲ�׶�Ӧλ����1��λ���Դ�����

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

void enable_SPI(void) // ʹ������оƬ
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET); // PA4���ű��趨Ϊ�������CSƬѡ�źţ�������ʹ��
}


void disable_SPI(void) // �ر�����оƬ
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET); // PA4���ű��趨Ϊ�������CSƬѡ�źţ�������ʹ��
}

// SPIд�����ݺͶ�ȡ���ݣ�
// SPI�ڶԽӵ�������洢����Ŀ���Ǵ�����洢����д����
// ����Ҫ��SPI��дӦ���ȷ��͡���/д�����Ȼ������д�������
// SPI�������ݣ�����洢���ȷ���д��־���˽⵽��д����0x02����Ȼ������д������洢������ʼ��ַ
// �ڴ�����洢��д���ַ��Ϊ�����βΣ�ͬ����Ϊ�βε��ǻ�������ַ���������ݣ������������ͳ���
void SPI_Write_Flash(uint8_t* buffer, uint16_t length, uint16_t addr)
{
    enable_SPI();

    // ʱ���߼���SI��06 02 AD AD AD DD DD DD ...
    
    // uint16_t i;
    uint8_t cmd[4]; // һ�ڷ�������
    cmd[0] = 0x06;
    
    HAL_SPI_Transmit(&hspi1, cmd, 1, HAL_MAX_DELAY);
    
    disable_SPI();
    
    union 
    {
        uint16_t add16[1];
        uint8_t add8[2];
    }get_addr;
    get_addr.add16[0] = addr;
    
    cmd[0] = 0x02; // �շ���ʾ��
    cmd[1] = 0x00; // ��ַ�߰�λ
//    cmd[2] = (addr >> 8) & 0xFF; // ȡ����ַ�߰�λ�����͵�ַ�а�λ
//    cmd[3] = addr & 0xFF; // ȡ����ַ�Ͱ�λ
    cmd[2] = get_addr.add8[1]; // 1�Ǹ߰�λ��0�ǵͰ�λ
    cmd[3] = get_addr.add8[0];
    
    enable_SPI();
    
    uint8_t sRx[512];
    
    HAL_SPI_TransmitReceive(&hspi1, cmd, sRx, 4, HAL_MAX_DELAY);
    
    HAL_SPI_Transmit(&hspi1, buffer, length, HAL_MAX_DELAY);
    // ����Ӧ��һ�η������

    disable_SPI();
    
    while((ReadSR()&0x01) == 0x01) // ����ر�дʹ��
    {
        enable_SPI();
        cmd[0] = 0x04;
        HAL_SPI_TransmitReceive(&hspi1, cmd, sRx, 1, HAL_MAX_DELAY);
        disable_SPI();
    }
}

void SPI_Write_zeros_ones(uint8_t* buffer, uint16_t length, uint16_t addr, uint8_t addr_high) // ȫ��ȫһ�����ã�����˵�ַ���λ
{
    enable_SPI();

    // ʱ���߼���SI��06 02 AD AD AD DD DD DD ...
    
    // uint16_t i;
    uint8_t cmd[4]; // һ�ڷ�������
    cmd[0] = 0x06;
    
    HAL_SPI_Transmit(&hspi1, cmd, 1, HAL_MAX_DELAY);
    
    disable_SPI();
    
    union 
    {
        uint16_t add16[1];
        uint8_t add8[2];
    }get_addr;
    get_addr.add16[0] = addr;
    
    cmd[0] = 0x02; // �շ���ʾ��
    cmd[1] = addr_high; // ��ַ�߰�λ
//    cmd[2] = (addr >> 8) & 0xFF; // ȡ����ַ�߰�λ�����͵�ַ�а�λ
//    cmd[3] = addr & 0xFF; // ȡ����ַ�Ͱ�λ
    cmd[2] = get_addr.add8[1]; // 1�Ǹ߰�λ��0�ǵͰ�λ
    cmd[3] = get_addr.add8[0];
    
    enable_SPI();
    
    uint8_t sRx[512];
    
    HAL_SPI_TransmitReceive(&hspi1, cmd, sRx, 4, HAL_MAX_DELAY);
    
    HAL_SPI_Transmit(&hspi1, buffer, length, HAL_MAX_DELAY);
    // ����Ӧ��һ�η������

    disable_SPI();
    
    while((ReadSR()&0x01) == 0x01) // ����ر�дʹ��
    {
        enable_SPI();
        cmd[0] = 0x04;
        HAL_SPI_TransmitReceive(&hspi1, cmd, sRx, 1, HAL_MAX_DELAY);
        disable_SPI();
    }
}

// ����SPI������洢����ȡ���ݣ�
// ͬ���ȷ��Ͷ�ȡ���ţ�Ȼ���Ͷ�ȡ���׵�ַ
// �ɴ�����һ�����⣬������ô�����״����￪ʼ��
// ����˼·��ÿ�μ�¼���͹�ȥ���ֽ�/λ����Ȼ���͵�ʱ���͵�ַ����ȥ��ָ��ֹһ�η��͵������
// Ȼ���¼���ֽ�/λ������ȡ��ʱ���ַ��ȥ�ֽ�/λ����������ʼ����
void SPI_Read_Flash(uint8_t* inbuffer, uint16_t length, uint16_t addr)
{
    enable_SPI(); // Ӳ�����ƣ�Ƭѡ���ͣ�����
    
    // ʱ���߼��� SI��03 AD AD AD�� SO��DD DD DD ...
    
    union 
    {
        uint16_t add16[1];
        uint8_t add8[2];
    }get_addr;
    get_addr.add16[0] = addr;
    uint8_t sRx[5];

    uint8_t cmd[4]; // Ԥ��������
    cmd[0] = 0x03; // ���մ��ţ�������������洢���ͺ�
    cmd[1] = 0x00;
//    cmd[2] = (addr >> 8) & 0xFF;
//    cmd[3] = addr & 0xFF;
    cmd[2] = get_addr.add8[1];
    cmd[3] = get_addr.add8[0];
    
    HAL_SPI_TransmitReceive(&hspi1, cmd, sRx, 4, HAL_MAX_DELAY);
    HAL_SPI_TransmitReceive(&hspi1, inbuffer, inbuffer, length, HAL_MAX_DELAY);

    // HAL_SPI_TransmitReceive(&hspi1, cmd, inbuffer, 4, HAL_MAX_DELAY);
    
    disable_SPI(); // Ӳ�����ƣ�Ƭѡ���߽���ͨ�ţ�����
    
}

void SPI_Read_zeros_ones(uint8_t* inbuffer, uint16_t length, uint16_t addr, uint8_t addr_high)
{
    enable_SPI(); // Ӳ�����ƣ�Ƭѡ���ͣ�����
    
    // ʱ���߼��� SI��03 AD AD AD�� SO��DD DD DD ...
    
    union 
    {
        uint16_t add16[1];
        uint8_t add8[2];
    }get_addr;
    get_addr.add16[0] = addr;
    uint8_t sRx[5];

    uint8_t cmd[4]; // Ԥ��������
    cmd[0] = 0x03; // ���մ��ţ�������������洢���ͺ�
    cmd[1] = addr_high;
//    cmd[2] = (addr >> 8) & 0xFF;
//    cmd[3] = addr & 0xFF;
    cmd[2] = get_addr.add8[1];
    cmd[3] = get_addr.add8[0];
    
    HAL_SPI_TransmitReceive(&hspi1, cmd, sRx, 4, HAL_MAX_DELAY);
    HAL_SPI_TransmitReceive(&hspi1, inbuffer, inbuffer, length, HAL_MAX_DELAY);

    // HAL_SPI_TransmitReceive(&hspi1, cmd, inbuffer, 4, HAL_MAX_DELAY);
    
    disable_SPI(); // Ӳ�����ƣ�Ƭѡ���߽���ͨ�ţ�����
    
}

// �ؼ����裺SPI���������޸ĺ�������Ӳ��ϣ����ԣ�
void setSPIRate(uint32_t prescaler) // �������壺�Ĳ�����
{
    // SPI���������ⲿʱ��+Ԥ��Ƶ�����ɣ��������������趨���ڴ��ɸ����Ĳ����ʼ���Ԥ��Ƶ����ֵ
    // uint32_t pclk = HAL_RCC_GetPCLK2Freq(); // �ⲿʱ��Ƶ�ʻ�ȡ��1��2����
    // uint32_t prescaler = 0; // Ԥ��Ƶ������Ҫ���㣬������̽���ָ����
    // prescaler = pclk / (2 * baudRate); // ����Ԥ��Ƶ��
    // Ԥ��Ƶ��ֻ��ѡȡ��4��8��16��32��64��128��256
    // ���岨���ʿ����м���
    SPI1->CR1 &= ~SPI_CR1_BR; // ע�⣬SPI->CR1�Ĵ�������һ��BRλ��3λ�������Կ��Ʋ����ʣ��ڴ����֮��λ�����㣩
    // BR��3λ������ѡȡ8��Ԥ��Ƶ��ֵ
    SPI1->CR1 |= (prescaler << SPI_CR1_BR_Pos); // ������Ԥ��Ƶ���趨
    SPI1->CR1 |= SPI_CR1_SPE; // ����SPI
}

void SPI_Rate_changer(uint32_t prescaler) // �Ĳ����ʵ���һ�ֺ��������ԣ�
{
    hspi1.Init.BaudRatePrescaler = prescaler;
    HAL_SPI_Init(&hspi1);
}

void multi_infor_relay(UART_HandleTypeDef *huart, uint16_t length, uint16_t address) // ��λ�����ڵ�ת��
{
    // ���ģ���ݶ�ȡ��Ŀǰֻ��64KB���²ſ�����Ϊ���Ⱥ͵�ַ����16λ
    // С�͵ĺ���Ҳ���ԣ��ж�Ӧ�㷨
    // ���ݷ���û���ƴ�С�������������������ݶ�ȡ�ܲ���Ҳ����������
    // ֻ�ǹ��룬û�в��Ժ�ʵװ
    uint16_t to_get = 1024; // һ��1KB��
    uint16_t rest_length = length;
    uint8_t relay_buffer[1024]; // ת������
    uint8_t cmd[3]; // Ԥ��������
    while(rest_length > 0) // һֱ��ȡֱ������
    {
        //Ŀǰ�˽⵽������ǣ�����һ�ν�������ֻ�ܽ���һ�Σ�����Ҫֹͣ�����������
        cmd[0] = 0x03; // ���մ��ţ�������������洢���ͺ�
        cmd[1] = (address >> 8) & 0xFF;
        cmd[2] = address & 0xFF;
        if(HAL_SPI_Transmit_DMA(&hspi1, cmd, 3) != HAL_OK) // ����Ԥ�������ݣ�DMA��һ����ʹ��������
        {
            Error_Handler();
        }

        if(rest_length < to_get)
        {
            to_get = rest_length; // ���һ�ζ�ȡ�趨
        }

        //ȥ��SPI��ȡ����
        if(HAL_SPI_Receive_DMA(&hspi1, relay_buffer, to_get) != HAL_OK) // DMA���ݶ�ȡ����if��Ҳ��ִ��
        {
            Error_Handler(); // ������
        }
        address = address + to_get; // ��ַƫ�ƣ���һ�δ��µ�ַ��ʼ��ȡ
        rest_length = rest_length - to_get; // ʣ�೤�����¼���

        //��ȡ�����ݵ������ڸ���λ������ȥ
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


void SPI_FastRead(uint8_t* inbuffer, uint16_t length, uint16_t addr) // ���ٶ�ȡ
{
    enable_SPI();
    
    uint8_t cmd[4]; // Ԥ��������
    cmd[0] = 0x0B; // ���մ��ţ�������������洢���ͺ�
    cmd[1] = 0x00;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;
    
    HAL_SPI_Transmit(&hspi1, cmd, 4, HAL_MAX_DELAY); // ����ָ��
    HAL_SPI_Receive(&hspi1, spi1.spiReader, length, HAL_MAX_DELAY); // ��������
    
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