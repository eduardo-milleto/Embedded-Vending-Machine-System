#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "Serial.h"


void setupUSART(){
	
/*Pino 5 da porta B como saída*/
	DDRB |= (1<<5);
	
/*Habilita interrupções*/
	sei();
	
/* baud rate 19200 */
	UBRR0H = 0;
	UBRR0L = 51;

/* transmissor ativo*/
	UCSR0B |= (1<<3);

/* receptor ativo*/
	UCSR0B |= (1<<4);

/* formato: 8 + 1 stop bit */
	UCSR0C = 0x6;
	
	
}

// Armazena os caracteres recebidos em RX na string especificada. É necessário especificar o tamanho da String 
// para funcionar corretamente. Só sai do loop quando receber todos os caracteres. 
void SerialReceiveMessage(int sizeS, char* string){
	
	int i;
	
	for(i = 0; i < sizeS; i++){
		
		while ( !(UCSR0A & (1<<RXC0)));
		
		string[i] = UDR0;
	}

}


// Escreve um único caracter em TX
void SerialWriteChar(unsigned char c){
	UDR0 = c;
	while ((UCSR0A & (1<<6))==0);
	UCSR0A &= ~(1<<6);
	
}



// Escreve a mensagem especificada em TX
void SerialWriteMessage(const char *msg){
	int i;

	for (i = 0; msg[i] != 0; i++) {
		SerialWriteChar(msg[i]);
	}

}