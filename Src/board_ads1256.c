#include "board_ads1256.h"
#include "usart.h"
#include "stdlib.h"

/********************************************************************************************************
*	函 数 名: ADS1256_WaitDRDY
*	功能说明: 等待内部操作完成。 自校准时间较长，需要等待。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ADS1256_WaitDRDY(void)
{
	uint32_t i;

	for (i = 0; i < 100; i++)
	{
		if (DRDY == 0)
		{
			break;
		}
	}
	if (i >= 100)
	{
		printf("ADS1256_WaitDRDY() Time Out ...\r\n");		/* 调试语句. 用语排错 */
	}
}


/*******************************************
函数名称：ADS1256_Init
功    能：初始化SPI总线
参    数：无
返回值  ：无
********************************************/
int ADS1256_Init(void) {
    CS_SET;
    delay_us(10);
    SCLK_RESET;
    delay_us(10);
    DIN_RESET;
    delay_us(10);
    RESET_RESET;
    delay_us(10);
    RESET_SET;
    delay_us(10);
    CS_RESET;
    delay_us(1);
		ADS1256_WaitDRDY();
		ADS1256_SendCommand(ADS1256_CMD_SELFCAL);//自动校验
	
    ADS1256_WriteToRegister(ADS1256_STATUS, 0x06);//开启自动校验
		ADS1256_WriteToRegister(ADS1256_ADCON, 0x00);//增益为1
    ADS1256_WriteToRegister(ADS1256_DRATE, ADS1256_DRATE_30SPS);
		ADS1256_WriteToRegister(ADS1256_IO,0x00);
	
		ADS1256_WriteToRegister(ADS1256_MUX,ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM);
    ADS1256_SendCommand(ADS1256_CMD_SYNC);    //同步校准
		delay_us(5);
    ADS1256_WriteByte(ADS1256_CMD_WAKEUP);  //唤醒
    delay_us(25);
		CS_SET;
    SCLK_RESET;
    DIN_RESET;
    return 0;
}

/*******************************************
函数名称：ADS1256_WriteByte
功    能：向SPI总线写一个字节数据
参    数：date-一个字节数据
返回值  ：无
********************************************/
void ADS1256_WriteByte(uint8_t date) {
    uint8_t i;
		
    for (i = 0; i < 8; i++) 
		{
				if (date & 0x80) 
					DIN_SET;
				else
					DIN_RESET;
				SCLK_SET;
				delay_us(1);
				date = date << 1;
				SCLK_RESET;
				delay_us(1);
		}
}

/*******************************************
函数名称：ADS1256_ReadByte
功    能：从SPI总线读一个字节数据
参    数：无
返回值  ：所读的一个字节
********************************************/
uint8_t ADS1256_ReadByte() {
    uint8_t i, dat = 0;
		SCLK_RESET;
    for (i = 0; i < 8; i++) {
        SCLK_SET;
        delay_us(2);
				dat <<= 1;
				SCLK_RESET;
        if (DOUT) 
					dat++;
				delay_us(2);
    }
    return dat;
}

/*******************************************
函数名称：ADS1256_SendCommand
功    能：向ADS1256发送一条命令
参    数：Command——要发送的命令
返回值  ：无
********************************************/
void ADS1256_SendCommand(uint8_t Command) {
    delay_us(1);
    ADS1256_WaitDRDY();  // DRDY为低的时候才可以写数据
    ADS1256_WriteByte(Command);
}

/*******************************************
函数名称：ADS1256_WriteToRegister
功    能：写指定的寄存器的值
参    数：Address----寄存器地址
                                        Data	 ----要写入的数据
返回值  ：无
********************************************/
void ADS1256_WriteToRegister(uint8_t Address, uint8_t Data) {
    CS_RESET;
    ADS1256_WaitDRDY();       // DRDY为低的时候才可以写数据
    ADS1256_WriteByte(Address | 0x50);  //发送寄存器地址
    ADS1256_WaitDRDY();
    ADS1256_WriteByte(0x00);  //只写一个寄存器
    ADS1256_WaitDRDY();
    ADS1256_WriteByte(Data);  //发送数据
    CS_SET;
}

uint8_t ADS1256_ReadReg(uint8_t _RegID)
{
	uint8_t read;

	CS_RESET;	/* SPI片选 = 0 */
	ADS1256_WriteByte(ADS1256_CMD_RREG | _RegID);	/* 写寄存器的命令, 并发送寄存器地址 */
	ADS1256_WriteByte(0x00);	/* 寄存器个数 - 1, 此处读1个寄存器 */

	delay_us(10);	/* 必须延迟才能读取芯片返回数据 */

	read = ADS1256_ReadByte();	/* 读寄存器值 */
	CS_SET;	/* SPI片选 = 1 */

	return read;
}

/*
*********************************************************************************************************
*	函 数 名: ADS1256_ReadData
*	功能说明: 读ADC数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static int32_t ADS1256_ReadData(void)
{
	uint32_t read = 0;
	
	ADS1256_SendCommand(ADS1256_CMD_RDATA);	/* 读数据的命令 */

	delay_us(20);	/* 必须延迟才能读取芯片返回数据 */

	/* 读采样结果，3个字节，高字节在前 */
	read = ADS1256_ReadByte() << 16;
	read += (ADS1256_ReadByte() << 8);
	read += ADS1256_ReadByte();

	/* 负数进行扩展。24位有符号数扩展为32位有符号数 */
	if (read & 0x800000)
	{
		read += 0xFF000000;
	}
	return (int32_t)read;
}

/*******************************************
函数名称：ADS1256_GetData
功    能：获取指定通道的数据
返回值  ：uint32_t----所获得的数据
********************************************/
uint32_t ADS1256_GetAdc(void) 
{
    uint32_t Data = 0;
    CS_RESET;
		Data = ADS1256_ReadData();
		delay_us(20);
    CS_SET;
    return Data;
}


uint8_t ADS1256_ReadChipID(void)
{
	uint8_t id;

	ADS1256_WaitDRDY();
	id = ADS1256_ReadReg(ADS1256_STATUS);
	return (id >> 4);
}
