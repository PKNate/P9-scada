#include <18f4550.h>
#fuses HSPLL, PLL2, CPUDIV1, NOPROTECT, NOWDT, NOMCLR, NOLVP
#use delay (clock=48M)

#BYTE SSPBUF = 0xFC9

#define SETUP 1
#define LOOP 2
#define END 3

int data;

#int_ssp
void spi_rcv()
{  
   data = SSPBUF;
   
   switch(data)
   {
      case 2:
         output_toggle(PIN_D0);
      break;
   }  
}

void main()
{
   setup_spi(spi_slave | spi_L_to_H);
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
   
   while(true)
   {
   }
   
}

