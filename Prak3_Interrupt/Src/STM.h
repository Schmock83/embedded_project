/*
 * STM.h
 * Vorlage fürs Praktikum 
 */

#ifndef STM32F407XX_H_
#define STM32F407XX_H_

#include<stddef.h>
#include<stdint.h>

//Allgemeine Makros
#define ENABLE 									1
#define DISABLE 								0
#define SET 									ENABLE
#define RESET 									DISABLE
#define GPIO_PIN_SET        					SET
#define GPIO_PIN_RESET      					RESET

/*
 * Basis-Adressen der AHBx and APBx Busse
 */

#define PERIPH_BASEADDR 						0x40000000U
#define APB1PERIPH_BASEADDR						PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR						0x40010000U
#define AHB1PERIPH_BASEADDR						0x40020000U
#define AHB2PERIPH_BASEADDR						0x50000000U

/*
 * Basis-Adressen der GPIO-Ports
 */

#define GPIOA_BASEADDR							(AHB1PERIPH_BASEADDR + 0x0000U)
#define GPIOB_BASEADDR							(AHB1PERIPH_BASEADDR + 0x0400U)
#define GPIOC_BASEADDR							(AHB1PERIPH_BASEADDR + 0x0800U)
#define GPIOD_BASEADDR							(AHB1PERIPH_BASEADDR + 0x0C00U)
#define GPIOE_BASEADDR 							(AHB1PERIPH_BASEADDR + 0x1000U)
#define GPIOF_BASEADDR 							(AHB1PERIPH_BASEADDR + 0x1400U)
#define GPIOG_BASEADDR 							(AHB1PERIPH_BASEADDR + 0x1800U)
#define GPIOH_BASEADDR 							(AHB1PERIPH_BASEADDR + 0x1C00U)
#define GPIOI_BASEADDR 							(AHB1PERIPH_BASEADDR + 0x2000U)

/*
 * Basis-Adressen des Reset and Clock control Module (RCC)
 */
#define RCC_BASEADDR							0x40023800U

/********************************** Register definition structs **********************************/


/*
 * Struct mit Konfigurations und Daten-Registern der GPIO-Ports
 * Welche weiteren Register zur Konfiguration können in dem Struct eingefügt werden und was ist Ihre Bedeutung? DONE
 */
typedef struct
{
  volatile uint32_t MODE_REG;      				/* Address offset: 0x00 */
  volatile uint32_t OUTPUT_TYPE;  				/* Address offset: 0x04 */
  volatile uint32_t OUTPUT_SPEED;  				/* Address offset: 0x08 */
  volatile uint32_t PULL_UP_DOWN;  				/* Address offset: 0x0C */
  volatile uint32_t INPUT_DATA_REG;    			/* Address offset: 0x10 */
  volatile uint32_t OUTPUT_DATA_REG;			/* Address offset: 0x14 */
  volatile uint32_t BIT_SR_REG;					/* Address offset: 0x18 */
  volatile uint32_t CFG_LCK_REG;				/* Address offset: 0x1C */
  volatile uint32_t ALT_FN_REG[2];				/* Address offset: 0x20-0x24 */
}GPIO_RegDef_t;

/*
 * Konfigurationsregister des RCC
 */
typedef struct
{
  volatile uint32_t C_CTRL;     	 			/* Address offset: 0x00 */
  volatile uint32_t PLL_CFG;     	 			/* Address offset: 0x04 */
  volatile uint32_t C_CFG;     	 				/* Address offset: 0x08 */
  volatile uint32_t C_INTR;     	 			/* Address offset: 0x0C */
  volatile uint32_t AHB1_RST;     	 			/* Address offset: 0x10 */
  volatile uint32_t AHB2_RST;  	 				/* Address offset: 0x14 */
  volatile uint32_t AHB3_RST;     				/* Address offset: 0x18 */
  uint32_t      RESERVED0;     	 				/* Address offset: 0x1C */
  volatile uint32_t APB1_RST;     				/* Address offset: 0x20 */
  volatile uint32_t APB2_RST;     				/* Address offset: 0x24 */
  uint32_t      RESERVED1[2];     	 			/* Address offset: 0x28-0x2C */
  volatile uint32_t AHB1_EN;     				/* Address offset: 0x30 */
  volatile uint32_t AHB2_EN;     				/* Address offset: 0x34 */
  volatile uint32_t AHB3_EN;     				/* Address offset: 0x38 */
  uint32_t      RESERVED2; 	    	 			/* Address offset: 0x3C */
  volatile uint32_t APB1_EN;     				/* Address offset: 0x40 */
  volatile uint32_t APB2_EN;     				/* Address offset: 0x44 */
  uint32_t      RESERVED3[2];     	 			/* Address offset: 0x48-0x4C */
  volatile uint32_t AHB1_LP_EN;     			/* Address offset: 0x50 */
  volatile uint32_t AHB2_LP_EN;     			/* Address offset: 0x54 */
  volatile uint32_t AHB3_LP_EN;     			/* Address offset: 0x58 */
  uint32_t      RESERVED4; 	    	 			/* Address offset: 0x5C */
  volatile uint32_t APB1_LP_EN;     			/* Address offset: 0x60 */
  volatile uint32_t APB2_LP_EN;     			/* Address offset: 0x64 */
  uint32_t      RESERVED5[2];     	 			/* Address offset: 0x68-0x6C */
  volatile uint32_t BCK_DOM_CTRL;     			/* Address offset: 0x70 */
  volatile uint32_t C_CTRL_STS;     			/* Address offset: 0x74 */
  uint32_t      RESERVED6[2];     	 			/* Address offset: 0x78-0x7C */
  volatile uint32_t SSCG;     					/* Address offset: 0x80 */
  volatile uint32_t PLL_I2S_CFG; 				/* Address offset: 0x84 */
  volatile uint32_t PLL_SAI_CFG;     			/* Address offset: 0x88 */
  volatile uint32_t DCK_CFG;     				/* Address offset: 0x8C */
} RCC_RegDef_t;


