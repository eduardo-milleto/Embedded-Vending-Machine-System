#ifndef SERIAL_H
#define SERIAL_H

void setupUSART();
void SerialReceiveMessage(int sizeS, char* string);
void SerialWriteChar(unsigned char c);
void SerialWriteMessage(const char *msg);

#endif // SERIAL_H
