#include "CAN_registers.h"
#include <stdint.h>

void CAN_init();

struct CAN_msg{
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
};


uint8_t CAN_transmit(struct CAN_msg *msg);

uint8_t CAN_receive(struct CAN_msg *msg);