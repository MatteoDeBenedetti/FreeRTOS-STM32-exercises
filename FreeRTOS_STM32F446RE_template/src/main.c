/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <string.h>
#include <stdio.h>
#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"


static void prvSetupHardware(void);
static void prvSetupUart(void);
void printmsg(char *msg);
void led_task_handler(void *params);


int main(void)
{
	// reset RCC clock to HSI: sys clock = cpu clock = 16MHz
	RCC_DeInit();
	SystemCoreClockUpdate();

	// initialize hardware
	prvSetupHardware();

	// create task
	xTaskCreate(led_task_handler, "LED-task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	// start scheduler
	vTaskStartScheduler();


	for(;;);
}


void led_task_handler(void *params)
{
	while(1)
	{

	}
}


// Initializa uart
static void prvSetupUart(void)
{
	// enable USART2 and GPIOA peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// set tx and rx pin alternate functions (PA2 tx, PA3 rx)
	GPIO_InitTypeDef gpio_uart_pins;
	memset(&gpio_uart_pins,0,sizeof(gpio_uart_pins)); // to init everything to 0
	gpio_uart_pins.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF;
	gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &gpio_uart_pins);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	// USART2 params initialization
	USART_InitTypeDef uart2_init;
	memset(&uart2_init,0,sizeof(uart2_init)); // to init everything to 0
	uart2_init.USART_BaudRate = 115200;
	uart2_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart2_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	uart2_init.USART_Parity = USART_Parity_No;
	uart2_init.USART_StopBits = USART_StopBits_1;
	uart2_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &uart2_init);

	// enable USART2
	USART_Cmd(USART2, ENABLE);
}


// Implement hardware initializations
static void prvSetupHardware(void)
{
	prvSetupUart();
}


void printmsg(char *msg)
{
	for (uint32_t i = 0; i < strlen(msg); i++)
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) != SET);
		USART_SendData(USART2, msg[i]);
	}
}


