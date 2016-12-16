#include<avr/io.h>
 
void timer0_initialize( void )
{
  	DDRB = 0xFF;
  	TCNT0 = 0x00;
  	TCCR0A = 0x42;
  	TCCR0B = 0x00;
  	OCR0A = 0x3D;
}
 
void timer0_start_signal( void )
{
  	TCCR0B = 0x02;
}
 
int main( void )
{
  	// Initialize the timer.
  	timer0_initialize();
  	// Start the timer and thus, start the square wave signal out of
  	// PB7 (aka, OC0A pin).
  	timer0_start_signal();
  	// Do nothint else, let the harware do the rest.
  	while( 1 );
  	return 0;
} // end main()