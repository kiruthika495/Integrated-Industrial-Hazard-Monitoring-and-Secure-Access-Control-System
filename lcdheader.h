void LCD_INIT(void);
void LCD_COMMAND(unsigned char);
void LCD_DATA(unsigned char);
void LCD_STR(unsigned char *);
void LCD_INTEGER(int number);
void LCD_FLOAT(float number);

#define LCD_D 0xF<<14 //D4-D7(P0.14-P0.17)
#define RS 1<<12
#define E 1<<13

typedef unsigned int U32;

typedef unsigned char U8;

typedef signed char S8;

typedef float F32;

 
void LCD_INIT(void)
{

IODIR0|=LCD_D|RS|E;
LCD_COMMAND(0X01);
LCD_COMMAND(0X02);
LCD_COMMAND(0X0C);
LCD_COMMAND(0X28);
LCD_COMMAND(0X80);
}


void LCD_COMMAND(unsigned char cmd)
{
IOCLR0=LCD_D;
IOSET0=(cmd&0XF0)<<10;
IOCLR0=RS;
IOSET0=E;
delay_millisecond(2);
IOCLR0=E;

IOCLR0=LCD_D;
IOSET0=(cmd&0X0F)<<14;
IOCLR0=RS;
IOSET0=E;
delay_millisecond(2);
IOCLR0=E;
}


void LCD_DATA(unsigned char databyte)
{
IOCLR0=LCD_D;
IOSET0=(databyte&0XF0)<<10;
IOSET0=RS;
IOSET0=E;
delay_millisecond(2);
IOCLR0=E;

IOCLR0=LCD_D;
IOSET0=(databyte&0X0F)<<14;
IOSET0=RS;
IOSET0=E;
delay_millisecond(2);
IOCLR0=E;
}



void LCD_STR(unsigned char* s)
{
unsigned char count=0;
while(*s)
{
LCD_DATA(*s++);
count++;
if(count==16)
LCD_COMMAND(0XC0);
}
}




void LCD_INTEGER(int number)
{
unsigned char arr[5];
signed char i =0;
if(number==0)
LCD_DATA('0');
else
{
if(number<0)
{
LCD_DATA('-');
number =-number;
}
while (number>0)
{
arr[i++]=number%10;
number=number/10;
}
for(--i;i>=0;i--)
LCD_DATA(arr[i]+48);
}
}



void LCD_FLOAT(float f)
{
unsigned int n;
n=f;
LCD_INTEGER(n);
LCD_DATA('.');
n=(f-n)*100;
LCD_INTEGER(n);
}
