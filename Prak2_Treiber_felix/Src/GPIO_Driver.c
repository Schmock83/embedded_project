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
	volatile uint32_t *tmpreg = 0x0;
	uint8_t pin = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;

	//Anschalten der Clock
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	//Konfigurieren des PIN in INPUT oder OUTPUT-Modus
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		tmpreg = &(pGPIOHandle->pGPIOx->MODE_REG);
		uint8_t modus = pGPIOHandle->GPIO_PinConfig.GPIO_PinMode;
		if(modus == GPIO_MODE_IN)
		{
			BIT_CLEAR(*tmpreg, 2 * pin);
			BIT_CLEAR(*tmpreg, 2 * pin + 1);
		}else if (modus == GPIO_MODE_OUT)
		{
			BIT_SET(*tmpreg, 2 * pin);
			BIT_CLEAR(*tmpreg, 2 * pin + 1);
		}else if (modus == GPIO_MODE_ALTFN)
		{
			BIT_CLEAR(*tmpreg, 2 * pin);
			BIT_SET(*tmpreg, 2 * pin + 1);
		}else if (modus == GPIO_MODE_ANALOG)
		{
			BIT_SET(*tmpreg, 2 * pin);
			BIT_SET(*tmpreg, 2 * pin + 1);
		}
	}

// ####################################### Praktikum 3: IRQ ############################################################

	else
	{
		// IRQ-Modus (Muss erst im dritten Praktikum bearbeitet werden)
		// 1. Konfiguration für Trigger auf fallenende, steigende oder beide Flanken
		// 2. Konfiguration des entsprechenden GPIO-Ports in SYSCFG_EXTICR
		// 3  Aktivieren des EXTI Interrupts handling in IMR-Register
	}

// ####################################### ENDE IRQ #####################################################################

	//Konfigurieren des Output-Speeds
	tmpreg = &(pGPIOHandle->pGPIOx->OUTPUT_SPEED);
	uint8_t speed = pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed;
	if(speed == GPIO_SPEED_LOW)
	{
		BIT_CLEAR(*tmpreg, 2 * pin);
		BIT_CLEAR(*tmpreg, 2 * pin + 1);
	}else if (speed == GPIO_SPEED_MEDIUM)
	{
		BIT_SET(*tmpreg, 2 * pin);
		BIT_CLEAR(*tmpreg, 2 * pin + 1);
	}else if (speed == GPIO_SPEED_HIGH)
	{
		BIT_CLEAR(*tmpreg, 2 * pin);
		BIT_SET(*tmpreg, 2 * pin + 1);
	}else if (speed == GPIO_SPEED_VERY_HIGH)
	{
		BIT_SET(*tmpreg, 2 * pin);
		BIT_SET(*tmpreg, 2 * pin + 1);
	}

	//Konfigurieren des Pull-up/down Settings
	tmpreg = &(pGPIOHandle->pGPIOx->PULL_UP_DOWN);
	uint8_t pupd = pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl;
	if(pupd == GPIO_NO_PUPD)
	{
		BIT_CLEAR(*tmpreg, 2 * pin);
		BIT_CLEAR(*tmpreg, 2 * pin + 1);
	}else if (pupd == GPIO_PIN_PU)
	{
		BIT_SET(*tmpreg, 2 * pin);
		BIT_CLEAR(*tmpreg, 2 * pin + 1);
	}else if (pupd == GPIO_PIN_PD)
	{
		BIT_CLEAR(*tmpreg, 2 * pin);
		BIT_SET(*tmpreg, 2 * pin + 1);
	}
	
	//Konfigurieren des Output modus
	tmpreg = &(pGPIOHandle->pGPIOx->OUTPUT_TYPE);
	uint8_t optype = pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType;
	if(optype == GPIO_OP_TYPE_PP)
	{
		BIT_CLEAR(*tmpreg, pin);
	}else if (optype == GPIO_OP_TYPE_OD)
	{
		BIT_SET(*tmpreg, pin);
	}
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
	return BIT_CHECK(GPIO_ReadFromInputPort(pGPIO_Handle->pGPIOx), pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	return pGPIOx->INPUT_DATA_REG;
}

void GPIO_WriteToOutputPin(GPIO_Handle_t *pGPIO_Handle, uint8_t Value)
{
	if(Value)
	{
		BIT_SET(pGPIO_Handle->pGPIOx->OUTPUT_DATA_REG, pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
	}else {
		BIT_CLEAR(pGPIO_Handle->pGPIOx->OUTPUT_DATA_REG, pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
	}
}


void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->OUTPUT_DATA_REG = Value;
}


void GPIO_ToggleOutputPin(GPIO_Handle_t *pGPIO_Handle)
{
	BIT_FLIP(pGPIO_Handle->pGPIOx->OUTPUT_DATA_REG, pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
}

// ####################################### Praktikum 3: IRQ ############################################################

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	// Hinweis: Bedenken Sie welche ISER und ICER Register des Prozessors in dem verwendeten Mikrocontroller benutzt werden können.
	// Nicht alle Interrupts sind im Mikrocontroller aktiv. Überprüfen Sie dazu das Handbuch (Reference Manual) des Mikrocontrollers.
	if(EnorDi == ENABLE)
	{
		//ToDo: Programmieren der entsprechenden ISERx register
	}
	else
	{
		//ToDo: Programmieren der entsprechenden ICERx register
	}
}

void GPIO_IRQHandling(uint8_t PinNumber)
{
	// Abfrage und Zurücksetzen des EXTI-PR bits
}

// ####################################### ENDE IRQ ###################################################################
