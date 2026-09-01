#include <LPC21xx.h>
#include <string.h>
#include <stdio.h>

#include "types.h"
#include "delay_header.h"
#include "lcdheader.h"
#include "uartheader.h"
#include "keypad.h"
#include "spi.h"
#include "i2c_defines.h"
#include "i2c.h"
#include "i2c_eeprom.h"
#include "UART1.h"

#include "i2c.c"
#include "i2c_eeprom.c"



#define FLAME_PIN   16                

#define IR_PIN      17      



//void RTC_Set_Initial(void);
void RTC_Display(void);

void Monitor_Hazards(void);
void GSM_Send_Alert(unsigned char *);

char key_to_char(unsigned int key);
void get_password(char input[]);
void read_rfid(char *uid);
void RFID_Access_Control(void);



int main(void)
{
   
    IODIR1 &= ~((1 << FLAME_PIN) | (1 << IR_PIN));
   
   
UART0_CONFIG();
UART1_CONFIG();
SPI_INIT();
init_i2c();
LCD_INIT();
   


    LCD_COMMAND(0x80);
    LCD_STR("IndustrialHazard");
    LCD_COMMAND(0xC0);
    LCD_STR("     Monitoring & ");
    LCD_COMMAND(0x94);
    LCD_STR("Secure Access       ");
    LCD_COMMAND(0xD4);
    LCD_STR("Control System      ");

    delay_millisecond(3000);
    LCD_COMMAND(0x01);
                                             

   while(1)
    {
RFID_Access_Control();
RTC_Display();
        Monitor_Hazards();        
                   
    }




}



void RTC_Display(void)
{
    u8 hr, min, sec, date, month, year;

i2c_eeprom_write(0X68, 0x02, 0x08);  
    i2c_eeprom_write(0X68, 0x01, 0x30);  
    i2c_eeprom_write(0X68, 0x00, 0x40);  
    i2c_eeprom_write(0X68, 0x04, 0x26);  
    i2c_eeprom_write(0X68, 0x05, 0x08);  
    i2c_eeprom_write(0X68, 0x06, 0x26);

    LCD_COMMAND(0xc0);
LCD_STR((unsigned char*)"Time:");

 hr=i2c_eeprom_read(0X68,0X02);

LCD_DATA((hr/16)+48);

LCD_DATA((hr%16)+48);

LCD_DATA(':');

 min=i2c_eeprom_read(0X68,0X01);

LCD_DATA((min/16)+48);

LCD_DATA((min%16)+48);

LCD_DATA(':');

  sec=i2c_eeprom_read(0X68,0X00);

LCD_DATA((sec/16)+48);

LCD_DATA((sec%16)+48);


LCD_COMMAND(0xd4);
LCD_STR((unsigned char*)" ");

date=i2c_eeprom_read(0X68,0X04);

LCD_DATA((date/16)+48);

LCD_DATA((date%16)+48);

LCD_DATA(':');

month=i2c_eeprom_read(0X68,0X05);

LCD_DATA((month/16)+48);

LCD_DATA((month%16)+48);

LCD_DATA(':');

 year=i2c_eeprom_read(0X68,0X06);

LCD_DATA((year/16)+48);

LCD_DATA((year%16)+48);

delay_millisecond(2000);

LCD_COMMAND(0X01);

}



