#ifndef TECLADO_H
#define TECLADO_H

char scanKeypad(void);
char debounce(char coluna);

// Define os pinos do teclado
#define R1 PD6
#define R2 PD7
#define R3 PB0
#define R4 PB1
#define C1 PB2
#define C2 PB3
#define C3 PB4

#endif // TECLADO_H
