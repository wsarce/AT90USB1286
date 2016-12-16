#include <avr/io.h>			// Include AVR library
#include <util/delay.h>		// Include delay utilities
#include <stdint.h>			// Include uint8_t library

//=====[PROTOTYPES]=====
uint8_t READ_DIP( void );
uint8_t GEN_RAN( uint8_t value );
void DISPLAY_NUM( uint8_t val1 );
void PRINT_BINARY_STRING( uint8_t val1 );
void LCD_INIT( void );
void LCD_DATA( unsigned char data );
void LCD_CMD( unsigned char cmd );
void LCD_CLEAR( void );
void LCD_CHAR( unsigned char data );
//=====[PROTOTYPES]=====

int main(void)
{
	uint8_t value = 0;		// Set initial variable
	
	unsigned char r16 = 0xFF;	// Set DDRx
	DDRD = r16;
	DDRB = r16;
	r16 = 0x00;
	DDRE = r16;
	
    while(1)
    {
       LCD_INIT();			// Initialize LCD
	READ_DIP();			// Read DIP and generate first random number
	DISPLAY_NUM( value );	// Display to LCD, generate second number, display
    }
}

uint8_t READ_DIP( void )
{
uint8_t value = ~PINE;	// Import value from DIP switches, invert
	GEN_RAN( value );		// Generate random number
	return value;			// return random number
}

uint8_t GEN_RAN( uint8_t value )
{
	uint8_t val1, val2;		// Set temporary variables
	
	value = value<<1;		// Left shift number once
	val1 = value;			// Move value into val1
	val2 = value;			// Move value into val2
	
	val1 &= 0x80;			// Mask all but LSB
	val1 = val1>>7;		// Shift LSB into MSB spot
	
	val2 &= 0x40;			// Mask all but bit six
	val2 = val2>>6;		// Shift bit six into MSB
	
	val1 ^= val2;			// EOR the two MSBs
	
	value |= val1;		// OR into first value
	value &= 0x7F;		// Clear LSB
	
	return value;			// Return random number
}

void DISPLAY_NUM( uint8_t val1 )
{
	uint8_t val2;			// Initialize second random number
	uint8_t value = 0;		// Initialize for GEN_RAN function
	
	LCD_CHAR('F');		// Send 'First: ' to LCD
	LCD_CHAR('i');
	LCD_CHAR('r');
	LCD_CHAR('s');
	LCD_CHAR('t');
	LCD_CHAR(':');
	LCD_CHAR(' ');
	
	PRINT_BINARY_STRING( val1 );	// Print first binary number
	
	GEN_RAN( val1 );		// Generate second random number
	val2 = value;			// Move value to val2
	
	LCD_CMD( 0xC0 );		// Move cursor to second line
	LCD_CHAR('S');		// Send 'Second: ' to LCD
	LCD_CHAR('e');
	LCD_CHAR('c');
	LCD_CHAR('o');
	LCD_CHAR('n');
	LCD_CHAR('d');
	LCD_CHAR(':');
	LCD_CHAR(' ');
	
	PRINT_BINARY_STRING( val2 );	// Print second random number
	
	return;			// Return
}

void PRINT_BINARY_STRING( uint8_t val1 )
{
	uint8_t gen, num;			// Initialize template variables
	
	for(num = 0; num<<8; num++)	// Begin enhanced for loop
	{
		gen = val1;			// Move val1 into gen
		gen &= 1<<(7-num);		// Begin checking only MSB
		
		if( gen > 0 )			// If one, then send '1' to LCD
		{
			LCD_CHAR('1');
		}
		else
		{
			LCD_CHAR('0');	// Otherwise send '0' to LCD
		}
	}
}

void LCD_INIT(void)
{
	_delay_ms(15);		// Delay 15 milliseconds
	LCD_CMD(0x30);		// Send 0x30 to command register of LCD
	_delay_ms(15);		// Delay 15 milliseconds
	
	LCD_CMD(0x30);		// Send 0x30 to command register of LCD
	_delay_ms(1);			// Delay one millisecond
	
	LCD_CMD(0x30);		// Begin initialization process
	LCD_CMD(0x30);
	LCD_CMD(0x3C);
	LCD_CMD(0x08);
	LCD_CMD(0x06);
	LCD_CMD(0x0F);
	
	LCD_CLEAR();			// Clear LCD

	PORTF=1<<7;			// Turn on the backlight
}

void LCD_DATA(unsigned char data)
{
	PORTB = data;				// Move data to PORTB
	PORTD = (1<<6)|(0<<4)|(1<<5);	// Select DDRx / Select Write / Set Enable to HIGH
	_delay_ms(1);				// Delay for one millisecond
	PORTD = (0<<5);			// Set Enable pin to LOW
}

void LCD_CMD(unsigned char cmd)
{
	PORTB = cmd;				// Move cmd into PORTB
	PORTD = (0<<6)|(0<<4)|(1<<5);	// Select command register / Select Write / Set Enable to HIGH
	_delay_ms(1);				// Delay one millisecond
	PORTD = (0<<5);			// Set Enable to LOW
}

void LCD_CLEAR(void)
{
	LCD_CMD(0x01);			// Send 0x01 to clear LCD
}

void LCD_CHAR(unsigned char data)
{
	LCD_DATA( data );			// Call LCD_DATA
}
