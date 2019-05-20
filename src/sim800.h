/*
 * sim800.h
 *
 *  Created on: Feb 13, 2019
 *      Author: atakane
 */

#ifndef SIM800_H_
#define SIM800_H_

#include "usart.h"

#define GSM_BAUDRATE				9600

#define GSM_RESP_ERROR 				1
#define GSM_RESP_SUCCESS 			0

#define GSM_SIMPIN_REQUEST			1
#define GSM_SIMPIN_READY			0

#define GSM_CMD_AT_TEST				"AT\r\n"
#define GSM_CMD_SIMPIN_UNLOCK		"AT+CPIN=\"%d\"\r\n"
#define GSM_CMD_SIMPIN_STATUS		"AT+CPIN?\r\n"
#define GSM_CMD_NETWORK_INFO		"AT+COPS?\r\n"
#define GSM_CMD_NETWORK_ISREG		"AT+CREG?\r\n"
#define GSM_CMD_NETWORK_REG			"AT+CREG=1\r\n"
#define GSM_GET_MSG_FORMAT			"AT+CMGF?\r\n"
#define GSM_SET_MSG_FORMAT			"AT+CMGF=1\r\n"
#define GSM_GET_ALL_SMS				"AT+CMGL=\"ALL\"\r\n"
#define GSM_SET_BAUD_RATE			"AT+IPR=%d\r\n"
#define GSM_DELETE_INBOX			"AT+CMGD=1,4\r\n"
#define GSM_SEND_SMS				"AT+CMGS=\"%s\"\r\n"
#define GSM_SEND_SMS_MSG			"%s\r\n"





void GSM_Init(USART_TypeDef *USARTn, uint32_t BaudRate);
int GSM_Test(void);
int GSM_Resp_Check(char *resp);
void GSM_Cmd(char *cmd);
char *GSM_GetResponse(void);
int GSM_Resp_Check(char *resp);
char *GSM_NetworkInfo(void);
int GSM_SimStatus(void);
int GSM_UnlockSim(int pin);
int GSM_NetworkConnCheck(void);
void GSM_SetMsgFormat(void);
int GSM_NetworkConnReg(void);
char *GSM_GetAllSMS(void);
void GSM_SetBaudRate(uint32_t BaudRate);
void GSM_FreeCmdStr(void);
void GSM_DeleteInbox(void);
int GSM_SendSMS(char *addr, char *msg);

#endif /* SIM800_H_ */
