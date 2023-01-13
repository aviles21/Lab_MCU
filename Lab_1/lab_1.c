#include <pic14/pic12f675.h>

#define BTN GP3
#define POLY_MASK_32 0XB4BCD35C
#define POLY_MASK_31 0X7A5BC2E3

typedef unsigned int word;

// Desabilitar el watchdog timer y poner GP3 como I/O
word __at 0x2007 __CONFIG = (_MCLRE_OFF & _WDT_OFF);
word lfsr32, lfsr31;

void blink_dice (int rand);

int shift_lfsr(word *lfsr, word polynomial_mask);

void init_lfsrs(void);

int get_random(void);

char BTN_pressed = 0;
char BTN_press = 0;
char BTN_release = 0;
char Bouncevalue = 500;

int rand;
 
void main(void)
{

    TRISIO 	=	0b00001000; //Pin [3] entrada, los demas son output
	GPIO 	&=	(0b11001000); //Poner pines de salida en bajo
	ANSEL	=	0b00001000; //Pin [3] como entrada analógica

	init_lfsrs();
 
    //Loop forever
    while ( 1 )
    {	
		// If BTN is pressed 
        if (BTN == 1)
		//while (BTN == 1)
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
                    do {
						rand = get_random();
					} while ((0 > rand ) || (rand > 6));

					blink_dice(rand);

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
				GPIO &= (0b11001000);
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
			GPIO |= (0b00010111);
			break;
		
		case 6:
			GPIO |= (0b00110111);
			break;
		
		default:
			GPIO |= (0b00000000);
			break;
	}
}

int shift_lfsr(word *lfsr, word polynomial_mask)
{
	int feedback;

	feedback = *lfsr & 1;
	*lfsr >>= 1;
	if (feedback == 1)
		*lfsr ^= polynomial_mask;
	return *lfsr;
}

void init_lfsrs(void)
{
	lfsr32 = 0xABCDE;	/* seed values */
	lfsr31 = 0x23456789;
}

int get_random(void)
{
	/* This random number generator shifts the 32-bit LFSR twice before XORing
	it with the 31-bit LFSR. The bottom 3 bits are used for the random number */

	shift_lfsr(&lfsr32, POLY_MASK_32);
	return (shift_lfsr(&lfsr32, POLY_MASK_32) ^ shift_lfsr(&lfsr31, POLY_MASK_31)) & 0x7;
}
