#include <18f4550.h>
#fuses HSPLL, PLL2, CPUDIV1, NOPROTECT, NOWDT, NOMCLR, NOLVP
#use delay (clock=48M)
#use rs232(xmit = PIN_D0, rcv = PIN_D1, baud =9600, bits = 8, parity = N)
#include <string.h>
#include <stdlib.h>

#define S_SENSOR PIN_C0
#define S_GLCD PIN_C1
#define S_MOTOR PIN_C2
#define RED_LED PIN_D6
#define GREEN_LED PIN_D7

#define SETUP 1
#define LOOP 2
#define END 3

//RCSTA: RECEIVE STATUS AND CONTROL REGISTER

#bit CREN=0xFAB.4    // Continuous Receive Enable Bit
#bit OERR=0xFAB.1    // Overrun Error Bit

void fflush();
void btConnection();
int btCommands();
void writeGLCD(int command, int mDistance);
void writePWM(int command, int mDistance);
int readSensor();

void main()
{
   signed int mDistance=0, Distance=0;
   
   //SPI Config
   output_high(S_SENSOR);
   output_high(S_GLCD);
   output_high(S_MOTOR);
   setup_spi(spi_master | spi_l_to_h | spi_clk_div_4);
   
   btConnection();
   
   start:
   Distance=btCommands();
   /*
   writeGLCD(SETUP,Distance);
   writePWM(SETUP,Distance);
   */
   while(true)
   {
      mDistance=readSensor();
      printf("Distance read: %i\n\r",mDistance);
      /*
      if(mDistance>Distance)
      {
         writeGLCD(LOOP,mDistance);
         writePWM(LOOP,mDistance);
      }
      
      else
      {
         writeGLCD(END,Distance);
         writePWM(END,Distance);
         Distance=0;
         mDistance=0;
         goto start;
      }
      */
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

int btCommands()
{
   int distance=0;
   char c;
   
   printf("ENTER COMMAND: \n\r");
   
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
                  return distance;
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

void writeGLCD(int command, int mDistance)
{
   output_low(S_GLCD);
   spi_write(command);
   //Could it work without a delay?
   spi_write(mDistance);
   output_high(S_GLCD);
}

void writePWM(int command, int mDistance)
{
   output_low(S_MOTOR);
   spi_write(command);
   //Could it work without a delay?
   spi_write(mDistance);
   output_high(S_GLCD);
}

int readSensor()
{
   int temp=0;
   
   output_low(S_GLCD);
   spi_write(LOOP);
   //Could it work without a delay?
   //temp=spi_read();
   output_high(S_GLCD);
   
   return temp;
}
