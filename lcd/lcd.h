#ifndef LCD_H_
#define LCD_H_

// Define os pinos do LCD
#define RS PD2
#define EN PD3
#define D4 PC0
#define D5 PC1
#define D6 PC2
#define D7 PC3

void LCD_Command(unsigned char cmnd);
void LCD_Char(unsigned char data);
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(unsigned char row, unsigned char col);
void displayMessage(const char *str);
void exibirClassificaNoLCD(char quantidadeDoRanking);

#endif /* LCD_H_ */