/********************************** Macros to access peripherals **********************************/

/*
 * Basisadressen gecastet auf entsprechende structs
 */

#define GPIOA  									((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB  									((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC  									((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD  									((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE  									((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF  									((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG  									((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH  									((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI  									((GPIO_RegDef_t*)GPIOI_BASEADDR)
#define RCC 									((RCC_RegDef_t*)RCC_BASEADDR)


/*
 * Clock Enable Makros für GPIO-Ports
 */		
#define GPIOA_PCLK_EN()							(RCC->AHB1_EN |= (1 << GPIO_BASEADDR_TO_CODE(GPIOA)))
#define GPIOB_PCLK_EN()							(RCC->AHB1_EN |= (1 << GPIO_BASEADDR_TO_CODE(GPIOB)))
#define GPIOC_PCLK_EN()							(RCC->AHB1_EN |= (1 << GPIO_BASEADDR_TO_CODE(GPIOC)))
#define GPIOD_PCLK_EN()							(RCC->AHB1_EN |= (1 << GPIO_BASEADDR_TO_CODE(GPIOD)))
#define GPIOE_PCLK_EN()							(RCC->AHB1_EN |= (1 << GPIO_BASEADDR_TO_CODE(GPIOE)))
#define GPIOF_PCLK_EN()							(RCC->AHB1_EN |= (1 << GPIO_BASEADDR_TO_CODE(GPIOF)))
#define GPIOG_PCLK_EN()							(RCC->AHB1_EN |= (1 << GPIO_BASEADDR_TO_CODE(GPIOG)))
#define GPIOH_PCLK_EN()							(RCC->AHB1_EN |= (1 << GPIO_BASEADDR_TO_CODE(GPIOH)))
#define GPIOI_PCLK_EN()							(RCC->AHB1_EN |= (1 << GPIO_BASEADDR_TO_CODE(GPIOI)))

#define GPIOA_PCLK_DI()							(RCC->AHB1_EN &= ~(1 << GPIO_BASEADDR_TO_CODE(GPIOA)))
#define GPIOB_PCLK_DI()							(RCC->AHB1_EN &= ~(1 << GPIO_BASEADDR_TO_CODE(GPIOB)))
#define GPIOC_PCLK_DI()							(RCC->AHB1_EN &= ~(1 << GPIO_BASEADDR_TO_CODE(GPIOC)))
#define GPIOD_PCLK_DI()							(RCC->AHB1_EN &= ~(1 << GPIO_BASEADDR_TO_CODE(GPIOD)))
#define GPIOE_PCLK_DI()							(RCC->AHB1_EN &= ~(1 << GPIO_BASEADDR_TO_CODE(GPIOE)))
#define GPIOF_PCLK_DI()							(RCC->AHB1_EN &= ~(1 << GPIO_BASEADDR_TO_CODE(GPIOF)))
#define GPIOG_PCLK_DI()							(RCC->AHB1_EN &= ~(1 << GPIO_BASEADDR_TO_CODE(GPIOG)))
#define GPIOH_PCLK_DI()							(RCC->AHB1_EN &= ~(1 << GPIO_BASEADDR_TO_CODE(GPIOH)))
#define GPIOI_PCLK_DI()							(RCC->AHB1_EN &= ~(1 << GPIO_BASEADDR_TO_CODE(GPIOI)))

/*
 *  Makros um GPIO-Ports zurückzusetzen
 */
