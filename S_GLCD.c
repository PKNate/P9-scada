#include <18f4550.h>
#fuses INTRC, NOPROTECT, NOWDT, NOLVP, NOMCLR, PLL2, CPUDIV1
#use delay (clock = 8M)
#include <glcd_edit.c>
#BYTE TRISB = 0xF93
#BYTE INTCON  = 0xFF2
#BYTE INTCON2 = 0xFF1
#BYTE RCON    = 0xFD0
#BYTE T0CON = 0xFD5
#BIT TMR0ON = 0xFD5.7

#BYTE SSPBUF = 0xFC9

#define STOP 4
#define SETUP 5
#define UPDATE 6

short flagStop=1;
short flagSetup=0;
short flagUpdate=0;
int prevDistance=0;
int desiredDistance=2;
int Distance=0;
short erase=0;
int data;

#int_ssp
void spi_rcv()
{  
   data = SSPBUF;
   
   if(flagUpdate)
   {
      Distance=data;
      flagUpdate=0;
   }
   
   if(flagSetup)
   {
      desiredDistance=data;
      flagSetup=0;
      flagStop=0;
   } 
   
   switch(data)
   {
      case STOP:
      {
         flagStop=1;
         break;
      }
      
      case SETUP:
      {
         flagSetup=1;
         break;
      }
      
      case UPDATE:
      {
         flagUpdate=1;
         break;
      }
   }
}
short spriteCar[14][10]={
0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
1, 1, 0, 1, 1, 1, 1, 0, 1, 1,
1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
1, 1, 0, 1, 0, 0, 1, 0, 1, 1,
0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
0, 0, 1, 0, 1, 1, 0, 1, 0, 0,
0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
0, 0, 1, 0, 1, 1, 0, 1, 0, 0,
0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
0, 0, 1, 1, 1, 1, 1, 1, 0, 0};
short number[10][5][4]={
0, 1, 1, 0,
1, 0, 0, 1,
1, 0, 0, 1,
1, 0, 0, 1,
0, 1, 1, 0,

1, 1, 1, 0,
0, 1, 0, 0,
0, 1, 0, 0,
1, 1, 0, 0,
0, 1, 0, 0,

1, 1, 1, 1,
1, 0, 0, 0,
0, 1, 1, 0,
0, 0, 0, 1,
1, 1, 1, 0,

1, 1, 1, 0,
0, 0, 0, 1,
0, 1, 1, 0,
0, 0, 0, 1,
1, 1, 1, 0,

0, 0, 0, 1,
0, 0, 0, 1,
1, 1, 1, 1,
1, 0, 0, 1,
1, 0, 0, 1,

1, 1, 1, 0,
0, 0, 0, 1,
1, 1, 1, 0,
1, 0, 0, 0,
1, 1, 1, 1,

0, 1, 1, 0,
1, 0, 0, 1,
1, 1, 1, 0,
1, 0, 0, 0,
0, 1, 1, 0,

0, 1, 0, 0,
0, 1, 0, 0,
0, 0, 1, 0,
0, 0, 0, 1,
1, 1, 1, 1,

0, 1, 1, 0,
1, 0, 0, 1,
0, 1, 1, 0,
1, 0, 0, 1,
0, 1, 1, 0,

0, 1, 1, 0,
0, 0, 0, 1,
0, 1, 1, 1,
1, 0, 0, 1,
0, 1, 1, 0,
};
short flag [8][9]={
1, 0, 0, 0, 0, 0, 0, 0, 0,
1, 0, 0, 0, 0, 0, 0, 0, 0,
1, 0, 0, 0, 0, 0, 0, 0, 0,
1, 0, 1, 1, 0, 0, 0, 0, 0,
1, 0, 1, 1, 1, 1, 0, 0, 0,
1, 0, 1, 1, 1, 1, 1, 1, 1,
1, 0, 1, 1, 1, 1, 0, 0, 0,
1, 0, 1, 1, 0, 0, 0, 0, 0,
};
short arrow[5][5]={
1, 0, 0, 0, 0
0, 1, 1, 0, 0,
0, 0, 0, 1, 1,
0, 1, 1, 0, 0,
1, 0, 0, 0, 0};
short bluetooth[17][12]={
0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0,
1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1,
1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1,
1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1,
1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 
1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1,
1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1,
0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0,
0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0};

