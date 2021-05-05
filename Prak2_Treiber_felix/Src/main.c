/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Felix Baumann
 * @brief          : Praktikum 3
 ******************************************************************************
 */

#include <stdint.h>
#include <string.h> //memset
#include "GPIO_Driver.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


void delay(void) {
	for (volatile uint32_t i = 0; i < 3400000; i++); //grob 3 Sekunden für Aufgabe 2.2
}

void delay5(void) {
	for (volatile uint32_t i = 0; i < 5700000; i++); //grob 5 Sekunden für Aufgabe 2.4
}


int main(void)
{
	/*
	//Aufgabe 2.2
	//blaue LED leuchten lassen
	GPIO_Handle_t gpio_blue_led;
	memset(&gpio_blue_led, 0, sizeof(gpio_blue_led));

	//LED-Konfiguration
	gpio_blue_led.pGPIOx = GPIOD;
	gpio_blue_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	gpio_blue_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpio_blue_led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	gpio_blue_led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	gpio_blue_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&gpio_blue_led);

	GPIO_WriteToOutputPin(&gpio_blue_led, GPIO_PIN_RESET);

    // Loop forever
	for(;;)
	{
		GPIO_ToggleOutputPin(&gpio_blue_led);
		delay();
	}*/


	/*
	//Lauflicht leuchten lassen
	GPIO_Handle_t gpio_leds[4];
	uint8_t RED=0, YELLOW=1, GREEN=2, BLUE=3;
	memset(gpio_leds, 0, 4 * sizeof(*gpio_leds));

	//LED-Konfiguration
	gpio_leds[RED].pGPIOx = GPIOD;
	gpio_leds[RED].GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	gpio_leds[RED].GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpio_leds[RED].GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	gpio_leds[RED].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	gpio_leds[RED].GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&(gpio_leds[RED]));

	gpio_leds[YELLOW].pGPIOx = GPIOD;
	gpio_leds[YELLOW].GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	gpio_leds[YELLOW].GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpio_leds[YELLOW].GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	gpio_leds[YELLOW].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	gpio_leds[YELLOW].GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&(gpio_leds[YELLOW]));

	gpio_leds[GREEN].pGPIOx = GPIOD;
	gpio_leds[GREEN].GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	gpio_leds[GREEN].GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpio_leds[GREEN].GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	gpio_leds[GREEN].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	gpio_leds[GREEN].GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&(gpio_leds[GREEN]));

	gpio_leds[BLUE].pGPIOx = GPIOD;
	gpio_leds[BLUE].GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	gpio_leds[BLUE].GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpio_leds[BLUE].GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	gpio_leds[BLUE].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	gpio_leds[BLUE].GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&(gpio_leds[BLUE]));

	GPIO_WriteToOutputPin(&(gpio_leds[RED]), GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(&(gpio_leds[YELLOW]), GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(&(gpio_leds[GREEN]), GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(&(gpio_leds[BLUE]), GPIO_PIN_RESET);

	// Loop forever
	for(;;)
	{
		GPIO_ToggleOutputPin(&(gpio_leds[RED]));
		delay();
		GPIO_ToggleOutputPin(&(gpio_leds[RED]));
		GPIO_ToggleOutputPin(&(gpio_leds[YELLOW]));
		delay();
		GPIO_ToggleOutputPin(&(gpio_leds[YELLOW]));
		GPIO_ToggleOutputPin(&(gpio_leds[GREEN]));
		delay();
		GPIO_ToggleOutputPin(&(gpio_leds[GREEN]));
		GPIO_ToggleOutputPin(&(gpio_leds[BLUE]));
		delay();
		GPIO_ToggleOutputPin(&(gpio_leds[BLUE]));
	}*/

	//Aufgabe 2.4
	//Ampelschaltung
		GPIO_Handle_t gpio_leds[3];
		uint8_t RED=0, YELLOW=1, GREEN=2;
		memset(gpio_leds, 0, 3 * sizeof(*gpio_leds));

		//LED-Konfiguration
		gpio_leds[RED].pGPIOx = GPIOD;
		gpio_leds[RED].GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
		gpio_leds[RED].GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
		gpio_leds[RED].GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
		gpio_leds[RED].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
		gpio_leds[RED].GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
		GPIO_Init(&(gpio_leds[RED]));

		gpio_leds[YELLOW].pGPIOx = GPIOD;
		gpio_leds[YELLOW].GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
		gpio_leds[YELLOW].GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
		gpio_leds[YELLOW].GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
		gpio_leds[YELLOW].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
		gpio_leds[YELLOW].GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
		GPIO_Init(&(gpio_leds[YELLOW]));

		gpio_leds[GREEN].pGPIOx = GPIOD;
		gpio_leds[GREEN].GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
		gpio_leds[GREEN].GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
		gpio_leds[GREEN].GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
		gpio_leds[GREEN].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
		gpio_leds[GREEN].GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
		GPIO_Init(&(gpio_leds[GREEN]));

		GPIO_WriteToOutputPin(&(gpio_leds[RED]), GPIO_PIN_RESET);
		GPIO_WriteToOutputPin(&(gpio_leds[YELLOW]), GPIO_PIN_RESET);
		GPIO_WriteToOutputPin(&(gpio_leds[GREEN]), GPIO_PIN_RESET);

		// Loop forever
		for(;;)
		{
			GPIO_ToggleOutputPin(&(gpio_leds[RED])); //rot an
			delay5();
			GPIO_ToggleOutputPin(&(gpio_leds[YELLOW])); //gelb an
			delay5();
			GPIO_ToggleOutputPin(&(gpio_leds[RED])); //rot aus
			GPIO_ToggleOutputPin(&(gpio_leds[YELLOW])); //gelb aus
			GPIO_ToggleOutputPin(&(gpio_leds[GREEN])); //grün an
			delay5();
			GPIO_ToggleOutputPin(&(gpio_leds[YELLOW])); //gelb an
			GPIO_ToggleOutputPin(&(gpio_leds[GREEN])); //grün aus
			delay5();
			GPIO_ToggleOutputPin(&(gpio_leds[YELLOW])); //gelb aus
		}
}
