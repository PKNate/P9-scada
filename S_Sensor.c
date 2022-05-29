#include <18F4550.h>
#fuses INTRC,NOPROTECT,NOWDT,CPUDIV1,NOMCLR
#use delay (clock=8M)
#use rs232(baud=9600,xmit=PIN_D2,rcv=PIN_D3, parity=N, bits=8)

#BYTE SSPBUF = 0xFC9
#BYTE SSPSTAT=0xFC7
#BYTE SSPCON1=0xFC6
#bit BUFFER_FULL=0xFC7.0
#bit OVERFLOW=0xFC6.6
#bit COLLISION=0xFC6.7

#BYTE T1CON=0xFCD
#BYTE TMR1L=0xFCE
#BYTE TMR1H=0xFCF
#bit CREN=0xFAB.4

#DEFINE TRIGGER PIN_B3
#DEFINE ECHO PIN_B2

int data=0,distancia_entera=0;
int16 cuenta;
float tiempo,distancia=0;
int timer_alto=0, timer_bajo=0;

void ultrasonico(){

output_high(TRIGGER);
delay_us(10);
output_low(TRIGGER);
while(!input(ECHO)){}

set_timer1(0);

while(input(ECHO)){}

cuenta=get_timer1();
timer_alto=TMR1H;
timer_bajo=TMR1L;

tiempo=cuenta*(4); //tiempo(us)= (4/Fosc*prescaler)/(1x10-6)

distancia=(tiempo)/((58.30));
distancia_entera=distancia;

}
/*
#int_ssp
void spi_rcv()
{  
   data = SSPBUF;
   
   switch(data)
   {
      case 1:
         OVERFLOW=0;     
         COLLISION=0;
         //ultrasonico();
         SSPBUF = 10;
         //printf("Parte baja: %x\n\r",TMR1L);
         
      break;
      
      case 2:
         OVERFLOW=0;
         COLLISION=0;
         //ultrasonico();
         SSPBUF = 20;
         //printf("Parte alta: %x\n\r\n\r",TMR1H);
         
      break;

   }  
   
}
*/
void main()
{
   
   /*
   //FUNCIONES PARA LA COMUNICACIÓN SPI
   setup_spi(spi_slave | spi_L_to_H);
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
   */
   
   T1CON=0b11110001;    // <7>16bits  <5-4>Prescaler 8 <3>TMR1 Osc disabled <2>Ignored <1>Internal clock source <0>Enable TMR1
   
   
   while(TRUE)
   {
      ultrasonico();
      printf("Distancia: %3.2f\r\n",distancia);
      printf("Parte alta: %x\r\n",TMR1H);
      printf("Parte baja: %x\r\n",TMR1L);
      delay_ms(1000);
   } 
}


