#include <18f4550.h>
#fuses INTRC_IO, CPUDIV1, NOPROTECT, NOWDT, NOMCLR, NOLVP
#use delay (clock=8M)

#BYTE SSPBUF = 0xFC9

/*
// comandos
#define ADC    1
#define CONT   2
#define PUSH   3
*/
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

