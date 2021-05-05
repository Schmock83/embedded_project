/**
 * Gruppe 4
 * Felix Baumann fb7632s
 * Benedikt Happe bh8332s
 */

#include <stdint.h>
#include <stddef.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


#define RCC_ADDR 0x40023800U
#define AHB1_ADDR_OFFSET 0x30U
#define GPIOD_ADDR 0x40020C00U
#define GPIO_MODER_ADDR_OFFSET 0x0
#define GPIO_ODR_ADDR_OFFSET 0x14

#define TRUE 1

#define SHIFT_BIT(POS) (1 << POS)

#define GPIO_PIN_D 3
#define BLUE_LED_PIN 15
#define RED_LED_PIN 14

void delay(void)
{
	for(volatile uint32_t i=0; i < 500000; i++);
}

void setupClock(uint32_t GPIO_PIN) {
	//AHB1 (Clock fuer GPIOD) enablen in RCC
	*((volatile uint32_t*) (RCC_ADDR + AHB1_ADDR_OFFSET)) |= SHIFT_BIT(GPIO_PIN);
}

void setup_GPIO_OutputMode(uint32_t PIN) {

	//Output-Mode fuer PIN setzten ( 2*PIN auf 1, 2*PIN+1 auf 0) -> z.B. PIN 15 (30 auf 1, 29 auf 0)
	*((volatile uint32_t*) (GPIOD_ADDR + GPIO_MODER_ADDR_OFFSET)) |= SHIFT_BIT( (2 * PIN) ); //BIT (2*PIN) auf 1
	*((volatile uint32_t*) (GPIOD_ADDR + GPIO_MODER_ADDR_OFFSET)) &= ~SHIFT_BIT( ((2 * PIN)+1) ); //BIT (2*PIN+1) auf 0

}

void toggleLED(uint32_t LED_PIN) {
	//toggle LED
	*((volatile uint32_t*) (GPIOD_ADDR + GPIO_ODR_ADDR_OFFSET)) ^= SHIFT_BIT(LED_PIN);
}

int main(void)
{
	setupClock(GPIO_PIN_D);

	setup_GPIO_OutputMode(BLUE_LED_PIN);
	setup_GPIO_OutputMode(RED_LED_PIN);

	//rote LED togglen, damit rote und blaue nicht gleich leuchten
	toggleLED(RED_LED_PIN);

	while(TRUE){
		toggleLED(BLUE_LED_PIN);
		toggleLED(RED_LED_PIN);
		delay();
	}
}
