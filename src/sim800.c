/*
 * sim800.c
 *
 *  Created on: Feb 13, 2019
 *      Author: atakane
 */

#include <stdio.h>
#include <string.h>

#include "sim800.h"
#include "delay.h"


USART_TypeDef *USART_Port = USART1;
char GSM_CmdStr[250];

void GSM_Init(USART_TypeDef *USARTn, uint32_t BaudRate) {
	USART_InitTypeDef Usart;

	Usart.USART_BaudRate = BaudRate;
	Usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart.USART_Parity = USART_Parity_No;
	Usart.USART_StopBits = USART_StopBits_1;
	Usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart.USART_WordLength = USART_WordLength_8b;

	UsartInit(USARTn, &Usart);
}

int GSM_Test() {
	char *resp;
	GSM_Cmd(GSM_CMD_AT_TEST);
	resp = GSM_GetResponse();
	return GSM_Resp_Check(resp);
}

int GSM_Resp_Check(char *resp) {
	if (strstr(resp, "OK") != NULL) {
		return GSM_RESP_SUCCESS;
	} else {
		return GSM_RESP_ERROR;
	}
}

void GSM_Cmd(char *cmd) {
	UsartSendString(USART_Port, cmd);
}

char *GSM_NetworkInfo() {
	char *resp;
	GSM_Cmd(GSM_CMD_NETWORK_INFO);
	resp = GSM_GetResponse();
	return resp;
}

int GSM_SimStatus() {
	char *resp;
	GSM_Cmd(GSM_CMD_SIMPIN_STATUS);
	resp = GSM_GetResponse();
	if (strstr(resp, "SIM PIN") != NULL)
		return GSM_SIMPIN_REQUEST;
	else
		return GSM_SIMPIN_READY;
}

int GSM_UnlockSim(int pin) {
	char *resp;

	sprintf(GSM_CmdStr, GSM_CMD_SIMPIN_UNLOCK, pin);
	GSM_Cmd(GSM_CmdStr);
	resp = GSM_GetResponse();

	return GSM_Resp_Check(resp);
}

int GSM_NetworkConnCheck() {
	char *resp;

	GSM_Cmd(GSM_CMD_NETWORK_ISREG);
	resp = GSM_GetResponse();

	if (strstr(resp, "0,1") != NULL) {
		return GSM_RESP_ERROR;
	} else {
		return GSM_RESP_SUCCESS;
	}
}

int GSM_NetworkConnReg() {
	char *resp;

	GSM_Cmd(GSM_CMD_NETWORK_REG);
	resp = GSM_GetResponse();

	return GSM_Resp_Check(resp);
}

void GSM_SetMsgFormat() {
	char *resp;

	GSM_Cmd(GSM_GET_MSG_FORMAT);
	resp = GSM_GetResponse();

	if (strstr(resp, "CMGF: 0") != NULL) {
		GSM_Cmd(GSM_SET_MSG_FORMAT);
		resp = GSM_GetResponse();
	}
}

char *GSM_GetAllSMS(void) {
	char *resp;
	GSM_Cmd(GSM_GET_ALL_SMS);
	resp = GSM_GetResponse();

	return resp;
}

char *GSM_GetResponse() {
	char *AT_Resp = NULL;
	int offset = 0, total = 0;

	while(1) {
		char *data = UsartReceiveString(USART_Port);
		total += strlen(data);
		AT_Resp = (char *) realloc(AT_Resp, sizeof(char) * (total + 1));
		memcpy(AT_Resp + offset, data, strlen(data));
		offset += strlen(data);
		free(data);
		if (strstr(AT_Resp, "OK") != NULL || strstr(AT_Resp, "ERROR") != NULL)
			break;
	}

	AT_Resp[total] = '\0';
	return AT_Resp;
}

int GSM_SendSMS(char *addr, char *msg) {
	char *resp;

	while (GSM_NetworkConnCheck() == GSM_RESP_ERROR) {
		trace_printf("Waiting for network connection to be established...\n");
	}
	GSM_SetMsgFormat();
	GSM_Cmd("AT+CSCS=\"GSM\"\r\n");
	resp = GSM_GetResponse();
	trace_printf("%s\n", resp);


	GSM_FreeCmdStr();
	sprintf(GSM_CmdStr, GSM_SEND_SMS, addr);
	GSM_Cmd(GSM_CmdStr);
	resp = GSM_GetResponse();

	trace_printf("%s\n", resp);
	delay_ms(3000);

	GSM_Cmd(msg);
	GSM_Cmd("\r\n");
	GSM_Cmd(0x1A);
	GSM_Cmd("\r\n");

	resp = GSM_GetResponse();
	trace_printf("%s\n", resp);

	return GSM_Resp_Check(resp);
}


void GSM_FreeCmdStr() {
	memcpy(GSM_CmdStr, 0, 250);
}

void GSM_SetBaudRate(uint32_t BaudRate) {
	char *resp;
	GSM_FreeCmdStr();
	sprintf(GSM_CmdStr, GSM_SET_BAUD_RATE, BaudRate);
	GSM_Cmd(GSM_CmdStr);
	resp = GSM_GetResponse();
	trace_printf("%s\n", resp);
	GSM_Init(USART1, BaudRate);
	GSM_Cmd("AT&W0\r\n");
	resp = GSM_GetResponse();
	trace_printf("%s\n", resp);
}

void GSM_DeleteInbox() {
	char *resp;

	GSM_Cmd(GSM_DELETE_INBOX);
	resp = GSM_GetResponse();
	trace_printf("%s\n", resp);
}
