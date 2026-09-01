void UART1_CONFIG(void);
void UART1_Tx(unsigned char);
unsigned char UART1_Rx(void);

void UART1_CONFIG(void)									               
{
PINSEL0|=0X00050000;
U1LCR=0X83;
U1DLL=8;
U1LCR=0X03;
}

void UART1_Tx(unsigned char d)
{
while((U1LSR&(1<<5))==0);
U1THR=d;
}

unsigned char UART1_Rx(void)
{
while((U1LSR&(1<<0))==0);
return U1RBR;
}

void UART1_STR(unsigned char *s)
{
while(*s)
UART1_Tx(*s++);
}
		  

