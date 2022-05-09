#include <18f4550.h>
#fuses INTRC_IO, CPUDIV1, NOPROTECT, NOWDT, NOMCLR, NOLVP
#use delay (clock=8M)
#use rs232(xmit = PIN_D0, rcv = PIN_D1, baud =9600, bits = 8, parity = N, stream = TTL)
#include <string.h>
#include <stdlib.h>

#define S_SENSOR PIN_C0
#define S_GLCD PIN_C1
#define S_MOTOR PIN_C2

//RCSTA: RECEIVE STATUS AND CONTROL REGISTER

#bit CREN=0xFAB.4    // Continuous Receive Enable Bit
#bit OERR=0xFAB.1    // Overrun Error Bit

/*
// comandos
#define ADC    1
#define CONT   2
#define PUSH   3
*/

void fflush();

void main()
{
   int i;
   short notConnected=1;
   char c, s[50];
   
   output_high(S_SENSOR);
   output_high(S_GLCD);
   output_high(S_MOTOR);
   output_high(PIN_D6);
   setup_spi(spi_master | spi_l_to_h | spi_clk_div_4);
   
   while(notConnected)
   {
      fflush();
      if(kbhit())
      {
         gets(s);                        
         if(s[1] == '+' && s[2] == 'C' && s[3] == 'O' && s[4] == 'N' && s[5] == 'N'&& s[6] == 'E' && s[7] == 'C' && s[8] == 'T' && s[9] == 'E' && s[10] == 'D')
            notConnected=0;
         else
         for(i=0;i<strlen(s);i++)
            s[i]='\0';
      }
   }
   
   printf("Connected succesfully\n\r");
   
   while(true)
   {
      output_low(PIN_D6);
      output_high(PIN_D7);
      
      fflush();
      if(kbhit())
      {
         c=getc();
         switch(c)
         {
            case '1':
            {
               printf("Command received: %c\n\r",c);
               output_low(S_SENSOR);
               spi_write(1);
               output_high(S_SENSOR);
               break;
            }
            
            case '2':
            {
               printf("Command received: %c\n\r",c);
               output_low(S_GLCD);
               spi_write(2);
               output_high(S_GLCD);
               break;
            }
            
            case '3':
            {
               printf("Command received: %c\n\r",c);
               output_low(S_MOTOR);
               spi_write(3);
               output_high(S_MOTOR);
               break;
            }
         }
      }
   }
}

void fflush()
{
   if(OERR)
   {
      getc();     //Clear buffer
      getc();
      CREN=0;      //Clear CREN bit
      CREN=1;           
   }
}
