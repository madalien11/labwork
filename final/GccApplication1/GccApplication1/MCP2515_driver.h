#include "SPI_driver.h"

char read(char address);
void write(char address, char data);
void RTS(char buffer);
char read_status();
void bit_modify(char address, char mask, char data);
void reset();
