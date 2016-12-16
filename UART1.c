#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//===[PROTOTYPES]===
void USART_configure(void);
void USART_enable_TX(void);
void USART_enable_RX(void);
void USART_send(uint8_t data);
uint8_t USART_get(void);
//===[PROTOTYPES]===

int main (void)
{
	USART_configure();
	USART_enable_TX();

	uint8_t dataLength = 14;
	uint8_t str[12] = "Walker Arce\r";
	
	uint8_t counter = 0;
	
	while (1)
	{
		while ( counter < dataLength)

			USART_send(str[counter++]);

		counter = 0;

		_delay_ms(250);
		_delay_ms(250);
		_delay_ms(250);
		_delay_ms(250);
	}
	return 0;
}

void USART_configure(void)
{
	UCSR1A |= _BV( U2X1 );
	UCSR1B = 0;
	UCSR1C = 6;
	UBRR1H = 0;
	UBRR1L = 12;
}

void USART_enable_TX(void)
{
	UCSR1B |= 0x08;
}

void USART_enable_RX(void)
{
	UCSR1B |= 0x10;
	
}

void USART_send(uint8_t data)
{
	while ( !( UCSR1A & (1<<UDRE1)) ); 
	UDR1 = data;
}
