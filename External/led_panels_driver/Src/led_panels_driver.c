#include "led_panels_driver.h"
#include "led_panels_defs.h"
#include "led_panels_io.h"
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

// #include "hc06_driver.h"
// #include "hc06_defs.h"
// #include "hc06_io.h"
// #include "hc06_time.h"
// #include <stdint.h>
// #include <string.h>
// #include <stdio.h>

// Defines -------------------------------------------------------------------

// Static variables ----------------------------------------------------------

// static char buffer[HC06_BUFFER_SIZE];
// static char *baudrate_cmd = "AT+UART=%u,0,0\r\n";
// static char *change_name_cmd = "AT+NAME=%s\r\n";
// static char *change_pin_cmd = "AT+PSWD=\"%04u\"\r\n";
// static char *hex_symbols = "123456789A";
// static const uint32_t baudrate_int[] = {
//   1200, 2400, 4800, 9600, 19200, 38400, 
//   57600, 115200, 230400, 460800
// };
// static hc06_baudrate current_baudrate;

// // Static functions ----------------------------------------------------------

// static void set_check_cmd()
// {
//   strcpy(buffer, "AT\r\n");
// }

// static void set_baudrate_cmd(hc06_baudrate baudrate)
// {
//   sprintf(buffer, baudrate_cmd, GET_BAUDRATE_INT(baudrate));
// }

// static void set_change_name_cmd(const char *const name)
// {
//   sprintf(buffer, change_name_cmd, name);
// }

// static void set_change_pin_cmd(const uint16_t pin)
// {
//   sprintf(buffer, change_pin_cmd, pin);
// }

// static hc06_status send_at_cmd()
// {
//   hc06_io_write((uint8_t*)buffer, strlen(buffer));
//   hc06_io_read((uint8_t*)buffer, 4);

//   if (strstr(buffer, "OK") == NULL)
//     return HC06_ERROR;

//   hc06_delay(HC06_AT_DELAY);

//   return HC06_OK;
// }

static void led_panels_convert_color_to_pwm_data(
  uint8_t *const pwm_data,
  const uint8_t *const color
)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    *(pwm_data + i) = (bool)(*color & (1 << (7 - i))) ? 
      LED_PANELS_1_VALUE : LED_PANELS_0_VALUE;
  }
}

static void led_panels_convert_pwm_data_to_color(
  const uint8_t *const pwm_data,
  uint8_t *const color
)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    *color |= (
      *(pwm_data + (7 - i)) == LED_PANELS_1_VALUE ? 1U : 0U
    ) << i;
  }
}


// // RGB to GRB (MSB)
// static void led_panels_convert_pixels_to_pwm_data(
//   uint8_t *const pwm_data,
//   const led_panels_color *const pixels,
//   const uint16_t pixels_num
// )
// {
//   for (uint16_t i = 0; i < pixels_num * 3; i += 3)
//   {
//     uint16_t pixel_index = i / 3;

//     led_panels_convert_color_to_pwm_data(
//       pwm_data + i,
//       &pixels[pixel_index].green
//     );
//     led_panels_convert_color_to_pwm_data(
//       pwm_data + (i + 1),
//       &pixels[pixel_index].red
//     );
//     led_panels_convert_color_to_pwm_data(
//       pwm_data + (i + 2),
//       &pixels[pixel_index].blue
//     );
//   }
// }

// static void led_panels_convert_pwm_data_to_pixels(
//   uint8_t *const pwm_data,
//   led_panels_color *const pixels,
//   const uint16_t pixels_num
// )
// {
//   for (uint16_t i = 0; i < pixels_num * 3; i += 3)
//   {
//     uint16_t pixel_index = i / 3;

//     led_panels_convert_pwm_data_to_color(
//       pwm_data + i,
//       &pixels[pixel_index].green
//     );
//     led_panels_convert_pwm_data_to_color(
//       pwm_data + i,
//       &pixels[pixel_index].red
//     );
//     led_panels_convert_pwm_data_to_color(
//       pwm_data + i,
//       &pixels[pixel_index].blue
//     );
//   }
// }

static void led_panels_convert_pwm_data_to_pixel(
  uint8_t *const pwm_data,
  led_panels_color *const pixel
)
{
  led_panels_convert_pwm_data_to_color(pwm_data, &pixel->green);
  led_panels_convert_pwm_data_to_color(pwm_data + 1, &pixel->red);
  led_panels_convert_pwm_data_to_color(pwm_data + 2, &pixel->blue);
}