#define GPIOA_REG_RESET()               do{ (RCC->AHB1_RST |= (1 << 0)); (RCC->AHB1_RST &= ~(1 << 0)); }while(0)
#define GPIOB_REG_RESET()               do{ (RCC->AHB1_RST |= (1 << 1)); (RCC->AHB1_RST &= ~(1 << 1)); }while(0)
#define GPIOC_REG_RESET()               do{ (RCC->AHB1_RST |= (1 << 2)); (RCC->AHB1_RST &= ~(1 << 2)); }while(0)
#define GPIOD_REG_RESET()               do{ (RCC->AHB1_RST |= (1 << 3)); (RCC->AHB1_RST &= ~(1 << 3)); }while(0)
#define GPIOE_REG_RESET()               do{ (RCC->AHB1_RST |= (1 << 4)); (RCC->AHB1_RST &= ~(1 << 4)); }while(0)
#define GPIOF_REG_RESET()               do{ (RCC->AHB1_RST |= (1 << 5)); (RCC->AHB1_RST &= ~(1 << 5)); }while(0)
#define GPIOG_REG_RESET()               do{ (RCC->AHB1_RST |= (1 << 6)); (RCC->AHB1_RST &= ~(1 << 6)); }while(0)
#define GPIOH_REG_RESET()               do{ (RCC->AHB1_RST |= (1 << 7)); (RCC->AHB1_RST &= ~(1 << 7)); }while(0)
#define GPIOI_REG_RESET()               do{ (RCC->AHB1_RST |= (1 << 8)); (RCC->AHB1_RST &= ~(1 << 8)); }while(0)

// ####################################### Teil IRQ ############################################################
/*
 * ARM Cortex Mx Processor NVIC register Addresses
 */

#define NVIC_ISER0          					0xE000E100U
#define NVIC_ISER1          					(NVIC_ISER0 + 0x04)
#define NVIC_ISER2          					(NVIC_ISER0 + 0x08)
#define NVIC_ISER3          					(NVIC_ISER0 + 0x0C)

#define NVIC_ICER0          					(NVIC_ISER0 + 0x80)
#define NVIC_ICER1          					(NVIC_ICER0 + 0x04)
#define NVIC_ICER2          					(NVIC_ICER0 + 0x08)
#define NVIC_ICER3          					(NVIC_ICER0 + 0x0C)

/*
 * IRQ(Interrupt Request) Nummern für STM32F407x MCU
 */

#define IRQ_NO_EXTI0 							6
#define IRQ_NO_EXTI1 							7
#define IRQ_NO_EXTI2 							8
#define IRQ_NO_EXTI3 							9
#define IRQ_NO_EXTI4 							10
#define IRQ_NO_EXTI9_5 							23
#define IRQ_NO_EXTI15_10 						40

// ####################################### ENDE IRQ ############################################################

/*
 * EXTI Adressen und Makros
 */
#define EXTI_BASEADDR							(APB2PERIPH_BASEADDR + 0x3C00)
#define EXTI									((EXTI_RegDef_t*)EXTI_BASEADDR)
/*
 * Sysconfig Adressen und Makros
 */
#define SYSCFG_BASEADDR							(APB2PERIPH_BASEADDR + 0x3800)
#define SYSCFG									((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)
/*
 * Clock Enable und Disable Makros für SYSCFG
 */
#define SYSCFG_PCLK_EN()						(RCC->APB2_EN |= (1 << 14))
#define SYSCFG_PCLK_DI()						(RCC->APB2_EN &= ~(1 << 14))

/*
 * Register struct EXTI
 */
typedef struct
{
  volatile uint32_t IMR;    		 			/*!< Interrupt mask register				Address offset: 0x00 */
  volatile uint32_t EMR;    		 			/*!< Event mask register					Address offset: 0x04 */
  volatile uint32_t RTSR;    		 			/*!< Rising trigger selection register		Address offset: 0x08 */
  volatile uint32_t FTSR;    		 			/*!< Falling trigger selection register		Address offset: 0x0C */
  volatile uint32_t SWIER;    		 			/*!< Software interrupt event register		Address offset: 0x10 */
  volatile uint32_t PR;    		 				/*!< Pending register						Address offset: 0x14 */
}EXTI_RegDef_t;

/*
 * peripheral register definition structure for SYSCFG
 */
typedef struct
{
  volatile uint32_t MEMRMP;        				/*!< 										Address offset: 0x00		*/
  volatile uint32_t PMC;           				/*!<										Address offset: 0x04		*/
  volatile uint32_t EXTICR[4];     				/*!<									 	Address offset: 0x08-0x14	*/
  uint32_t      	  RESERVED1[2]; 			/*!<										Reserved, 0x18-0x1C			*/
  volatile uint32_t CMPCR;         				/*!<										Address offset: 0x20		*/
  uint32_t      	  RESERVED2[2]; 			/*!<										Reserved, 0x24-0x28 		*/
  volatile uint32_t CFGR;          				/*!< not defined in manual(?)				Address offset: 0x2C		*/
} SYSCFG_RegDef_t;


/*
 * Makro zum Zurückgeben eines Codes (between 0 to 7) für den entsprechenden GPIO-PORT (x)
 */
#define GPIO_BASEADDR_TO_CODE(x)      ( (x == GPIOA)?0:\
										(x == GPIOB)?1:\
										(x == GPIOC)?2:\
										(x == GPIOD)?3:\
								        (x == GPIOE)?4:\
								        (x == GPIOF)?5:\
								        (x == GPIOG)?6:\
								        (x == GPIOH)?7: \
								        (x == GPIOI)?8:0)



#include "GPIO_Driver.h"

#endif /* STM32F407XX_H_ */
