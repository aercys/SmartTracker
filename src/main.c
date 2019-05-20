/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "diag/Trace.h"
#include "stm32f10x_conf.h"
#include "neo_gps.h"
#include "delay.h"
#include "usart.h"
#include "sim800.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F1 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"



int
main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.
	SysTick_Init();
	char *msgs;

	GSM_Init(USART1, GSM_BAUDRATE);

	delay_ms(20);

	if (GSM_SimStatus() == GSM_SIMPIN_REQUEST) {
		trace_printf("Requested pin code to unlock!\n");
		if (GSM_UnlockSim(1989) == GSM_RESP_ERROR) {
			trace_printf("Sim Unlock Unsuccessful!\n");
		} else {
			trace_printf("Sim Unlock Successful!\n");
		}
	} else {
		trace_printf("Sim is ready!\n");
	}

	if (GSM_NetworkConnCheck() == GSM_RESP_SUCCESS) {
		trace_printf("Connected to home network...\n");
	} else {
		trace_printf("Connection has not been established yet!\n");
		GSM_NetworkConnReg();
	}


	while(1) {

	}


}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
