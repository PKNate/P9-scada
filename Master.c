#include <18f4550.h>
#fuses INTRC, PLL2, CPUDIV1, NOPROTECT, NOWDT, NOMCLR, NOLVP
#use delay (clock=8M)
#use rs232(xmit = PIN_D0, rcv = PIN_D1, baud =9600, bits = 8, parity = N)
#include <string.h>
#include <stdlib.h>

#define S_SENSOR PIN_C0
#define S_GLCD PIN_C1
#define S_MOTOR PIN_C2

//SPI Registers
#BYTE SSPBUF = 0xFC9
#BYTE SSPSTAT =0xFC7
#BYTE SSPCON1 =0xFC6
#bit BUFFER_FULL=0xFC7.0
#bit OVERFLOW=0xFC6.6
#bit COLLISION=0xFC6.7
#bit CREN=0xFAB.4    // Continuous Receive Enable Bit
#bit OERR=0xFAB.1    // Overrun Error Bit

#define CLOCK 0
#define READLSB 1
#define READMSB 2
#define FORWARD 3
#define STOP 4
#define SETUP 5
#define UPDATE 6

int timer_alto=0;
int timer_bajo=0;
int16 timer_total=0;
float distancia=0;
int8 distancia_int;
int16 distance;

void fflush();
void btConnection();
void btCommands();
void writePWM(int command);
void writeGLCD(int command, int mDistance);
void readSensor();

void main()
{
   output_high(S_SENSOR);
   output_high(S_GLCD);
   output_high(S_MOTOR);
   setup_spi(spi_master | spi_l_to_h | spi_clk_div_4);
   delay_ms(5000);
   //btConnection();
   
   //start:
   //btCommands();
   Distance=17;
   writeGLCD(SETUP,Distance);

   while(true)
   {
      readSensor();
      writeGLCD(UPDATE,distancia_int);
      
      /*
      if(>Distance)
      {
         writeGLCD(CLOCK,mDistance);
         writePWM(FORWARD);
      }
      
      else
      {
         writeGLCD(CLOCK,Distance);
         writePWM(STOP);
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
}
void btCommands()
{
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
                  return;
               }
               else
               printf("NO DISTANCE SELECTED\n\r");

               break;
            }
            
            case '2':
            {
               printf("STOP: OK\n\r");
               
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
   switch(command)
   {
      case SETUP:
      {
         output_low(S_GLCD); 
         spi_write(SETUP);
         output_high(S_GLCD); 
         delay_us(100);
         output_low(S_GLCD); 
         spi_write(mDistance); 
         delay_us(100); 
         output_high(S_GLCD); 
         break;
      }
      
      case UPDATE:
      {
         output_low(S_GLCD); 
         spi_write(UPDATE);
         output_high(S_GLCD); 
         delay_us(100);
         output_low(S_GLCD); 
         spi_write(mDistance); 
         delay_us(100); 
         output_high(S_GLCD); 
         break;
      }
      
      case STOP:
      {
         output_low(S_GLCD); 
         spi_write(STOP);
         output_high(S_GLCD); 
         break;
      }
   }
}
void writePWM(int command)
{

}
void readSensor()
{
   output_low(S_SENSOR); 
   spi_write(1); // Envia el comando  
   output_high(S_SENSOR); 
   delay_ms(30); 
   output_low(S_SENSOR); 
   timer_bajo = spi_read(0); 
   delay_ms(30); 
   output_high(S_SENSOR); 
   
   output_low(S_SENSOR); 
   spi_write(2); // Envia el comando  
   output_high(S_SENSOR); 
   delay_ms(30); 
   output_low(S_SENSOR); 
   timer_alto = spi_read(0); 
   delay_ms(30); 
   output_high(S_SENSOR); 
       
   timer_total=timer_alto<<8;
   timer_total+=timer_bajo;
   distancia=(timer_total*4);
   distancia=distancia/58.3;
   
   if(distancia>99)
   distancia=99;
   distancia_int=(int)distancia;
   
   //printf("LSB: %x  ",timer_bajo);
   //printf("MSB: %x  ",timer_alto);
   //printf("D: %3.2f\r\n",distancia);
}


