#include <pic14/pic12f675.h>

typedef unsigned int word;

// Desabilitar el watchdog timer y poner GP3 como I/O
word __at 0x2007 __CONFIG = (_MCLRE_OFF & _WDT_OFF);
 
void delay (unsigned int tiempo);

void blink_dice (unsigned int rand, unsigned int *pressed);
 
void main(void)
{

    TRISIO 	=	0b00100000; //Pin [5] entrada, pines [4:0] salidas
	GPIO 	&=	(0b11100000); //Poner pines de salida en bajo
	ANSEL	=	0b00100000; //Pin [5] como entrada analógica

 
    unsigned int time = 100;
	unsigned int pressed = 0;
	unsigned int rand = 6;
 
    //Loop forever
    while ( 1 )
    {		/*
			GP0 = 0x00;
			delay(time);

			GP0 = ~GP0;
			delay(time);
			*/
		if (GP5)
		{
			if (!pressed)
			{
				blink_dice(rand, &pressed);
				//rand--;
				pressed = 1;
			}
		}
		else
		{
			GPIO &= (0b11100000);
			pressed = 0;
		}
    }
 
}

void blink_dice(unsigned int rand, unsigned int *pressed)
{
	switch (rand)
	{
		case 1:
			GPIO |= (0b00000000);
		
		case 2:
			GPIO |= (0b00000001);
		
		case 3:
			GPIO |= (0b00000011);
		
		case 4:
			GPIO |= (0b00000111);
		
		case 5:
			GPIO |= (0b00001111);
		
		case 6:
			GPIO |= (0b00011111);
		
		default:
			GPIO |= (0b00000101);
	}

	*pressed = 1;
}

void delay(unsigned int tiempo)
{
	unsigned int i;
	unsigned int j;

	for(i=0;i<tiempo;i++)
	  for(j=0;j<1275;j++);
}