static void led_panels_convert_pixel_to_pwm_data(
  uint8_t *const pwm_data,
  led_panels_color *const pixel
)
{
  // led_panels_convert_pwm_data_to_color(pwm_data, &pixel->green);
  // led_panels_convert_pwm_data_to_color(pwm_data + 1, &pixel->red);
  // led_panels_convert_pwm_data_to_color(pwm_data + 2, &pixel->blue);
  led_panels_convert_color_to_pwm_data(pwm_data, &pixel->green);
  led_panels_convert_color_to_pwm_data(pwm_data, &pixel->red);
  led_panels_convert_color_to_pwm_data(pwm_data, &pixel->blue);
}

static uint16_t led_panels_pwm_data_get_offset(
  uint8_t panel_index,
  led_panels_size *panels_sizes
)
{
  uint16_t pixels_offset = 0U;

  uint8_t i = 0;
  for (; i < panel_index; i++)
    pixels_offset += (uint16_t)(panels_sizes[i]);

  return pixels_offset * 3;
}

static uint16_t led_panels_get_pwm_data_size(led_panels_buffer *buffer)
{
  uint16_t result = 0U;

  uint8_t i = 0;
  for (; i < buffer->panels_num; i++)
    result += (uint16_t)(buffer->panels_sizes[i]);

  return result * 3;
}

// // Implementations -----------------------------------------------------------

// void hc06_create()
// {
//   hc06_io_set_baudrate(GET_BAUDRATE_INT(HC06_9600));
//   //hc06_set_baudrate(HC06_9600);
//   current_baudrate = HC06_9600;
// }

// void hc06_destroy()
// {
//   hc06_io_set_baudrate(GET_BAUDRATE_INT(HC06_9600));
// }

// hc06_status hc06_check_link()
// {
//   set_check_cmd();

//   return send_at_cmd();
// }

// hc06_status hc06_set_baudrate(hc06_baudrate baudrate)
// {
//   set_baudrate_cmd(baudrate);

//   hc06_status status = send_at_cmd();
//   status |= hc06_io_set_baudrate(GET_BAUDRATE_INT(baudrate));
//   current_baudrate = baudrate;

//   return status;
// }

// hc06_baudrate hc_06_determine_baudrate(void)
// {
//   hc06_baudrate baudrate = HC06_1200;
//   for (; baudrate <= HC06_UNDEFINED; baudrate++)
//   {
//     hc06_io_set_baudrate(GET_BAUDRATE_INT(baudrate));
//     if (hc06_check_link() == HC06_OK)
//       break;
//   }

//   return baudrate;
// }

// // The name is limited in 20 characters
// // (Guangzhou HC IT HC-06 product datasheet pg. 16).
// hc06_status hc06_set_name(const char* const name)
// {
//   if (strlen(name) > 20)
//     return HC06_ERROR;

//   set_change_name_cmd(name);

//   return send_at_cmd();
// }

// hc06_status hc06_set_pin(const uint16_t pin)
// {
//   if (pin > 0x270F) // 4 digits
//     return HC06_ERROR;

//   set_change_pin_cmd(pin);

//   return send_at_cmd();
// }

// hc06_status hc06_write(const uint8_t *const data, const uint16_t size)
// {
//   return hc06_io_write(data, size);
// }

// hc06_status hc06_read(uint8_t *const data, const uint16_t size)
// {
//   return hc06_io_read(data, size);
// }

led_panels_buffer *led_panels_create(
  uint8_t panels_num,
  const led_panels_size *const panels_sizes
)
{
  uint16_t pixels_num = 0U;

  led_panels_buffer *buffer = malloc(sizeof(led_panels_buffer));
  buffer->panels_num = panels_num;
  buffer->panels_sizes = calloc(panels_num, sizeof(led_panels_size));
  memcpy(
    buffer->panels_sizes,
    panels_sizes,
    sizeof(led_panels_size) * panels_num
  );

  for (uint8_t i = 0; i < panels_num; i++)
    pixels_num += (uint16_t)panels_sizes[i];

  uint16_t total_size = pixels_num * 3 + 50;
  buffer->pwm_data = calloc(total_size, sizeof(uint8_t));
  memset(
    buffer->pwm_data + (total_size - 50),
    LED_PANELS_RESET_VALUE,
    50 * sizeof(uint8_t)
  ); // reset (above 50 us)
  led_panels_flush(buffer);

  return buffer;
}

