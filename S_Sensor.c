#include <18f4550.h>
#fuses HSPLL, PLL2, CPUDIV1, NOPROTECT, NOWDT, NOMCLR, NOLVP
#use delay (clock=48M)

#BYTE SSPBUF = 0xFC9

#define TRIGGER PIN_B3
#define SETUP 1
#define LOOP 2
#define END 3

int8 distance=0;
int32 data=0;
short noTrigger = 1;

#INT_EXT2
void measure()
{
   set_timer0(0);
   while(input(PIN_B2));
   
   data=get_timer0();
   data=(data/174.9271137                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      );
   
   if(data>255)
   data=255;
   
   distance=data;
   noTrigger = 1;
}

#int_ssp
void spi_rcv()
{  
   SSPBUF=distance;
}


void main()
{
   setup_spi(spi_slave | spi_L_to_H);
   enable_interrupts(INT_SSP|INT_EXT2_L2H);
   enable_interrupts(GLOBAL);
   setup_timer_0(T0_INTERNAL|T0_DIV_4);

   while(true)
   {
      if(noTrigger)
      {
         output_high(TRIGGER);
         delay_us(10);
         output_low(TRIGGER);
         noTrigger=0;
      }
   }
}

