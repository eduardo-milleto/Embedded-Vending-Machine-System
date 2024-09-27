#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

void LCD_Command(unsigned char cmnd) {
	PORTC = (PORTC & 0xF0) | (cmnd >> 4); // Envia os 4 bits mais significativos
	PORTD &= ~(1 << RS); // RS = 0 para comando
	PORTD |= (1 << EN); // EN = 1
	_delay_us(1);
	PORTD &= ~(1 << EN); // EN = 0
	_delay_us(200);
	PORTC = (PORTC & 0xF0) | (cmnd & 0x0F); // Envia os 4 bits menos significativos
	PORTD |= (1 << EN); // EN = 1
	_delay_us(1);
	PORTD &= ~(1 << EN); // EN = 0
	_delay_ms(2);
}

void LCD_Char(unsigned char data) {
	PORTC = (PORTC & 0xF0) | (data >> 4); // Envia os 4 bits mais significativos
	PORTD |= (1 << RS); // RS = 1 para dados
	PORTD |= (1 << EN); // EN = 1
	_delay_us(1);
	PORTD &= ~(1 << EN); // EN = 0
	_delay_us(200);
	PORTC = (PORTC & 0xF0) | (data & 0x0F); // Envia os 4 bits menos significativos
	PORTD |= (1 << EN); // EN = 1
	_delay_us(1);
	PORTD &= ~(1 << EN); // EN = 0
	_delay_ms(2);
}

void LCD_Init(void) {
	DDRC |= (1 << D4) | (1 << D5) | (1 << D6) | (1 << D7);
	DDRD |= (1 << RS) | (1 << EN);
	_delay_ms(20); // Espera por 20ms após ligar o LCD
	LCD_Command(0x02); // Inicializa o LCD em modo 4 bits
	LCD_Command(0x28); // 2 linhas, 5x7 matriz
	LCD_Command(0x0E); // Display ligado, cursor desligado
	LCD_Command(0x06); // Incrementa automaticamente o cursor
	LCD_Command(0x01); // Limpa o display
	_delay_ms(2);
	LCD_Command(0x80); // Coloca o cursor na linha 1, coluna 0
}

void LCD_Clear(void) {
	LCD_Command(0x01); // Comando para limpar o display
	_delay_ms(2);
	LCD_Command(0x80); // Coloca o cursor na linha 1, coluna 0
}

void LCD_SetCursor(unsigned char row, unsigned char col) {
	unsigned char pos = 0; // Inicializa `pos` com um valor padrão
	if (row == 0) {
		pos = 0x80 + col;
		} else if (row == 1) {
		pos = 0xC0 + col;
	}
	LCD_Command(pos);
}

void displayMessage(const char *str) {
	int i;
	int cursor_pos = 0; // Inicializa a posição do cursor

	for (i = 0; str[i] != '\0'; i++) {
		LCD_Char(str[i]);
		cursor_pos++;
		if (cursor_pos == 16) {
			// Move o cursor para o início da segunda linha após 16 caracteres
			LCD_SetCursor(1, 0);
			} else if (cursor_pos == 32) {
			// Parar se a mensagem ultrapassar a capacidade de 2 linhas do LCD
			break;
		}
	}
}



