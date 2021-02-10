/*
 * button_uart.cpp
 *
 * Created: 30/12/2020 15:12:11
 * Author : Ngo Quoc Nhu
 */ 
#define CPU_F 8000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/sfr_defs.h>

#define sbi(PORT,bit) {PORT |= (1<<bit);}
#define cbi(PORT,bit) {PORT &= ~(1<<bit);}

char check_button();
void uart_char_tx(unsigned char chr);
int main(void)
{
	//USART_Init(9600);
	
	UBRRH=0;
	UBRRL=51;
	
	UCSRA=0x00;
	UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB=(1<<TXEN);
	
	
	DDRC = 0x00;
	PORTC = 0x00;
	char code = '0';
    while (1) 
    {	
		code = check_button();
		if (code == '1' || code == '2' || code == '3' || code == '4'){
			
			//USART_TxChar(code);
			uart_char_tx(code);
			_delay_ms(100);
			code = '0';
		}
    }
}

char check_button(){
	if (bit_is_clear(PINC, 0)){
		while (bit_is_clear(PINC, 0)){
			// nothing 
		}
		return '1';
	}
	if (bit_is_clear(PINC, 1)){
		while (bit_is_clear(PINC, 1)){
			// nothing
		}
		return '2';
	}
	if (bit_is_clear(PINC, 2)){
		while (bit_is_clear(PINC, 2)){
			// nothing
		}
		return '3';
	}
	if (bit_is_clear(PINC, 3)){
		while (bit_is_clear(PINC, 3)){
			// nothing
		}
		return '4';
	}
	return '0';
};

void uart_char_tx(unsigned char chr){
	while (bit_is_clear(UCSRA,UDRE)) {}; //cho den khi bit UDRE=1
	UDR=chr;
}
