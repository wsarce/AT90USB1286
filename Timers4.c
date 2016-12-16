#include<avr/io.h>

//===[PROTOTYPES]====
void timer0( void );
void timer0_initialize( void );
void timer0_wait_until_OVF( void );
//===[PROTOTYPES]====

int main( void )
{
	unsigned char timer = 0;
	DDRE = 0x00;
	while(1)
	{
		if(((PINE&1)==0) | (timer == 1))
		{
			timer += 1;
			timer0();
			timer0();
			timer0();
			timer0();
			timer0();
			timer -= 1;
			break;
		}	
	}
}

void timer0( void )
{
      // Initialize the timer.
      timer0_initialize();
      // Set up the port pin that we're going to 'toggle'.
      DDRD |= _BV( PD4 );
      // Initialize it to LOW.
      PORTD &= ~_BV( PD4 );
      // Loop forever
            // Set it.
            PORTD |= _BV( PD4 );
            // Wait.
            timer0_wait_until_OVF();
            // Clear it.
            PORTD &= ~_BV( PD4 );
            // Wait again...
            timer0_wait_until_OVF();
            // Repeat...
       // end while()
      return;
} // end main()

void timer0_initialize( void )
{
      TCNT0 = 0x00;
      TCCR0A = 0x00;
      TCCR0B = 0x00;
	  return;
}
 
void timer0_wait_until_OVF( void )
{
      TCNT0 = 132;
      TCCR0B = 0x02;
      while ((TIFR0 & 0x01) == 0);
      TCCR0B = 0x00;
      TIFR0 = 0x01;
	  return;
}
