#ifndef LED_PANELS_DRIVER_H
#define LED_PANELS_DRIVER_H

#include <stdint.h>
#include "led_panels_defs.h"

// void hc06_create(void);
// void hc06_destroy(void);
// hc06_status hc06_check_link(void);
// hc06_status hc06_set_baudrate(hc06_baudrate baudrate);
// hc06_baudrate hc_06_determine_baudrate(void);
// hc06_status hc06_set_name(const char* const name);
// hc06_status hc06_set_pin(const uint16_t pin);
// hc06_status hc06_write(const uint8_t *const data, const uint16_t size);
// hc06_status hc06_read(uint8_t *const data, const uint16_t size);

led_panels_buffer *led_panels_create(
  uint8_t panels_num,
  const led_panels_size *const panels_sizes
);
void led_panels_destroy(led_panels_buffer *buffer);
led_panels_status led_panels_get_pixel(
  led_panels_buffer *buffer,
  const uint8_t panel_index,
  const uint8_t pixel_x,
  const uint8_t pixel_y,
  led_panels_color *const pixel
);
led_panels_status led_panels_set_pixel(
  led_panels_buffer *buffer,
  const uint8_t panel_index,
  const uint8_t pixel_x,
  const uint8_t pixel_y,
  const led_panels_color pixel
);
void led_panels_flush(
  led_panels_buffer *buffer
);
led_panels_status led_panels_send(led_panels_buffer *buffer);

#endif
