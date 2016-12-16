#include<avr/io.h>
 
void timer0_initialize( void )
{
  	TCNT0 = 0x00;
  	TCCR0A = 0x00;
  	TCCR0B = 0x00;
}
 
void timer0_wait_until_OVF( void )
{
  	TCNT0 = 0xC3;
  	TCCR0B = 0x02;
  	while ((TIFR0 & 0x01) == 0);
  	TCCR0B = 0x00;
  	TIFR0 = 0x01;
}
 
int main( void )
{
  	// Initialize the timer.
  	timer0_initialize();
  	// Set up the port pin that we're going to 'toggle'.
  	DDRD |= _BV( PD4 );
  	// Initialize it to LOW.
  	PORTD &= ~_BV( PD4 );
  	// Loop forever.
  	while( 1 )
  	{
        	// Set it.
        	PORTD |= _BV( PD4 );
        	// Wait.
        	timer0_wait_until_OVF();
        	// Clear it.
        	PORTD &= ~_BV( PD4 );
        	// Wait again...
        	timer0_wait_until_OVF();
        	// Repeat...
  	} // end while()
  	return 0;
} // end main()
