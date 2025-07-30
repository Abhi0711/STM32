#include <stdint.h>
#include <stm32f405xx.h>
#include "lcd.h"
#include <stdio.h>

uint32_t adc_value;
float voltage;
char display_buffer[50];

void adc_setup() {
   RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;  // Enable GPIOC clock
   RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;   // Enable ADC2 clock

   GPIOC->MODER |= GPIO_MODER_MODER1;    // Set PC1 as analog mode

   ADC2->SQR3 = 11;                      // Select ADC Channel 11 (PC1)
   ADC2->CR1 |= ADC_CR1_SCAN;            // Enable scan mode
   ADC2->CR2 |= ADC_CR2_ADON;            // Power on ADC2
}

uint32_t adc_read() {
   ADC2->CR2 |= ADC_CR2_SWSTART;         // Start ADC conversion
   while (!(ADC2->SR & ADC_SR_EOC));     // Wait until conversion completes
   return ADC2->DR;                      // Return ADC result
}

int main(void) {
   adc_setup();
   lcd_gpio_init();
   lcd_init();

   while (1) {
       adc_value = adc_read();
       voltage = (adc_value * 3.0f) / 4095.0f;  // Convert ADC value to voltage

       lcd(0x80, 0);
       lcd_string("ADC2: ");
       single_print(adc_value);

       snprintf(display_buffer, sizeof(display_buffer), "Vol: %.3f", voltage);
       lcd(0xC0, 0);
       lcd_string(display_buffer);
   }
}
