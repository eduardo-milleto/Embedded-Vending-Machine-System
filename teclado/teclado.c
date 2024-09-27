#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "teclado.h"

void atrasotemp1ms() {
	_delay_ms(1);
}

char debounce(char coluna) {
	char BOUNCE = 7;
	char Count = 0;
	char Key_now = 1;  // Inicializando Key_now
	char Key_last = 1;  // Inicializando Key_last
	
	while (Count < BOUNCE) {
		atrasotemp1ms(); // Espera um pouco antes de ler novamente
		Key_now = PINB & (1 << coluna); // Lê o estado atual da chave
		if (Key_now != Key_last) { // Se a chave mudou desde a última leitura
			Count = 0; // Reinicia o contador se a chave não estiver estável
			} else {
			Count++; // Incrementa o contador se a chave permaneceu a mesma
		}
		Key_last = Key_now; // Atualiza a última chave lida
	}

	return Key_now; // Retorna a chave quando estável
}

char scanKeypad(void) {
	// Configura as linhas do teclado
	PORTD &= ~(1 << R1);
	PORTD |= (1 << R2);
	PORTB |= (1 << R3) | (1 << R4);
	
	// Verifica as colunas
	if (debounce(C1) == 0) return '1';
	if (debounce(C2) == 0) return '2';
	if (debounce(C3) == 0) return '3';
	
	PORTD &= ~(1 << R2);
	PORTD |= (1 << R1);
	PORTB |= (1 << R3) | (1 << R4);
	
	if (debounce(C1) == 0) return '4';
	if (debounce(C2) == 0) return '5';
	if (debounce(C3) == 0) return '6';
	
	PORTB &= ~(1 << R3);
	PORTD |= (1 << R1) | (1 << R2);
	PORTB |= (1 << R4);
	
	if (debounce(C1) == 0) return '7';
	if (debounce(C2) == 0) return '8';
	if (debounce(C3) == 0) return '9';
	
	PORTB &= ~(1 << R4);
	PORTD |= (1 << R1) | (1 << R2);
	PORTB |= (1 << R3);
	
	if (debounce(C1) == 0) return '*';
	if (debounce(C2) == 0) return '0';
	if (debounce(C3) == 0) return '#';
	
	return '\0';
}
