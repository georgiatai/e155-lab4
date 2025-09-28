void configureTimer(TIM_TypeDef *TIMx) {
    // wait for 1000 ms
    // Enable TIM15 clock
    RCC->APB2ENR |= (1 << 16);

    // Set prescaler (80MHz / (prescaler+1)) to XX Hz
    TIMx->PSC = 15;

    TIMx->EGR |= (1 << 0); // Generate an update event to load the prescaler value to the clock

    // Counter Enable
    TIMx->CR1 |= (1 << 0);

    // Set auto-reload register to get XX Hz overflow
    // time-delay / clk-period = 1000 ms / (1/5 MHz) = 5000
    TIMx->ARR = 5000;

    // force update

    // clear UIF

    // reset count

    // Wait for update event
    while(UIF != 1);



}