   
void UART0_CONFIG(void);
void UART0_Tx(unsigned char);
unsigned char UART0_Rx(void);

void UART0_CONFIG(void)
{
PINSEL0|=0X00000005;
U0LCR=0X83;
U0DLL=97;
U0LCR=0X03;
}

void UART0_Tx(unsigned char d)
{
while((U0LSR&(1<<5))==0);
U0THR=d;
}

unsigned char UART0_Rx(void)
{
while((U0LSR&(1<<0))==0);
return U0RBR;
}

void UART0_STR(unsigned char *s)
{
while(*s)
UART0_Tx(*s++);
}
		  

