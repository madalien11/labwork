#define SS_Enable() PORTB &= ~(1<<PB4)
#define SS_Disable() PORTB |= (1<<PB4) 

void SPI_master_init();

void SPI_MasterTransmit(char cData);

char SPI_MasterReceive();

void SPI_SlaveInit(void);

char SPI_SlaveReceive(void);