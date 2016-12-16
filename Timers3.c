#include<avr/io.h>

//PROTOTYPE	
void timer1_delay_3sec( void );
//PROTOTYPE

void timer1_delay_3sec( void ){
		
	TCCR1A = 0x00;
	TCCR1B = 0x05;
		
	TCNT1H = 0xF4;
	TCNT1L = 0x47;
		
	// monitor button and timer
	while ((( TIFR1 & 1 ) == 0 ) && (( PINE & 1 ) == 0));

	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TIFR1 = 0x01;
		
return;
}

int main( void ){
		
	DDRE = 0x00;
	DDRC = 0xFF;						// set I/O

	while ( 1 ){
			
		PORTC = 0xFF;

		while(( PINE & 1 ) == 0){

			PORTC = 0xFE;

			timer1_delay_3sec();		// turn 1st LED on

			if(( PINE & 1 ) == 1)		// delay
			{
				break;
			}
			PORTC = 0xFC;			// turn 2nd LED on

			while(( PINE & 1 ) == 0 );
			break;					// break if button off
		}
	}
	return 0;
} // END MAIN