void led_panels_destroy(led_panels_buffer *buffer)
{
  free(buffer->panels_sizes);
  free(buffer->pwm_data);
  free(buffer);
  buffer = NULL;
}

// led_panels_color *const led_panels_get_colors(
//   led_panels_buffer *buffer,
//   const uint8_t panel_index,
//   uint16_t *const panel_size
// )
// {
//   // uint16_t pixels_offset = 0U;

//   if (panel_index >= buffer->panels_num)
//     return NULL;

//   // uint8_t i = 0;
//   // for (; i < panel_index; i++)
//   //   pixels_offset += (uint16_t)(buffer->panels_sizes[i]);

//   uint16_t pixels_offset = led_panels_pwm_data_get_offset(
//     panel_index,
//     buffer->panels_sizes
//   );
  
//   *panel_size = (uint16_t)(buffer->panels_sizes[panel_index]);

//   //return buffer->pixels + pixels_offset;

//   led_panels_color *pixels = calloc(
//     *panel_size,
//     sizeof(led_panels_color)
//   );
//   led_panels_convert_pwm_data_to_pixels(
//     buffer->pwm_data + pixels_offset,
//     pixels,
//     *panel_size
//   );

//   return pixels;
// }

// led_panels_status led_panels_set_colors(
//   led_panels_buffer *buffer,
//   const uint8_t panel_index,
//   led_panels_color *pixels
// )
// {
//   if (panel_index >= buffer->panels_num)
//     return LED_PANELS_ERROR;

//   uint16_t pixels_offset = led_panels_pwm_data_get_offset(
//     panel_index,
//     buffer->panels_sizes
//   );

//   led_panels_convert_pwm_data_to_pixels(
//     buffer->pwm_data + pixels_offset,
//     pixels,
//     buffer->panels_sizes[panel_index]
//   );

//   free(pixels);
//   pixels = NULL;
  
//   return LED_PANELS_OK;
// }

led_panels_status led_panels_get_pixel(
  led_panels_buffer *buffer,
  const uint8_t panel_index,
  const uint8_t pixel_x,
  const uint8_t pixel_y,
  led_panels_color *const pixel
)
{
  if (panel_index >= buffer->panels_num)
    return LED_PANELS_ERROR;

  uint16_t pixels_offset = led_panels_pwm_data_get_offset(
    panel_index,
    buffer->panels_sizes
  );
  uint8_t *panel = buffer->pwm_data + pixels_offset;
  uint16_t pwm_positions_offset = pixel_x * 3 + 
    (pixel_y * 3 * (uint16_t)buffer->panels_sizes[panel_index]);

  led_panels_convert_pwm_data_to_pixel(
    panel + pwm_positions_offset,
    pixel
  );

  return LED_PANELS_OK;
}

led_panels_status led_panels_set_pixel(
  led_panels_buffer *buffer,
  const uint8_t panel_index,
  const uint8_t pixel_x,
  const uint8_t pixel_y,
  const led_panels_color pixel
)
{
  if (panel_index >= buffer->panels_num)
    return LED_PANELS_ERROR;

  uint16_t pixels_offset = led_panels_pwm_data_get_offset(
    panel_index,
    buffer->panels_sizes
  );
  uint8_t *panel = buffer->pwm_data + pixels_offset;
  uint16_t pwm_positions_offset = pixel_x * 3 + 
    (pixel_y * 3 * (uint16_t)buffer->panels_sizes[panel_index]);

  // led_panels_convert_pwm_data_to_pixel(
  //   panel + pwm_positions_offset,
  //   pixel
  // );

  led_panels_convert_pixel_to_pwm_data(
    panel + pwm_positions_offset,
    &pixel
  );

  return LED_PANELS_OK;
}

led_panels_status led_panels_flush(led_panels_buffer *buffer)
{
  memset(
    buffer->pwm_data,
    LED_PANELS_0_VALUE,
    led_panels_get_pwm_data_size(buffer)
  );
}

led_panels_status led_panels_send(led_panels_buffer *buffer)
{

  return led_panels_io_send_data(
    buffer->pwm_data,
    led_panels_get_pwm_data_size(buffer)
  );
}
