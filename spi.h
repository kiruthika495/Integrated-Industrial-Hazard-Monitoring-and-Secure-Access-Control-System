#define FOSC 12000000

#define CCLK (5*FOSC)

#define PCLK (CCLK/4)

#define SPCCR_VAL 60

#define SPI_RATE (u8)(PCLK/SPCCR_VAL)

#define SETBIT(WORD,BITPOS)         (WORD|=1<<BITPOS)

#define CLRBIT(WORD,BITPOS)         (WORD&=~(1<<BITPOS))

#define CPLBIT(WORD,BITPOS)         (WORD^=(1<<BITPOS))

#define WRITEBIT(WORD,BITPOS,BIT)   (BIT ? SETBIT(WORD,BITPOS): CLRBIT(WORD,BITPOS))

#define READBIT(WORD,BITPOS)        ((WORD>>BITPOS)&1)

// SPCR Bits Setting
#define CS 7
#define Mode_0     0x00  // CPOL 0 CPHA 0

#define Mode_1     0x08  // CPOL 0 CPHA 1

#define Mode_2     0x10  // CPOL 1 CPHA 0

#define Mode_3     0x18  // CPOL 1 CPHA 1

#define MSTR_BIT   5     // SPI0 as Master

#define LSBF_BIT   6     // default MSB first,if set LSB first

#define SPIE_BIT   7     //SPI Interrupt Enable Bit


// SPSR bits

#define SPIF_BIT   7    // Data Transfer Completion Flag


//SPINT bit

#define SPIINTF_BIT 0   //SPI Interrupt Flag Bit


void SPI_INIT(void)
{
PINSEL0 |=0X00001500;//P0.4 to P0.6 as SPI pins

  S0SPCCR = SPI_RATE;             // cfg bit clock rate

  S0SPCR  = (1<<MSTR_BIT|Mode_3); //spi module in master mode,

                                  //CPOL =1,CCPHA = 1. MSB first

SETBIT(IOPIN0,CS);

SETBIT(IODIR0,CS);
}

unsigned char SPI0(unsigned char data)
{


    S0SPSR=0<<7;
S0SPDR = data;//data register
while(((S0SPSR>>7)&1)==0);
return S0SPDR;//read data from SPI data reg, place into buffer

}

float READ_ADC_MCP3204(unsigned char channel_no)
{

  u8 hByte,lByte;

  u32 adcVal=0;

   

  //select/activate chip

  CLRBIT(IOPIN0,CS);

//delay_ms(100);



  SPI0(0x06);

  hByte = SPI0(channel_no<<6);

  lByte = SPI0(0x00);



//de-select/de-activate chp

SETBIT(IOPIN0,CS);

//delay_ms(100);

  adcVal=((hByte&0x0f)<<8)|lByte;

  return ((adcVal*3.3)/4096);

}
