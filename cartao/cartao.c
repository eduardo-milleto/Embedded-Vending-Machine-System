#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "cartao.h"
#include "../lcd/lcd.h"
#include "../teclado/teclado.h"
#include "../opcao/opcao.h"
#include "../serial/serial.h" // Biblioteca de comunicação serial
#include <stdio.h>
#include <string.h>

// Estruturas e variáveis globais
#define LED PB5

typedef struct {
	char number[7];
	int balance; // Saldo em centavos
} Card;

extern char selectedProductCode[3]; // Ajustado para corresponder ao código principal
extern char preco[10]; // Preço do produto

Card cards[] = {
	{"300123", 100}, // 1,00 real em centavos
	{"300121", 2000}, // 20,00 reais em centavos
	{"250025", 2500}  // 25,00 reais em centavos
};
int numCards = sizeof(cards) / sizeof(cards[0]);

void sendCardMessage(const char* cardNumber) {
	char message[10];
	sprintf(message, "VC%s", cardNumber);
	SerialWriteMessage(message);
}

int getProductPrice(void) {
	// Converte o preço de string para inteiro em centavos
	int reais = 0, centavos = 0;
	sscanf(preco, "%d,%d", &reais, &centavos);
	return reais * 100 + centavos;
}

void readCardNumber(void) {
	LCD_Clear();  // Limpa a tela antes de exibir a nova mensagem
	_delay_ms(100);  // Adiciona um pequeno atraso para garantir que a tela seja limpa
	LCD_SetCursor(0, 0);
	displayMessage("Digite cartao:");
	LCD_SetCursor(1, 0);

	char cardNumber[7] = {0};
	for (int i = 0; i < 6; i++) {
		char key = '\0';
		while (key == '\0') {
			key = scanKeypad();
		}
		cardNumber[i] = key;
		LCD_Char(key);
		_delay_ms(300);
	}
	cardNumber[6] = '\0';

	// Exibe o saldo do cartão
	int cardBalance = checkCardBalance(cardNumber);
	if (cardBalance >= 0) {
		char balanceMsg[16];
		sprintf(balanceMsg, "Saldo: %d.%02d", cardBalance / 100, cardBalance % 100);
		LCD_Clear();
		LCD_SetCursor(0, 0);
		displayMessage(balanceMsg);
		_delay_ms(2000); // Delay de 2 segundos para o usuário confirmar
		} else {
		displayErrorMessage();
		return;
	}

	// Verifica se o saldo é suficiente antes de enviar a mensagem serial
	int productPrice = getProductPrice();
	if (productPrice > cardBalance) {
		LCD_Clear();
		displayMessage("SALDO INSUFICIENTE");
		_delay_ms(2000);
		requestProductCode();
		return;
	}

	// Aguarda a confirmação do usuário pressionando '#'
	int timeout = 4000; // Tempo limite de 4 segundos
	while (timeout > 0 && scanKeypad() != '#') {
		_delay_ms(100);
		timeout -= 100;
	}

	if (timeout <= 0) {
		// Tempo limite atingido, retorna para a solicitação do código do produto
		LCD_Clear();
		displayMessage("Tempo esgotado");
		_delay_ms(2000);
		requestProductCode();
		return;
	}

	// Envia a mensagem serial após a confirmação do usuário
	sendCardMessage(cardNumber);

	_delay_ms(5000); // Delay de 5 segundos para aguardar a resposta

	// Verifica a resposta serial do aplicativo externo
	char response[4] = {0};
	SerialReceiveMessage(3, response);

	// Processa a resposta do aplicativo externo
	if (strcmp(response, "AC0") == 0) {
		confirmPurchase(cardNumber);
		displaySuccessMessage();
		PORTB |= (1 << LED);
		_delay_ms(2000);
		PORTB &= ~(1 << LED);
		} else if (strcmp(response, "AC1") == 0) {
		LCD_Clear();
		displayMessage("PRODUTO INVALIDO");
		_delay_ms(2000);
		requestProductCode();
		} else if (strcmp(response, "AC2") == 0) {
		LCD_Clear();
		displayMessage("QTD INSUFICIENTE");
		_delay_ms(2000);
		requestProductCode();
		} else if (strcmp(response, "AC3") == 0) {
		LCD_Clear();
		displayMessage("VALIDADE VENCIDA");
		_delay_ms(2000);
		requestProductCode();
		} else if (strcmp(response, "AC4") == 0) {
		LCD_Clear();
		displayMessage("CARTAO INVALIDO");
		_delay_ms(2000);
		readCardNumber();
		} else {
		LCD_Clear();
		displayMessage("ERRO DESCONHECIDO");
		_delay_ms(2000);
		requestProductCode();
	}

	// Limpa a resposta
	memset(response, 0, sizeof(response));
}

int checkCardBalance(const char* cardNumber) {
	for (int i = 0; i < numCards; i++) {
		if (strcmp(cards[i].number, cardNumber) == 0) {
			return cards[i].balance;
		}
	}
	return -1;
}

void confirmPurchase(const char* cardNumber) {
	int productPrice = getProductPrice();
	int balance = checkCardBalance(cardNumber);
	if (balance >= productPrice) { // Comparando valores em centavos
		for (int i = 0; i < numCards; i++) {
			if (strcmp(cards[i].number, cardNumber) == 0) {
				cards[i].balance -= productPrice;
				return;
			}
		}
		} else {
		displayErrorMessage();
	}
}

void displayErrorMessage(void) {
	LCD_Clear();
	displayMessage("CARTAO INVALIDO");
	_delay_ms(2000);
	LCD_Clear();
	readCardNumber();
}

void displaySuccessMessage(void) {
	LCD_Clear();
	displayMessage("COMPRA CONFIRMADA");
	_delay_ms(2000);
}

void handleCardPayment(void) {
	readCardNumber();
}
