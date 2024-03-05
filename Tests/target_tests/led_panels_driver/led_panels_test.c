#include "unity_fixture.h"
#include "led_panels_driver.h"

// Static functions ----------------------------------------------------------

static void delay()
{
  int32_t count = 30000000;
  while (count--) {
    asm(""); // prevent optimize of this function
  }
}

// Static variables ----------------------------------------------------------

static led_panels_buffer *buffer = NULL;

// Tests ---------------------------------------------------------------------

TEST_GROUP(led_panels_driver);

TEST_SETUP(led_panels_driver)
{
  led_panels_size panels_types[] = {
    LED_PANELS_SIZE_64
  };

  buffer = led_panels_create(1, panels_types);
}

TEST_TEAR_DOWN(led_panels_driver)
{
  led_panels_destroy(buffer);
}

TEST(led_panels_driver, show_clear_panel)
{
  led_panels_send(buffer);
  delay();
}

TEST(led_panels_driver, show_red_panel)
{
  led_panels_color red_pixel = (led_panels_color) {
    .red = 1U,
    .green = 0U,
    .blue = 0U
  };

  for (uint8_t y = 0; y < 8; y++)
  {
    for (uint8_t x = 0; x < 8; x++)
      led_panels_set_pixel(buffer, 0, x, y, red_pixel);
  }
  led_panels_send(buffer);
  delay();
}

TEST(led_panels_driver, show_green_panel)
{
  led_panels_color green_pixel = (led_panels_color) {
    .red = 0U,
    .green = 1U,
    .blue = 0U
  };

  for (uint8_t y = 0; y < 8; y++)
  {
    for (uint8_t x = 0; x < 8; x++)
      led_panels_set_pixel(buffer, 0, x, y, green_pixel);
  }
  led_panels_send(buffer);
  delay();
}

TEST(led_panels_driver, show_blue_panel)
{
  led_panels_color blue_pixel = (led_panels_color) {
    .red = 0U,
    .green = 0U,
    .blue = 1U
  };

  for (uint8_t y = 0; y < 8; y++)
  {
    for (uint8_t x = 0; x < 8; x++)
      led_panels_set_pixel(buffer, 0, x, y, blue_pixel);
  }
  led_panels_send(buffer);
  delay();
}
