#include <18f4550.h>
#fuses INTRC_IO, CPUDIV1, NOPROTECT, NOWDT, NOMCLR, NOLVP
#use delay (clock=8M)
#use rs232(xmit = PIN_D0, rcv = PIN_D1, baud =9600, bits = 8, parity = N, stream = TTL)
#include <string.h>
#include <stdlib.h>

#define S_SENSOR PIN_C0
#define S_GLCD PIN_C1
#define S_MOTOR PIN_C2
#define RED_LED PIN_D6
#define GREEN_LED PIN_D7

#define ADC 1
#define PWM 2
#define GLCD 3

//RCSTA: RECEIVE STATUS AND CONTROL REGISTER

#bit CREN=0xFAB.4    // Continuous Receive Enable Bit
#bit OERR=0xFAB.1    // Overrun Error Bit

int distance=0;

void fflush();
void btConnection();
void btCommands();
void writePWM(int duty);
void writeGLCD(int D1, int D2);
int readSensor();

void main()
{
   int mDist;
   
   output_high(S_SENSOR);
   output_high(S_GLCD);
   output_high(S_MOTOR);
   setup_spi(spi_master | spi_l_to_h | spi_clk_div_4);
   
   btConnection();
   btCommands();
   
   while(true)
   {
      mDist=readSensor();
      
      if(mDist<=distance)
      {
         writeGLCD(mDist, distance);
         writePWM(50);
      }
      
      else
      {
         writeGLCD(distance, distance);
         writePWM(0);
         //PWM Duty Cycle 0, restart variables
         btCommands();
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

void btConnection()
{
   int i;
   short notConnected=1;
   char s[50];
   
   output_high(RED_LED);
   
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
   
   output_low(RED_LED);
   output_high(GREEN_LED);
   
   printf("CONNECTED\n\r");
}

void btCommands()
{
   char c;
   
   while(true)
   {
      fflush();
      if(kbhit())
      {
         c=getc();
         switch(c)
         {
            case '1':
            {
               printf("START: ");
               if(distance)
               {
                  printf("OK\n\r"); 
                  return;
               }
               else
               printf("NO DISTANCE SELECTED\n\r");

               break;
            }
            
            case '2':
            {
               printf("STOP: OK\n\r");
               //PWM Duty cycle 0, restart all variables
               distance=0;
               break;
            }
            
            case '3':
            {
               printf("7 CM: OK\n\r");
               distance=7;
               break;
            }
            
            case '4':
            {
               printf("15 CM: OK\n\r");
               distance=15;
               break;
            }
            
            case '5':
            {
               printf("19 CM: OK\n\r");
               distance=19;
               break;
            }       
         }
      }
   }  
}

void writeGLCD(int distance)
{}


