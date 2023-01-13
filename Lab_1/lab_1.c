#include <pic14/pic12f675.h>

typedef unsigned int word;

#define BTN GP5

// Desabilitar el watchdog timer y poner GP3 como I/O
word __at 0x2007 __CONFIG = (_MCLRE_OFF & _WDT_OFF);
 
void delay (unsigned int tiempo);

void blink_dice (int rand);

char BTN_pressed = 0;
char BTN_press = 0;
char BTN_release = 0;
char Bouncevalue = 500;

unsigned int time = 100;
int rand = 6;
 
void main(void)
{

    TRISIO 	=	0b00100000; //Pin [3] entrada, los demas son output
	GPIO 	&=	(0b11100000); //Poner pines de salida en bajo
	ANSEL	=	0b00100000; //Pin [3] como entrada analógica
 
    //Loop forever
    while ( 1 )
    {	
		// If BTN is pressed 
        if (BTN == 1)
        {
            // Bouncing has started so increment BTN_press with 1, for each "high" bounce
            BTN_press++;
            // "reset" BTN_release
            BTN_release = 0;
            // If it bounces so much that BTN_press is greater than Bouncevalue
            // then button must be pressed
            if (BTN_press > Bouncevalue)
            {
                // This is initial value of BTN_pressed. 
                // If program gets here, button must be pressed
                if (BTN_pressed == 0)
                {
                    // Toggle the LEDs
                    //LED1 ^= 1;
                    //LED2 ^= 1;
					blink_dice(rand);
					rand--;
                    // Setting BTN_pressed to 1, ensuring that we will 
                    // not enter this code block again
                    BTN_pressed = 1;
                }
                // LEDs toggled, set BTN_pressed to 0, so we can enter 
                // toggle code block again
                BTN_press = 0;
            }
        }
        else
        {
            // Increment the "low" in the bouncing
            BTN_release++;
            BTN_press = 0;
            // If BTN_release is greater than Bouncevalue, we do not have a
            // pressed button
            if (BTN_release > Bouncevalue)
            {
				GPIO &= (0b11100000);
                BTN_pressed = 0;
                BTN_release = 0;
            }
        }
	}
}

void blink_dice(int rand)
{
	switch (rand)
	{
		case 1:
			GPIO |= (0b00000000);
			break;
		
		case 2:
			GPIO |= (0b00000001);
			break;
		
		case 3:
			GPIO |= (0b00000011);
			break;
		
		case 4:
			GPIO |= (0b00000111);
			break;
		
		case 5:
			GPIO |= (0b00001111);
			break;
		
		case 6:
			GPIO |= (0b00011111);
			break;
		
		default:
			GPIO |= (0b00010000);
			break;
	}
}

void delay(unsigned int tiempo)
{
	unsigned int i;
	unsigned int j;

	for(i=0;i<tiempo;i++)
	  for(j=0;j<1275;j++);
}
