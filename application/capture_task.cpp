//
// Created by szy on 2021/12/19.
//
#include "cmsis_os.h"
#include "capture_task.h"
#include "tim.h"
#include "usart.h"

uint32_t risingValue;
uint32_t fallingValue;
float duty;
float period_ms;
uint8_t rx_buff;
uint8_t rx_state;
uint8_t tx_state;
/**
 * @brief a PWM capture task.Use PWM capture mode in htim1 channel1.
 * @param argument
 */
void StartPwmCapture(void const * argument)
{
    HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET);
    HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_2);
    HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_RESET);

    uint8_t begin_buff[]={0xff,0xff,0xff};             //as a begin data
    HAL_UART_Transmit_DMA(&huart1,begin_buff,3);
    HAL_UART_Receive_IT(&huart1,&rx_buff,1);
    while(1)
    {
        rx_state = huart1.RxState;
        tx_state = huart1.gState;                   // this is for debug the uart function

        osDelay(100);
    }
}
/**
 * @brief the TIM1 's PWM calculation callback function
 * @param htim
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim1)
    {
        HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin,GPIO_PIN_SET);
        //HAL_GPIO_TogglePin(LED_B_GPIO_Port,LED_B_Pin);
        risingValue = HAL_TIM_ReadCapturedValue(&htim1,TIM_CHANNEL_1);
        fallingValue = HAL_TIM_ReadCapturedValue(&htim1,TIM_CHANNEL_2);
        if(risingValue != 0)
        {
            period_ms = risingValue * 0.001;
            duty = fallingValue * 1.0 / risingValue;
        }
    }
}
/**
 * @brief UART1 's receive callback function
 * @param huart
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart1)
    {
        HAL_GPIO_TogglePin(LED_G_GPIO_Port,LED_G_Pin);
        HAL_UART_Receive_IT(&huart1,&rx_buff,1);
    }
}
