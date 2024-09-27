#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "opcao.h"
#include "../lcd/lcd.h"
#include "../teclado/teclado.h"
#include "../serial/serial.h"
#include "../cartao/cartao.h" // Biblioteca para pagamento com cartão
#include "../dinheiro/dinheiro.h" // Biblioteca para pagamento com dinheiro
#include <string.h>
#include <stdio.h>

char selectedProductCode[3] = {0}; // Permitindo espaço para um código de produto de dois dígitos e um terminador nulo
char nomeComida[30] = {0}; // Nome do produto
char preco[10] = {0}; // Preço do produto

void requestProductCode() {
	LCD_Clear();
	_delay_ms(100);  // Aguarda um tempo após limpar o LCD
	LCD_SetCursor(0, 0);
	displayMessage("Digite codigo:");
	_delay_ms(100);  // Aguarda um tempo após setar a mensagem
	LCD_SetCursor(1, 0);

	char key = '\0';
	int index = 0;
	int timeout = 2000; // 2 segundos de timeout

	while (index < 2 && timeout > 0) {
		key = scanKeypad();
		if (key != '\0') {
			selectedProductCode[index++] = key;
			LCD_Char(key);
			timeout = 2000; // Reinicia o timeout após cada entrada
		}
		_delay_ms(100); // Delay para diminuir a frequência do loop
		timeout -= 100;
	}
	selectedProductCode[index] = '\0'; // Certifica-se de que a string está terminada

	_delay_ms(2000); // Atraso de 2 segundos após a entrada do código do produto

	int messageLength = 0;

	// Envia o código do produto via serial usando switch-case
	if (strcmp(selectedProductCode, "1") == 0) {
		SerialWriteMessage("VP01\n");
		messageLength = 22;
		} else if (strcmp(selectedProductCode, "2") == 0) {
		SerialWriteMessage("VP02\n");
		messageLength = 25;
		} else if (strcmp(selectedProductCode, "3") == 0) {
		SerialWriteMessage("VP03\n");
		messageLength = 25;
		} else if (strcmp(selectedProductCode, "7") == 0) {
		SerialWriteMessage("VP07\n");
		messageLength = 21;
		} else if (strcmp(selectedProductCode, "8") == 0) {
		SerialWriteMessage("VP08\n");
		messageLength = 25;
		} else if (strcmp(selectedProductCode, "9") == 0) {
		SerialWriteMessage("VP09\n");
		messageLength = 23;
		} else if (strcmp(selectedProductCode, "11") == 0) {
		SerialWriteMessage("VP11\n");
		messageLength = 23;
		} else if (strcmp(selectedProductCode, "12") == 0) {
		SerialWriteMessage("VP12\n");
		messageLength = 22;
		} else if (strcmp(selectedProductCode, "13") == 0) {
		SerialWriteMessage("VP13\n");
		messageLength = 23;
		} else {
		// Exibe mensagem de código inválido
		displayMessage("Codigo invalido");
		return;
	}

	LCD_Clear();

	// Recebe as informações do produto via serial até o delimitador '\n'
	char productInfo[50]; // Aumenta o tamanho para acomodar o terminador nulo e mensagem maior
	SerialReceiveMessage(messageLength, productInfo);

	// Certifica-se de que a string está terminada
	productInfo[messageLength] = '\0';

	// Processa a mensagem recebida
	int i = 3; // Ignora os primeiros 3 caracteres (AP e espaço)
	int j = 0;

	// Extrair nomeComida
	while (productInfo[i] != '\0') {
		nomeComida[j++] = productInfo[i++];
	}

	// Procurar o início do preço após o nome da comida
	i = j; // Posição após o nome da comida
	while (productInfo[i] != '\0') {
		i++;
	}
	i++; // Avançar para o próximo caractere após '\0'

	// Extrair preço
	j = 0;
	while (productInfo[i] != '\0') {
		preco[j++] = productInfo[i++];
	}
	preco[j] = '\0';

	// Exibir nome da comida e preço
	LCD_Clear();
	LCD_SetCursor(0, 0);
	displayMessage(nomeComida);
	LCD_SetCursor(1, 0);
	displayMessage("R$");
	displayMessage(preco);
	_delay_ms(3000);
	LCD_Clear();
	
	// Solicitar forma de pagamento
	displayMessage("1.Cartao");
	LCD_SetCursor(1, 0);
	displayMessage("2.Dinheiro");

	// Captura a escolha de pagamento
	key = '\0';
	while (key != '1' && key != '2') {
		key = scanKeypad();
	}

	// Processa a forma de pagamento
	if (key == '1') {
		// Chama a função de pagamento com cartão da biblioteca de cartão
		handleCardPayment();
		} else if (key == '2') {
		// Chama a função de pagamento com dinheiro da biblioteca de dinheiro
		handleCashPayment();
	}
}
