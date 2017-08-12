#include "pwm.h"

TIM_HandleTypeDef htim1;


HAL_StatusTypeDef pwm_init(void)
{
	HAL_StatusTypeDef state;
	GPIO_InitTypeDef gpio_init;
	TIM_OC_InitTypeDef oc_init;
	
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	gpio_init.Pin = GPIO_PIN_9 | GPIO_PIN_10;
	gpio_init.Mode = GPIO_MODE_AF_PP;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init.Alternate = GPIO_AF1_TIM1;
	
	HAL_GPIO_Init(GPIOA, &gpio_init);
	
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = SystemCoreClock / PWM_CLOCK - 1;
	htim1.Init.Period = PWM_Period-1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	
	state=HAL_TIM_PWM_Init(&htim1);
	
	oc_init.OCMode = TIM_OCMODE_PWM1;
	oc_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	oc_init.OCNPolarity = TIM_OCNPOLARITY_LOW;
	oc_init.OCFastMode = TIM_OCFAST_DISABLE;
	oc_init.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	oc_init.OCIdleState = TIM_OCIDLESTATE_RESET;
	oc_init.Pulse = 0;
	
	HAL_TIM_PWM_ConfigChannel(&htim1, &oc_init, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&htim1, &oc_init, TIM_CHANNEL_3);
	//HAL_TIM_PWM_ConfigChannel(&htim1, &oc_init, TIM_CHANNEL_1);
	set_pwm_val(TIM_CHANNEL_2, 6000-200);
	set_pwm_val(TIM_CHANNEL_3, 6000-200);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	
	
	return state;
}

void set_pwm_val(uint32_t channel, uint32_t val)
{
	__HAL_TIM_SetCompare(&htim1, channel, (val > PWM_Period ? PWM_Period : val));
}

void pwm_out(uint32_t channel, int32_t val)
{
	int32_t ref = 6000, out_val;
	if (channel == TIM_CHANNEL_2)
		ref = 6000+200;
	else if (channel == TIM_CHANNEL_3)
		ref = 6000-300;

	if (val>0)
	{
		val += 10;
	}
	if (val < 0)
	{
		val -= 10;
	}
	out_val = ref + val;
	if (out_val > 9000)
		out_val = 9000;
	else if (out_val < 3000)
		out_val = 3000;

	set_pwm_val(channel, out_val);
}
