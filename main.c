#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "opcao/opcao.h"
#include "lcd/lcd.h"
#include "teclado/teclado.h"
#include "serial/serial.h"

// Define os pinos do LED
#define LED PD3

int main(void) {
	// Configura os pinos do LCD
	DDRD |= (1 << PD2) | (1 << PD3); // RS e EN
	DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3); // D4, D5, D6, D7

	// Configura os pinos do LED
	DDRD |= (1 << LED);

	// Configura os pinos das linhas do teclado como saída
	DDRD |= (1 << PD6) | (1 << PD7);
	DDRB |= (1 << PB0) | (1 << PB1);

	// Configura os pinos das colunas do teclado como entrada com pull-up
	DDRB &= ~((1 << PB2) | (1 << PB3) | (1 << PB4));
	PORTB |= (1 << PB2) | (1 << PB3) | (1 << PB4);

	// Inicializa o LCD
	LCD_Init();

	// Inicializa o UART
	setupUSART();

	while (1) {
		
		requestProductCode();

	}
}