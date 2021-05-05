/*
 * GPIO_Driver.c
 * Vorlage fürs Praktikum 
 */

#include "GPIO_Driver.h"

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}else if (pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}else if (pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}else if (pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}else if (pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}else if (pGPIOx == GPIOI)
		{
			GPIOI_PCLK_EN();
		}
	}
	else
	{
		// Implementieren Sie die Funktionalität der Clock Deaktivierung
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}else if (pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}else if (pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}else if (pGPIOx == GPIOG)
		{
			GPIOG_PCLK_DI();
		}else if (pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DI();
		}else if (pGPIOx == GPIOI)
		{
			GPIOI_PCLK_DI();
		}
	}
}

// Implementieren Sie die weiteren Schritte. Denken Sie daran die Einträge eines Registers zurückzusetzen, bevor Sie einzelne Bits setzen!
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	 //Anschalten der Clock
	 GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	 //Konfigurieren des PIN in INPUT oder OUTPUT-Modus
	 if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	 {
		// Implementieren Sie die Funktionalität
		//pGPIOHandle->pGPIOx->MODE_REG = RESET;
		pGPIOHandle->pGPIOx->MODE_REG |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
		
	 }
	 
	 // ####################################### Praktikum 3: IRQ ############################################################
	 
	 else
	 { 
	    // IRQ-Modus (Muss erst im dritten Praktikum bearbeitet werden)
		// 1. Konfiguration für Trigger auf fallenende, steigende oder beide Flanken
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT){
			EXTI->RTSR &= ~(ENABLE << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR |= (ENABLE << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT){
			EXTI->FTSR &= ~(ENABLE << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR |= (ENABLE << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT){
			EXTI->RTSR |= (ENABLE << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR |= (ENABLE << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//2. Konfiguration des entsprechenden GPIO-Ports in SYSCFG_EXTICR
		SYSCFG_PCLK_EN();
		uint16_t extiCR_Number = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4);
		SYSCFG->EXTICR[extiCR_Number] |= (GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx) << ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%4)*4));

		//3  Aktivieren des EXTI Interrupts handling in IMR-Register
		EXTI->IMR |= (SET << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	 }
	 
	// ####################################### ENDE IRQ #####################################################################
	
	 //Konfigurieren des Output-Speeds
	 //pGPIOHandle->pGPIOx->OUTPUT_SPEED = RESET;
	 pGPIOHandle->pGPIOx->OUTPUT_SPEED |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	 
	 //Konfigurieren des Pull-up/down Settings
	 //pGPIOHandle->pGPIOx->PULL_UP_DOWN = RESET;
	 pGPIOHandle->pGPIOx->PULL_UP_DOWN |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	 
	 //Konfigurieren des Output modus
	 //pGPIOHandle->pGPIOx->OUTPUT_TYPE = RESET;
	 pGPIOHandle->pGPIOx->OUTPUT_TYPE |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	 
}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}else if (pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}else if (pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}else if (pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}else if (pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}else if (pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}else if (pGPIOx == GPIOG)
	{
		GPIOG_REG_RESET();
	}else if (pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}else if (pGPIOx == GPIOI)
	{
		GPIOI_REG_RESET();
	}
}

uint8_t GPIO_ReadFromInputPin(GPIO_Handle_t *pGPIO_Handle)
{
	// Implementieren Sie die Funktionalität
	if((pGPIO_Handle->pGPIOx->INPUT_DATA_REG & pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber) != RESET){
		return (uint8_t) SET;
	}else{
		return (uint8_t) RESET;
	}
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	// Implementieren Sie die Funktionalität
	return (uint16_t)pGPIOx->INPUT_DATA_REG;
}

void GPIO_WriteToOutputPin(GPIO_Handle_t *pGPIO_Handle, uint8_t Value)
{
	// Implementieren Sie die Funktionalität

	if(Value == SET){
		//BSSR benutzen um atomares bit zu setzen
		pGPIO_Handle->pGPIOx->BIT_SET_RESET_REG = (SET << pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
	}else{
		//BSSR benutzen um atomares bit zu resetten (pinNumber + 16bits)
		pGPIO_Handle->pGPIOx->BIT_SET_RESET_REG = (SET << (pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber + 16));
	}
}


void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	// Implementieren Sie die Funktionalität
	pGPIOx->OUTPUT_DATA_REG = Value;
}


void GPIO_ToggleOutputPin(GPIO_Handle_t *pGPIO_Handle)
{
	// Implementieren Sie die Funktionalität
	pGPIO_Handle->pGPIOx->OUTPUT_DATA_REG ^= (SET << pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
}

// ####################################### Praktikum 3: IRQ ############################################################

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	// Hinweis: Bedenken Sie welche ISER und ICER Register des Prozessors in dem verwendeten Mikrocontroller benutzt werden können.
	// Nicht alle Interrupts sind im Mikrocontroller aktiv. Überprüfen sie dazu das Handbuch (Reference Manual) des Mikrocontrollers.
	if(EnorDi == ENABLE)
	{
		//ToDo: Programmieren der entsprechenden ISERx register
		if(IRQNumber >= IRQ_NO_EXTI0 && IRQNumber <= IRQ_NO_EXTI9_5){
			*((volatile uint32_t*) NVIC_ISER0) |= (1 << IRQNumber);
		}else if(IRQNumber == IRQ_NO_EXTI15_10){
			*((volatile uint32_t*) NVIC_ISER1) |= (1 << (IRQNumber%32));
		}
	}
	else
	{
		//ToDo: Programmieren der entsprechenden ICERx register
		if(IRQNumber >= IRQ_NO_EXTI1 && IRQNumber <= IRQ_NO_EXTI9_5){
			*((volatile uint32_t*) NVIC_ICER0) |= (1 << IRQNumber);
		}else if(IRQNumber == IRQ_NO_EXTI15_10){
			*((volatile uint32_t*) NVIC_ICER1) |= (1 << (IRQNumber%32));
		}
	}
}

void GPIO_IRQHandling(uint8_t PinNumber)
{
	// Abfrage und zurücksetzen des EXTI-PR bits
	if((EXTI->PR) & (ENABLE << PinNumber) != RESET){
		EXTI->PR |= ( 1 << PinNumber);
	}
}

// ####################################### ENDE IRQ ###################################################################