void drawCar();
void drawBT();
void drawDistance(int x, int y, int distance);
void drawGoal(int distance);
void drawObstacle(int distance);

void main()
{   
   setup_spi(spi_slave | spi_L_to_H);
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
   glcd_init(on);
   delay_ms(1000);
   glcd_fillScreen(0);
   drawBT();
   
   while(true)
   {      
      while(flagStop){}
    
      glcd_fillScreen(0);
      drawCar();
      drawGoal(desiredDistance);
     
      while(!flagStop)
      {
         drawObstacle(Distance);
         drawDistance(2,53,Distance);
      }
      glcd_fillScreen(0);
      drawCar();
      drawGoal(desiredDistance);
      drawDistance(2,53,desiredDistance);
   }
}

void drawCar()
{
   int i, j;
   
   for (i = 0; i<14; i++)
      for (j = 0; j<10; j++)
         glcd_pixel(i+17,j+26,spriteCar[i][j]);
}
void drawBT()
{
   int i, j;
   
   for (i = 0; i<17; i++)
      for (j = 0; j<12; j++)
         glcd_pixel(i+55,j+26,bluetooth[i][j]);
}
void drawObstacle(int distance)
{
   glcd_line(31+prevDistance, 0, 31+prevDistance, 63, 0);
   glcd_line(32+prevDistance, 0, 32+prevDistance, 63, 0);
   
   if((32+distance)<=127)
   {
      glcd_line(31+distance, 0, 31+distance, 63, 1);
      glcd_line(32+distance, 0, 32+distance, 63, 1);      
      prevDistance=distance;
   }
}
void drawDistance(int x, int y, int distance)
{
   int i, j;
   int ones, tens;
   tens=distance/10;
   ones=distance-(tens*10);
   
   for (i = 0; i<5; i++)
      for (j = 0; j<4; j++)
         glcd_pixel(i+x,j+y,number[tens][i][j]);
         
   for (i = 0; i<5; i++)
      for (j = 0; j<4; j++)
         glcd_pixel(i+x,j+y+5,number[ones][i][j]);
         
   if(distance>95)
   {
      for (i = 0; i<5; i++)
         for (j = 0; j<5; j++)
            glcd_pixel(i+12,j+55,arrow[i][j]);
      erase=1;
   }
   
   else if(erase==1)
   {
      erase=0;
      glcd_rect(12, 55, 12+5, 55+5, 1, 0);
   }
   
}
void drawGoal(int distance)
{
   int i, j=0, k=1;
   
   for(i=0;i<64;i++)
   {
      glcd_pixel(31+distance,i,j);
      glcd_pixel(32+distance,i,k);
      
      if(j==0)
      j=1;
      else
      j=0;
      
      if(k==0)
      k=1;
      else
      k=0;
   }
   
   glcd_rect(0, 0, 9, 12, 0, 1);
   glcd_rect(0, 51, 9, 63, 0, 1);
   
   for (i = 0; i<8; i++)
      for (j = 0; j<9; j++)
         glcd_pixel(1+i, 15+j, flag[i][j]);
   
   glcd_circle(5, 40, 1, 0, 1);
   glcd_circle(5, 45, 1, 0, 1);
   glcd_rect(2, 37, 8, 48, 0, 1);
   glcd_pixel(2, 37, 0);
   glcd_pixel(2, 48, 0);
   glcd_pixel(8, 48, 0);
   glcd_pixel(8, 37, 0);
   drawDistance(2,2,distance);
}

