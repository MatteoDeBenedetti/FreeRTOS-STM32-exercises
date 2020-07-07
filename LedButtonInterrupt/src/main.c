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


#define TRUE 1
#define FALSE 0
#define NOT_PRESSED FALSE
#define PRESSED TRUE


static void prvSetupHardware(void);
static void prvSetupUart(void);
void printmsg(char *msg);
void led_task_handler(void *params);
void button_handler();
void prvSetupGPIO();


uint8_t button_status_flag = NOT_PRESSED;


int main(void)
{
	// enable CYCCNT in DWT_CTRL for segger systemview
	DWT->CTRL |= (1 << 0);

	// reset RCC clock to HSI: sys clock = cpu clock = 16MHz
	RCC_DeInit();
	SystemCoreClockUpdate();

	// initialize hardware
	prvSetupHardware();

	// start segger systemview recording
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	// create task
	xTaskCreate(led_task_handler, "LED-task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

	// start scheduler
	vTaskStartScheduler();


	for(;;);
}


void led_task_handler(void *params)
{
	while(1)
	{
		if(button_status_flag == PRESSED)
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
			//printmsg("LED on \r \n");
		}
		else
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
			//printmsg("LED off \r \n");
		}
	}
}


void button_handler()
{
	button_status_flag ^=1; // toggle flag
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
	prvSetupGPIO();
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


// board specific setup of gpio
void prvSetupGPIO()
{
	// setup led on pin A5
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef led_init;
	led_init.GPIO_Mode = GPIO_Mode_OUT;
	led_init.GPIO_OType = GPIO_OType_PP;
	led_init.GPIO_Pin = GPIO_Pin_5;
	led_init.GPIO_Speed = GPIO_Low_Speed;
	led_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &led_init);

	// setup button on C13
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef button_init;
	button_init.GPIO_Mode = GPIO_Mode_IN;
	button_init.GPIO_OType = GPIO_OType_PP;
	button_init.GPIO_Pin = GPIO_Pin_13;
	button_init.GPIO_Speed = GPIO_Low_Speed;
	button_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &button_init);

	// setup exti line 13 interrupt for button C13
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

	EXTI_InitTypeDef exti_init;
	exti_init.EXTI_Line = EXTI_Line13;
	exti_init.EXTI_LineCmd = ENABLE;
	exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&exti_init);

	// setup nvic interrupt for exti line 13
	NVIC_SetPriority(EXTI15_10_IRQn,5);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}


void EXTI15_10_IRQHandler(void)
{
	traceISR_ENTER();

	// clear interrupt pending bit of exti line 13
	EXTI_ClearITPendingBit(EXTI_Line13);

	button_handler();

	traceISR_EXIT();
}



