#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

//===[PROTOTYPES]===
void timer0_initialize( void );
void timer0_start_signal( void );
ISR (TIMER0_COMPA_vect);
ISR (INT0_vect);
ISR (INT1_vect);
//===[PROTOTYPES]===
 
int main( void )
{
	// Set initial states
	sei();
	DDRA = 0xFF;
	DDRE = 0x00;
	DDRC = 0xFF;

	//Initialize timer
  	timer0_initialize();
	EICRA = 0x09;
	EIMSK = 0x03;
  
  	// Begin signal generation
  	timer0_start_signal();
  	
	// Wait for interrupts
  	while( 1 )
	{
		PORTC = PINE;
	}
  	return 0;
} // end main()

void timer0_initialize( void )
{
	// Set initial states
  	DDRB = 0xFF;
	// Clear timer counter
  	TCNT0 = 0x00;
	// Set to CTC mode with a prescaler of 8
  	TCCR0A = 0x42;
  	TCCR0B = 0x00;
	// Define when to interrupt the program
	TIMSK0 = 0x02;
	// Load flag trigger
  	OCR0A = 250;
}
 
void timer0_start_signal( void )
{
	// Begin signal generation
  	TCCR0B = 0x01;
}

ISR (TIMER0_COMPA_vect)
{
	// When the compare flag is set, toggle pin 2 of PORTA
	PORTA ^= 0x04;
}

ISR (INT0_vect)
{
	// When first external interrupt set, toggle pin 0 of PORTA
	PORTA ^= 0x01;
}

ISR (INT1_vect)
{
	// When the second external interrupt set, toggle pin 1 of PORTA
	PORTA ^= 0x02;
	// Delay 15 milliseconds
	_delay_ms(15);
}