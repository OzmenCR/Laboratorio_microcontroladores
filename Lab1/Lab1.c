#include <pic14/pic12f675.h>
 
typedef unsigned int word ;
word __at 0x2007 __CONFIG = (_MCLRE_OFF & _WDT_OFF & _INTRC_OSC_NOCLKOUT) ;
 
void delay (unsigned inttiempo);
 
void main(void)
{
	ANSEL=0b00000000;
    TRISIO = 0b00100000; //Poner todos los pines como salidas
	GPIO = 0x00000000; //Poner pines en bajo
 
    unsigned int time = 250;
	int timer = 0;
	int cont = 0;
    //Loop forever
    while ( 1 )
    {
			timer = timer+1;
			if (timer == 6){
				timer =0;
			}
			if(GPIO==0b00100000){
				cont = timer;
				if(cont==1){
					GPIO=0b00000001;
					delay(time);
					GPIO=0b00000000;
				}
				if(cont == 2){
					GPIO=0b00000010;
					delay(time);
					GPIO=0b00000000;
				}
				if(cont == 3){
					GPIO=0b00000100;
					delay(time);
					GPIO=0b00000000;
				}
				if(cont == 4){
					GPIO=0b00000101;
					delay(time);
					GPIO=0b00000000;
				}
				if(cont == 5){
					GPIO=0b00000110;
					delay(time);
					GPIO=0b00000000;
				}
				if(cont == 6){
					GPIO=0b00000111;
					delay(time);
					GPIO=0b00000000;
				}
			}
    }
 
}

void delay(unsigned int tiempo)
{
	unsigned int i;
	unsigned int j;

	for(i=0;i<tiempo;i++)
	  for(j=0;j<1275;j++);
}