void Monitor_Hazards(void)
{ float f[4];
    float temperature;
float gas;
float ldr;

     unsigned char value[200];

     f[0] = READ_ADC_MCP3204(0);   // LM35
        f[1] = READ_ADC_MCP3204(1);   // Potentiometer
        f[3] = READ_ADC_MCP3204(3);   // LDR

        /* Convert LM35 voltage to temperature */

        temperature = f[0] * 100;
gas = (f[1] / 3.3) * 100;
ldr = (f[3] / 3.3) * 100;

        /* Temperature */

        LCD_COMMAND(0x80);
        LCD_STR((unsigned char*)"Temp:");
        LCD_FLOAT(temperature);
        LCD_STR((unsigned char*)" C");

        LCD_COMMAND(0xC0);
LCD_STR((unsigned char*)"Gas:");
LCD_FLOAT(gas);
LCD_STR((unsigned char*)"%");

LCD_COMMAND(0x94);
LCD_STR((unsigned char*)"LDR:");
LCD_FLOAT(ldr);
LCD_STR((unsigned char*)"%");

       

        if(temperature >= 50)
        {
            sprintf((char*)value,"Temperature High: %d C", (int)temperature);

            GSM_Send_Alert(value);
delay_millisecond(500);
        }

        if(gas >= 75)
{
   sprintf((char*)value,"Gas Level High: %d %%", (int)gas);
   GSM_Send_Alert(value);
   delay_millisecond(500);
}

if(ldr >= 75)
{
   sprintf((char*)value,"Light Level High: %d %%", (int)ldr);
   GSM_Send_Alert(value);
   delay_millisecond(500);
}

        delay_millisecond(3000);

LCD_COMMAND(0X01);
   

if((IOPIN1 & (1 << FLAME_PIN)) == 0)
        {
            LCD_COMMAND(0x80);
            LCD_STR("FLAME :DETECTED");

            sprintf((char*)value, "ALERT: Flame :DETECTED");
            GSM_Send_Alert(value);

            delay_millisecond(3000);
        }
        else
        {
            LCD_COMMAND(0x80);
            LCD_STR("FLAME: SAFE  ");
        }


        /* ---------------- IR SENSOR ---------------- */

        if((IOPIN1 & (1 << IR_PIN)) == 0)
        {
            LCD_COMMAND(0xC0);
            LCD_STR("IR OBJECT FOUND");

            sprintf((char*)value, "ALERT: Object detected");
            GSM_Send_Alert(value);

            delay_millisecond(3000);
        }
        else
        {
            LCD_COMMAND(0xC0);
            LCD_STR("IR: NO OBJECT   ");
        }

        delay_millisecond(500);
LCD_COMMAND(0X01);
}



void GSM_Send_Alert(unsigned char *value)
{
    UART1_STR("AT\r\n");
delay_millisecond(500);
UART1_STR("AT+CMGF=1\r\n");
delay_millisecond(500);
UART1_STR("AT+CMGS=\"+917013818528\"\r\n");
delay_millisecond(500);
UART1_STR(value);
delay_millisecond(500);
UART1_STR("\r\n");
delay_millisecond(500);
UART1_Tx(0x1A);
delay_millisecond(500);
}

void read_rfid(char *uid)
{
    int i;
    for(i = 0; i < 12; i++)
{
        uid[i] = UART0_Rx();
}
    uid[12] = '\0';
}

char key_to_char(unsigned int key)
{
    switch(key)
    {
        case 1:  return '1';
        case 2:  return '2';
        case 3:  return '3';
        case 5:  return '4';
        case 6:  return '5';
        case 7:  return '6';
        case 9:  return '7';
        case 10: return '8';
        case 11: return '9';
        case 14: return '0';
        default: return '\0';
    }
}

void get_password(char input[])
{
    int i = 0;
    unsigned int key;
    char ch;

    LCD_COMMAND(0x01);
    LCD_STR("Enter Password");
    LCD_COMMAND(0xC0);

    while(i < 4)
    {
        key = key_scan();        
        ch = key_to_char(key);

        if(ch != '\0')
        {
            input[i] = ch;
            LCD_DATA('*');      
            i++;
        }
    }

    input[4] = '\0';
}



void RFID_Access_Control(void)
{
    char valid_uid[] = "060067DBFC46";
    char password[]  = "1231";
    char uid[13];
    char input[5];

   // LCD_COMMAND(0x01);
        LCD_STR("Scan RFID");

        read_rfid(uid);
 UART0_STR(uid);           // Read RFID card UID


        if((strstr(uid, valid_uid)) !=NULL)
        {
            get_password(input);     // Read password from keypad

            LCD_COMMAND(0x01);

            if((strstr(input, password)) !=NULL)
            {
                LCD_STR("Access Verified");
delay_millisecond(200);

            }
            else
            {
                LCD_STR("Access Denied");
delay_millisecond(200);
            }
        }
        else
        {
            LCD_COMMAND(0x01);
            LCD_STR("Access Denied");
delay_millisecond(200);
        }

        delay_millisecond(200);  
        LCD_COMMAND(0x01);
}

