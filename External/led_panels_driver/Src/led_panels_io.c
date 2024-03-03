#include "led_panels_io.h"

// #include "hc06_io.h"
#include "stm32f1xx.h"
// #include "stm32f1xx_hal_uart.h"

extern TIM_HandleTypeDef *led_panels_tim;

// hc06_status hc06_io_read(
//   uint8_t *const data,
//   const uint16_t data_size
// )
// {
//   return (hc06_status)HAL_UART_Receive(
//     hc06_uart, data, data_size, HC06_TIMEOUT
//   );
// }

// hc06_status hc06_io_write(
//   const uint8_t *const data,
//   const uint16_t data_size
// )
// {
//   return (hc06_status)HAL_UART_Transmit(
//     hc06_uart, data, data_size, HC06_TIMEOUT
//   );
// }

// hc06_status hc06_io_set_baudrate(const uint32_t baudrate)
// {
//   HAL_UART_Abort_IT(hc06_uart);
//   HAL_UART_DeInit(hc06_uart);
//   hc06_uart->Init.BaudRate = baudrate;

//   hc06_status status = (hc06_status)HAL_UART_Init(hc06_uart);
//   return status;
// }

led_panels_status led_panels_io_send_data(
  const uint8_t *const data,
  const uint16_t data_size
)
{
  return (led_panels_status)HAL_TIM_PWM_Start_DMA(
    led_panels_tim,
    TIM_CHANNEL_1,
    (uint32_t *)data,
    data_size
  );
}
