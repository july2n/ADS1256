#ifndef __ADS1256_H_
#define __ADS1256_H_

#include <main.h>
#include <stm32f4xx.h>


/*端口定义*/
#define SCLK_SET (AD1256_SCLK_GPIO_Port->BSRR = AD1256_SCLK_Pin)
#define SCLK_RESET (AD1256_SCLK_GPIO_Port->BSRR = (uint32_t)AD1256_SCLK_Pin << 16)
#define DIN_SET (AD1256_DIN_GPIO_Port->BSRR = AD1256_DIN_Pin)
#define DIN_RESET (AD1256_DIN_GPIO_Port->BSRR = (uint32_t)AD1256_DIN_Pin << 16)
#define DOUT ((AD1256_DOUT_GPIO_Port->IDR & AD1256_DOUT_Pin)?1:0)
#define DRDY ((AD1256_DRDY_GPIO_Port->IDR & AD1256_DRDY_Pin)?1:0)
#define CS_SET (AD1256_CS_GPIO_Port->BSRR = AD1256_CS_Pin)
#define CS_RESET (AD1256_CS_GPIO_Port->BSRR = (uint32_t)AD1256_CS_Pin << 16)
#define RESET_SET (AD1256_RESET_GPIO_Port->BSRR = AD1256_RESET_Pin)
#define RESET_RESET (AD1256_RESET_GPIO_Port->BSRR = (uint32_t)AD1256_RESET_Pin << 16)

//指令
#define ADS1256_CMD_RDATA    0x01
#define ADS1256_CMD_RDATAC   0x03
#define ADS1256_CMD_SDATAC   0x0f
#define ADS1256_CMD_RREG     0x10
#define ADS1256_CMD_WREG     0x50
#define ADS1256_CMD_SELFCAL  0xf0
#define ADS1256_CMD_SELFOCAL 0xf1
#define ADS1256_CMD_SELFGCAL 0xf2
#define ADS1256_CMD_SYSOCAL  0xf3
#define ADS1256_CMD_SYSGCAL  0xf4
#define ADS1256_CMD_SYNC     0xfc
#define ADS1256_CMD_STANDBY  0xfd
#define ADS1256_CMD_RESET    0xfe
#define ADS1256_CMD_WAKEUP   0xff

//寄存器地址
#define ADS1256_STATUS       0x00
#define ADS1256_MUX          0x01
#define ADS1256_ADCON        0x02
#define ADS1256_DRATE        0x03
#define ADS1256_IO           0x04
#define ADS1256_OFC0         0x05
#define ADS1256_OFC1         0x06
#define ADS1256_OFC2         0x07
#define ADS1256_FSC0         0x08
#define ADS1256_FSC1         0x09
#define ADS1256_FSC2         0x0A
//输入引脚
#define ADS1256_MUXP_AIN0   0x00
#define ADS1256_MUXP_AIN1   0x10
#define ADS1256_MUXP_AIN2   0x20
#define ADS1256_MUXP_AIN3   0x30
#define ADS1256_MUXP_AIN4   0x40
#define ADS1256_MUXP_AIN5   0x50
#define ADS1256_MUXP_AIN6   0x60
#define ADS1256_MUXP_AIN7   0x70
#define ADS1256_MUXP_AINCOM 0x80

#define ADS1256_MUXN_AIN0   0x00
#define ADS1256_MUXN_AIN1   0x01
#define ADS1256_MUXN_AIN2   0x02
#define ADS1256_MUXN_AIN3   0x03
#define ADS1256_MUXN_AIN4   0x04
#define ADS1256_MUXN_AIN5   0x05
#define ADS1256_MUXN_AIN6   0x06
#define ADS1256_MUXN_AIN7   0x07
#define ADS1256_MUXN_AINCOM 0x08
//内置增益
#define ADS1256_GAIN_1      0x00
#define ADS1256_GAIN_2      0x01
#define ADS1256_GAIN_4      0x02
#define ADS1256_GAIN_8      0x03
#define ADS1256_GAIN_16     0x04
#define ADS1256_GAIN_32     0x05
#define ADS1256_GAIN_64     0x06
//采样率
#define ADS1256_DRATE_30000SPS   0xF0
#define ADS1256_DRATE_15000SPS   0xE0
#define ADS1256_DRATE_7500SPS    0xD0
#define ADS1256_DRATE_3750SPS    0xC0
#define ADS1256_DRATE_2000SPS    0xB0
#define ADS1256_DRATE_1000SPS    0xA1
#define ADS1256_DRATE_500SPS     0x92
#define ADS1256_DRATE_100SPS     0x82
#define ADS1256_DRATE_60SPS      0x72
#define ADS1256_DRATE_50SPS      0x63
#define ADS1256_DRATE_30SPS      0x53
#define ADS1256_DRATE_25SPS      0x43
#define ADS1256_DRATE_15SPS      0x33
#define ADS1256_DRATE_10SPS      0x23
#define ADS1256_DRATE_5SPS       0x13
#define ADS1256_DRATE_2_5SPS   	 0x03

int ADS1256_Init(void);
void ADS1256_WriteByte(uint8_t date);
uint8_t ADS1256_ReadByte(void);
void ADS1256_SendCommand(uint8_t Command);
void ADS1256_WriteToRegister(uint8_t Address, uint8_t Data);
uint32_t ADS1256_GetAdc(void);
uint8_t ADS1256_ReadChipID(void);
#endif
