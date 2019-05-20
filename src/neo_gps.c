/*
 * neo_gps.c
 *
 *  Created on: Feb 9, 2019
 *      Author: atakane
 */

#include "neo_gps.h"
#include "usart.h"

void GPS_Init() {
	USART_InitTypeDef Usart;

	Usart.USART_BaudRate = 9600;
	Usart.USART_Mode = USART_Mode_Rx;
	Usart.USART_Parity = USART_Parity_No;
	Usart.USART_StopBits = USART_StopBits_1;
	Usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart.USART_WordLength = USART_WordLength_8b;

	UsartInit(USART1, &Usart);
}

uint8_t USART_Receive() {
	while(!(USART1->SR & USART_FLAG_RXNE));

	return (uint8_t) (USART1->DR & 0xFF);
}

char *get_gps_sentence() {
	char *gpsRawData = NULL;
	uint8_t buffer;
	int len = 0;

	while(1) {
		buffer = UsartReceive(USART1);
		if (buffer == '$') {
			while ('$' != (buffer = UsartReceive(USART1))) {
				gpsRawData = (char *) realloc(gpsRawData, sizeof(char *) * (len + 1));
				gpsRawData[len++] = buffer;
			}
			break;
		}
	}

	gpsRawData[len] = '\0';

	return gpsRawData;
}
