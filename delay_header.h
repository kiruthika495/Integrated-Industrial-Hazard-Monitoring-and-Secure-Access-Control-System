#ifndef DELAY_HEADER_H
#define DELAY_HEADER_H

void delay_millisecond(unsigned int ms);
void delay_second(unsigned int second);

void delay_second(unsigned int second)
{
T0PR=15000000-1;
T0TCR=0X01;
while(T0TC<second);
T0TCR=0X03;
T0TCR=0X00;
}


void delay_millisecond(unsigned int ms)
{
T0PR=15000-1;
T0TCR=0X01;
  while(T0TC<ms);
T0TCR=0X03;
T0TCR=0X00;
}

#endif
