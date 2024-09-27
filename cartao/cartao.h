#ifndef CARTAO_H
#define CARTAO_H

void handleCardPayment();
void displayErrorMessage();
void displaySuccessMessage();
int checkCardBalance(const char* cardNumber);
void confirmPurchase(const char* cardNumber);

#endif // CARTAO_H
