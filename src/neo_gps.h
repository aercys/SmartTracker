/*
 * neo_gps.h
 *
 *  Created on: Feb 9, 2019
 *      Author: atakane
 */

#ifndef NEO_GPS_H_
#define NEO_GPS_H_


#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x_conf.h"

typedef struct GPS_Info {
	double 	latitude;
	double 	longitude;
	float	course;
	float	magneticVariation;
	int		timeOfFix;
	int		dateOfFix;
	int		speedOverGround;
	uint8_t	valid;
} GPS_Info;

void GPS_Init();
uint8_t USART_Receive(void);
int explode(char ***dest, char *src, char delim);
char *get_gps_sentence(void);


#endif /* NEO_GPS_H_ */
