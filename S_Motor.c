#include <18f4550.h>
#fuses INTRC, PLL2, CPUDIV1, NOPROTECT, NOWDT, NOMCLR, NOLVP
#use delay (clock=8M)

#byte PORTA = 0xF80
#byte PORTB = 0xF81
#byte PORTC = 0xF82
#byte PORTD = 0xF83
#byte PORTE = 0xF84
#byte TRISA = 0xF92
#byte TRISB = 0xF93
#byte TRISC = 0xF94
#byte TRISD = 0xF95
#byte TRISE = 0xF96
#bit BI2 = 0xF83.5
#bit BI1 = 0xF83.6
#bit AI2 = 0xF83.3
#bit AI1 = 0xF83.2
#bit STBY = 0xF83.4

#BYTE SSPBUF = 0xFC9
#define FORWARD 3
#define STOP 4

int16 i;
int data;
void motor(char M1, int16 pwm1, char M2, int16 pwm2);

#int_ssp
void spi_rcv()
{  
   data = SSPBUF;
   
   switch(data)
   {
      case FORWARD:
      {
         STBY=1;
         motor('D',(int16)422,'D',(int16)470);
         break;
      }
      
      case STOP:
      {
         motor('R',(int16)422,'R',(int16)470);
         for(i=0;i<200;i++){}
         STBY=0;
         motor('N',(int16)0,'N',(int16)0);
         break;
      }
   }  
}

void main()
{
   STBY=0;
   
   TRISA=0b00100000; //A5: SLAVE SELECTOR
   TRISB=0b11111111; //B0: SDA, B1: SCL (SLAVE), B4:B7 ENCODERS
   TRISC=0b00000000; 
   TRISD=0b00000000;
   TRISE=0b00000000;
   
   setup_timer_2(T2_DIV_BY_1,254,1);
   setup_ccp1(CCP_PWM);
   setup_ccp2(CCP_PWM); 
   
   setup_spi(spi_slave | spi_L_to_H);
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
   
   delay_ms(1000);
   
   while(true){}
}

void motor(char M1, int16 pwm1, char M2, int16 pwm2)
{
   switch (M1)
   {
      case 'D': {BI1=1; BI2=0; break;}
      case 'R': {BI1=0; BI2=1; break;}
      case 'N': {BI1=0; BI2=0; break;}
   }
   
   switch (M2)
   {
      case 'D': {AI1=1; AI2=0; break;}
      case 'R': {AI1=0; AI2=1; break;}
      case 'N': {AI1=0; AI2=0; break;}
   }
   
   set_pwm1_duty(pwm2);
   set_pwm2_duty(pwm1);

   return;
}
