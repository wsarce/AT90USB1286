#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

//===[PROTOTYPES]===
void USART_configure(void);
void USART_enable_TX(void);
void USART_enable_RX(void);
void USART_send(uint8_t data);
uint8_t USART_get(void);
void LCD_init();
void LCD_write_command(uint8_t command);
void LCD_write_data(uint8_t data);
uint8_t EEPROM_read(uint16_t uiAddress);
void EEPROM_write(uint16_t uiAddress, uint8_t ucData);
//===[PROTOTYPES]===
 
int main ( void )
{
       uint8_t data;                    
       uint8_t buffer [32];
       uint8_t message[] ="°Enter an Input°\r\n";        
       uint8_t i;                                             
       uint8_t j;                                             
       uint8_t p = 0;                   
       uint8_t printed = 0;
       uint16_t addr  = 0x0000; 
 
       DDRB = 0xFF;
       DDRD |= _BV(4) | _BV(5) | _BV(6);
       PORTB &= ~_BV(5);          

       _delay_ms(20);
       LCD_init();
       LCD_write_command(0x01);

       DDRE &= ~_BV(0);

       USART_configure();
       USART_enable_RX();
       USART_enable_TX();
 
       while (1)
	   {
			if (!(PINE & _BV(0)))
            {             
            	i = 0;

                while( message[ i ] != '\0' )
                	{
                    	USART_send( message[ i++ ] );
                    }

                    i = 0;
                    data = USART_get();

                while (data != '\r')
                    {
                    	buffer[i++] = data;
                    	data = USART_get();
                    }	

                    addr = 0;
                    for(j = 0; j<i; j++)
                    {
                    	data = buffer[j];
                    	EEPROM_write(addr++, data);
					}

                    EEPROM_write(addr++,'\0');
                    printed = 0;
                    }

                    else
                    {
                    	if (!printed)
                        {
                        	addr = 0;
                        	p = 0;
                        	LCD_write_command(0x01);
                        	data = EEPROM_read(addr++);
                        		while (data != '\0')
                        		{
                            		if (p == 16)
                               		LCD_write_command(0xC0);
                                	LCD_write_data(data);
                                	data = EEPROM_read(addr++);
                                	p++;
                            	}
                        }            
                     printed = 1;
                    }          
		}
}
 
void USART_configure(void)
{
       UBRR1H = 0;
       UBRR1L = 12;
 
       UCSR1A = 2;
       UCSR1B = 0;
 
       UCSR1C = 0b00000110;
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
 
uint8_t USART_get(void)
{
       while ( !(UCSR1A & (1<<RXC1)) );
       return UDR1;
}

void LCD_init()
{
       _delay_ms(20);
       LCD_write_command(0x30);
       _delay_ms(4);
       LCD_write_command(0x30);
       _delay_us(100);
       LCD_write_command(0x30);
       LCD_write_command(0x38);
       LCD_write_command(0x08);
       LCD_write_command(0x01);
       LCD_write_command(0x06);
       LCD_write_command(0x0F);
}

void LCD_write_command(uint8_t command)
{
       PORTB = command;
       PORTD &= ~_BV(6);
       PORTD &= ~_BV(4);
 
       PORTD |= _BV(5);
       PORTD &= ~_BV(5);
       _delay_ms(20);
}

void LCD_write_data(uint8_t data)
{
       PORTB = data;
       PORTD |= _BV(6);
       PORTD &= ~_BV(4);
 
       PORTD |= _BV(5);
       PORTD &= ~_BV(5);
       _delay_ms(20);
}

uint8_t EEPROM_read(uint16_t uiAddress)
{      
       while(EECR & (1<<EEPE));
       EEAR = uiAddress;
       EECR |= (1<<EERE);
       return EEDR;
}

void EEPROM_write(uint16_t uiAddress, uint8_t ucData)
{
       while(EECR & (1<<EEPE));
       EEAR = uiAddress;
       EEDR = ucData;
       EECR |= (1<<EEMPE);
       EECR |= (1<<EEPE);
}
