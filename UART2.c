#include <avr\io.h>
#include <util\delay.h>

//===[PROTOTYPES]===
void LCD_init (void);
void LCD_write (void);
void usart_init (void);
//===[PROTOTYPES]===

int main (void)
{                            
	DDRB = 0xFF;
	DDRD = 0xFF;
	PORTD = 0x00;
	LCD_init ();
	usart_init();
	while(1)
		{
		while (! (UCSR1A & (1<<RXC1) ) );
			LCD_write();
		}
	return 0;
}

void LCD_init (void)
{
	_delay_ms(20);
	PORTB = 0x38;
	PORTD = 0x20;
	_delay_ms(20);
	PORTD = ~(1<<5);
	_delay_ms(20);
	PORTB = 0x0E;
	PORTD = 0x20;
	_delay_ms(20);
	PORTD = ~(1<<5);
	_delay_ms(20);
	PORTB = 0x01;
	PORTD = 0x20;
	_delay_ms(20);
	PORTD = ~(1<<5);
	_delay_ms(20);
}
 
void LCD_write(void)
{
	PORTB =UDR1;
	PORTD = 0x60;
	_delay_us(20);
	PORTD = ~ (1<< 5);
	_delay_us(20);
}
 
void usart_init (void)
{         
	UCSR1B = (1<<RXEN1);
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10)|(1<<UMSEL11);
	UCSR1A = 0x02;
	UBRR1H = 0x00;
	UBRR1L = 0x0C; // 9,600 baud rate @ 8MHZ
}
