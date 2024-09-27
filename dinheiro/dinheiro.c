#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "dinheiro.h"
#include "../cartao/cartao.h"
#include "../lcd/lcd.h"
#include "../teclado/teclado.h"
#include "../opcao/opcao.h"
#include "../serial/serial.h" // Biblioteca de comunicação serial
#include <stdio.h>
#include <string.h>

// Estruturas e variáveis globais
#define LED_PIN PB5

extern char selectedProductCode[3]; // Ajustado para corresponder ao código principal
extern char preco[10]; // Preço do produto

void handleCashPayment(void) {
	LCD_Clear();
	_delay_ms(100); // Aguarda um tempo após limpar o LCD

	int totalInserted = 0;
	char totalMsg[16];

	// Converte o preço de string para inteiro em centavos
	int reais = 0, centavos = 0;
	sscanf(preco, "%d,%d", &reais, &centavos);
	int productPrice = reais * 100 + centavos;

	sprintf(totalMsg, "Total: %d.%02d", totalInserted / 100, totalInserted % 100);
	LCD_Clear();
	LCD_SetCursor(0, 0);
	displayMessage(totalMsg);

	int timeout = 10000; // Timeout de 10 segundos para inserir moedas
	while (timeout > 0) {
		char key = scanKeypad();
		if (key == '1') {
			totalInserted += 100; // Adiciona 1 real
			} else if (key == '5') {
			totalInserted += 50; // Adiciona 50 centavos
		}

		sprintf(totalMsg, "Total: %d.%02d", totalInserted / 100, totalInserted % 100);
		LCD_SetCursor(0, 0);
		displayMessage(totalMsg);

		if (totalInserted >= productPrice) {
			break;
		}

		_delay_ms(100);
		timeout -= 100;
	}

	if (totalInserted >= productPrice) {
		// Envia a mensagem serial após a confirmação da compra
		SerialWriteMessage("VE");

		_delay_ms(5000); // Delay de 5 segundos para aguardar a resposta

		// Verifica a resposta serial do aplicativo externo
		char response[4] = {0};
		SerialReceiveMessage(3, response);

		// Processa a resposta do aplicativo externo
		if (strcmp(response, "AE0") == 0) {
			LCD_Clear();
			displaySuccessMessage();
			PORTB |= (1 << LED_PIN);
			_delay_ms(2000);
			PORTB &= ~(1 << LED_PIN);
			_delay_ms(2000);
			LCD_Clear();
			requestProductCode();
			return;
			} else if (strcmp(response, "AE1") == 0) {
			LCD_Clear();
			displayMessage("PRODUTO INVALIDO");
			_delay_ms(2000);
			requestProductCode();
			} else if (strcmp(response, "AE2") == 0) {
			LCD_Clear();
			displayMessage("QTD INSUFICIENTE");
			_delay_ms(2000);
			requestProductCode();
			} else if (strcmp(response, "AE3") == 0) {
			LCD_Clear();
			displayMessage("VALIDADE VENCIDA");
			_delay_ms(2000);
			requestProductCode();
			} else {
			LCD_Clear();
			displayMessage("ERRO DESCONHECIDO");
			_delay_ms(2000);
			requestProductCode();
		}

		// Limpa a resposta
		memset(response, 0, sizeof(response));
		} else {
		LCD_Clear();
		displayMessage("DINHEIRO INSUFICIENTE");
		_delay_ms(2000);
	}

	requestProductCode();
}
