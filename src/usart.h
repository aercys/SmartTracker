/*
 * usart.h
 *
 *  Created on: Feb 11, 2019
 *      Author: atakane
 */

#ifndef USART_H_
#define USART_H_


#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x_conf.h"

#define USART1_RX	GPIO_Pin_10
#define	USART1_TX	GPIO_Pin_9

#define USART2_RX	GPIO_Pin_3
#define	USART2_TX	GPIO_Pin_2

#define USART3_RX	GPIO_Pin_11
#define	USART3_TX	GPIO_Pin_10


void UsartGpioInit(USART_TypeDef *USARTn);
void UsartInit(USART_TypeDef *USARTn, USART_InitTypeDef *USARTn_Init);
uint8_t UsartReceive(USART_TypeDef *USARTn);
void UsartSend(USART_TypeDef *USARTn, uint8_t UsartData);
void UsartSendString(USART_TypeDef *USARTn, char *UsartString);
char *UsartReceiveString(USART_TypeDef *USARTn);

#endif /* USART_H_ */
