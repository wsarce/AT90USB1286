#include <avr/io.h>
#include<stdint.h>
#include<avr/interrupt.h>	

//===[PROTOTYPES]===
void timer0_initialize(void);
void duty_cycle(uint8_t zero, uint8_t one);
ISR (TIMER0_OVF_vect);
//===[PROTOTYPES]===

int main(void)
{	
	// Configure Input/Outputs
    	DDRE = 0x00;                     		// PORTE is an input
   	DDRB = 0xFF;                       	// PORTB is an output
		
    	PORTB = 0xFF;                  		// Set PORTB High	
    
	timer0_initialize();                	// Initialize Timer0
   
	while(1)
	{  
        uint8_t value = PINE;         		// Variable = switch PINE
   		uint8_t zero = value & 0x01;   	// Saves the value of button 1
        uint8_t one = value & 0x02;  		// Saves the value of button 2
       	duty_cycle(zero, one);         	// Checks duty cycle to output
   	}	
    return 0;                 
}

// Timer 0 Initialization
void timer0_initialize(void)
{    
	TCCR0A = 0xC3;                     	// Set Timer0 to Fast PWM Mode
    	TCCR0B = 0x01;
   	TIMSK0 = 3;
    	TCNT0 = 0;                         	// Set the count value   
	
	return 0;
} 

void duty_cycle(uint8_t zero, uint8_t one)
{
	if(zero && one)
	{            
        	OCR0A = 0x3F;                 	// 25% Duty Cycle
    	}	
	
	else if(!zero && one)
	{ 
       	OCR0A = 0x7E;                  	// 50% Duty Cycle
    	}
	
   	else if(zero && !one)
	{          
		OCR0A = 0xBE;   			// 75% Duty Cycle
	}            
    
	else
	{                    
       	OCR0A = 0xFF;                   	// 100% Duty Cycle
    	} 
	return 0;
}                                 

ISR (TIMER0_OVF_vect)
{              
	PORTB ^= (1 << 7);						// Toggle PB7
}  
                                       