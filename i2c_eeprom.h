                         /* i2c_defines.h */

#ifndef __I2C_DEFINES_H__

#define __I2C_DEFINES_H__


//defines for pin function selection

#define SCL_EN  0x00000010

#define SDA_EN  0x00000040


//defines for I2C_SPEED Configuration 

#define CCLK      60000000  //Hz

#define PCLK      CCLK/4    //Hz

#define I2C_SPEED 100000    //Hz

#define LOADVAL   ((PCLK/I2C_SPEED)/2)

#endif
//bit defines for I2CONSET sfr

                          /* i2c_eeprom.h */

#ifndef __I2C_EEPROM_H__

#define __I2C_EEPROM_H__


void i2c_eeprom_write(u8,u8,u8);

u8   i2c_eeprom_read(u8,u8);

void i2c_eeprom_page_write(u8 slaveAddr,u8 wBuffStartAddr,u8 *p,u8 nBytes);

void i2c_eeprom_seq_read(u8 slaveAddr,u8 rBuffStartAddr,u8 *p,u8 nBytes);


#endif
