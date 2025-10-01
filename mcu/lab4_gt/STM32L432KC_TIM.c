// STM32L432KC_TIM.c
// Source code for TIM functions

#include "STM32L432KC_TIM.h"
#include <stdio.h>

void initTIM(TIMx_TypeDef *TIMx, uint32_t psc_val) {
    // Set prescaler value
    TIMx->PSC = psc_val;

    //// Disable slave mode (bit 16, 2:0 = 0000)
    //TIMx->SMCR &= ~(0b111 << 0);
    //TIMx->SMCR &= ~(1 << 16);

    // clear UIF
    TIMx->EGR  &= ~(1 << 0);
    // Generate an update event to load the prescaler value to the clock
    TIMx->EGR |= (1 << 0); 

    // Counter Enable
    TIMx->CR1 |= (1 << 0);

}

void delay_millis(TIMx_TypeDef * TIMx, uint32_t ms, uint32_t psc_val) {
    // Set auto-reload register
    // wait_time / 1000 * freq = arr_val
    int arr_val = ms * (80000 / (psc_val+1));
    TIMx->ARR = arr_val;

    // clear UIF
    TIMx->EGR  &= ~(1 << 0);

    // force update
    TIMx->EGR |=  (1 << 0);

    // clear UIF
    TIMx->SR  &= ~(1 << 0);

    // reset count
    TIMx->CNT = 0;
  
    // Wait for update event (SR bit 0)
    while(((TIMx->SR) & 1U) != 1);
}

void initPWM(TIMx_TypeDef *TIMx, uint32_t psc_val) {
    // Set prescaler value
    TIMx->PSC = psc_val;

    // Set output 1 polarity to active high
    TIMx->CCER |= (1 << 3);

    // Enable output on channel 1
    TIMx->CCER |= (1 << 0);

    // Set mode to PWM mode 1 (110) on channel 1
    TIMx->CCMR1 &= ~(0b111 << 4);
    TIMx->CCMR1 |=  (0b110 << 4);

    // Enable preload for CCR1 later for dutying cycle updates
    TIMx->CCMR1 |= (1 << 3);

    // Enable main output
    TIMx->BDTR |= (1 << 15);

    // force update
    TIMx->EGR |= (1 << 0);

    // Enable counter
    TIMx->CR1 = (1 << 0);

}

void PWM_setDutyCycle(TIMx_TypeDef *TIMx, uint32_t note_freq, uint32_t duty_cycle, uint32_t psc_val) {
    // Set auto-reload register
    // clock_freq = 80 MHz / (psc_val + 1)
    // arr_val = clock_freq / note_freq
    int arr_val = (note_freq > 0) ? ((80000000 / (psc_val+1)) / note_freq - 1) : 0;
    TIMx->ARR = arr_val;

    TIMx->CCR1 = arr_val * duty_cycle / 100;

    // force update
    TIMx->EGR |= (1 << 0);

    // reset count
    TIMx->CNT = 0;

}