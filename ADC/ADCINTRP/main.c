#include <stdint.h>
#include <stm32f405xx.h>
#include <stdio.h>
#include <lcd.h>
//adc interrupt on lcd

void adc_conversion(void);
void adc_init(void);
int var;
float Vol;
char buf[30];
int main(void)
{
    /* Loop forever */
	    lcd_gpio_init();
		lcd_init();

		adc_init();
		NVIC_EnableIRQ(ADC_IRQn);
		while(1)
		{
			ADC1->CR2|=(1<<30);
			lcd(0x80,0);
			lcd_string("ADC Value:");
		    single_print(var);
			Vol = (float)(var*3) /4095;
		    sprintf(buf,"Vol:%.3f",Vol);
			lcd(0xC0,0);
			lcd_string(buf);
		}
}
void adc_init()
{
	    RCC->AHB1ENR|=(1<<2);  //Enable clock for port c
		GPIOC->MODER|=(1<<2);  //enable gpioc moder for pc-1
		GPIOC->MODER|=(1<<3);
		RCC->APB2ENR|=(1<<8); //enable clock for adc-1
		ADC1->CR2=0; //make cr2 of adc-1 as 0
		ADC1->SQR1&=~(1<<20);
		ADC1->SQR1&=~(1<<21);
		ADC1->SQR1&=~(1<<22);
		ADC1->SQR1&=~(1<<23);

		ADC1->SQR3|=(1<<0);    //make the bits 4,3,2,1,0 as B(hex) since channel-no. 11 is used with binary value 01011
		ADC1->SQR3|=(1<<1);
		ADC1->SQR3&=~(1<<2);
		ADC1->SQR3|=(1<<3);
		ADC1->SQR3&=~(1<<4);

		ADC1->CR1|=(1<<8);

		ADC1->CR1|=(1<<0);    //make the bits 4,3,2,1,0 as B(hex) since channel-no. 11 is used with binary value 01011
		ADC1->CR1|=(1<<1);
		ADC1->CR1&=~(1<<2);
		ADC1->CR1|=(1<<3);
		ADC1->CR1&=~(1<<4);

		ADC1->CR2|=(1<<0);  //single channel mode

		ADC1->CR1|=(1<<5);  //enable interrupt enable eoc bit no. 5 for adc interrupt


}
void ADC_IRQHandler(void)
{
	ADC1->CR2|=(1<<30);
	if(ADC1->SR&(1<<1)) //check for the eoc bit in adc->sr register
	{
		var=ADC1->DR;
		ADC1->SR&=~(1<<1);
	}
}
