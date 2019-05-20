/*
 * usart.c
 *
 *  Created on: Feb 11, 2019
 *      Author: atakane
 */


#include "usart.h"
#include "delay.h"


void UsartInit(USART_TypeDef *USARTn, USART_InitTypeDef *USARTn_Init) {

	if (USARTn == USART1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		UsartGpioInit(USART1);
	} else if (USARTn == USART2) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		UsartGpioInit(USART2);
	} else if (USARTn == USART3) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		UsartGpioInit(USART3);
	}


	USART_Init(USARTn, USARTn_Init);
	USART_Cmd(USARTn, ENABLE);
	USART_ClearFlag(USARTn, USART_FLAG_TC);
}

void UsartGpioInit(USART_TypeDef *USARTn) {
	GPIO_InitTypeDef 	GPIO;
	GPIO_TypeDef		*GPIOn;

	if (USARTn == USART1) {
		GPIOn = GPIOA;
		GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO.GPIO_Pin = USART1_RX;

		GPIO_Init(GPIOn, &GPIO);
		GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO.GPIO_Pin = USART1_TX;
		GPIO.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOn, &GPIO);
	} else if (USARTn == USART2) {
		GPIOn = GPIOA;
		GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO.GPIO_Pin = USART2_RX;

		GPIO_Init(GPIOn, &GPIO);
		GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO.GPIO_Pin = USART2_TX;
		GPIO.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOn, &GPIO);
	} else if (USARTn == USART3) {
		GPIOn = GPIOB;
		GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO.GPIO_Pin = USART3_RX;

		GPIO_Init(GPIOn, &GPIO);
		GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO.GPIO_Pin = USART3_TX;
		GPIO.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOn, &GPIO);
	}

}

uint8_t UsartReceive(USART_TypeDef *USARTn) {
	uint8_t UsartData = 0x00;

	if (USARTn == USART1) {
		while(!(USART1->SR & USART_FLAG_RXNE));
		UsartData = (uint8_t) (USART1->DR & 0xFF);
	} else if (USARTn == USART2) {
		while(!(USART2->SR & USART_FLAG_RXNE));
		UsartData = (uint8_t) (USART2->DR & 0xFF);
	} else if (USARTn == USART3) {
		while(!(USART3->SR & USART_FLAG_RXNE));
		UsartData = (uint8_t) (USART3->DR & 0xFF);
	}

	return UsartData;
}

void UsartSend(USART_TypeDef *USARTn, uint8_t UsartData) {
	if (USARTn == USART1) {
		USART1->DR = UsartData;
		while(!(USART1->SR & USART_FLAG_TXE));
	} else if (USARTn == USART2) {
		USART1->DR = UsartData;
		while(!(USART2->SR & USART_FLAG_TXE));
	} else if (USARTn == USART3) {
		USART1->DR = UsartData;
		while(!(USART3->SR & USART_FLAG_TXE));
	}
}

void UsartSendString(USART_TypeDef *USARTn, char *UsartString) {
	while (*UsartString != '\0') {
		//trace_printf("Sending char: %c\n", (char) *UsartString);
		UsartSend(USARTn, *UsartString++);
	}
}

char *UsartReceiveString(USART_TypeDef *USARTn) {
	char *UsartString = NULL;
	char UsartData;
	int len = 0;

	while((UsartData = UsartReceive(USARTn)) != '\n') {
		UsartString = (char *) realloc(UsartString, sizeof(char) * (len + 1));
		UsartString[len] = UsartData;
		len++;
	}

	UsartString[len] = '\0';

	return UsartString;
}
