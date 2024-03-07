#include "unity_fixture.h"
#include "led_panels_driver.h"

// Static functions ----------------------------------------------------------

static void delay()
{
  int32_t count = 10000000;
  while (count--) {
    asm(""); // prevent optimize of this function
  }
}

// Static variables ----------------------------------------------------------

static led_panels_buffer *buffer = NULL;

static led_panels_color red_pixel = (led_panels_color) {
  .red = 1U,
  .green = 0U,
  .blue = 0U
};
static led_panels_color green_pixel = (led_panels_color) {
  .red = 0U,
  .green = 1U,
  .blue = 0U
};
static led_panels_color blue_pixel = (led_panels_color) {
  .red = 0U,
  .green = 0U,
  .blue = 1U
};
static led_panels_color white_pixel = (led_panels_color) {
  .red = 1U,
  .green = 1U,
  .blue = 1U
};

// Tests ---------------------------------------------------------------------

TEST_GROUP(led_panels_driver);

TEST_SETUP(led_panels_driver)
{
  led_panels_size panels_types[] = { LED_PANELS_SIZE_64 };

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
  for (uint8_t y = 0; y < 8; y++)
  {
    for (uint8_t x = 0; x < 8; x++)
      led_panels_set_pixel(buffer, 0, x, y, blue_pixel);
  }
  led_panels_send(buffer);
  delay();
}

TEST(led_panels_driver, flush_panel)
{
  for (uint8_t y = 0; y < 8; y++)
  {
    for (uint8_t x = 0; x < 8; x++)
      led_panels_set_pixel(buffer, 0, x, y, blue_pixel);
  }
  led_panels_send(buffer);
  led_panels_flush(buffer);
  led_panels_send(buffer);

  delay();
}

TEST(led_panels_driver, test_corners)
{
  led_panels_set_pixel(buffer, 0, 0, 0, red_pixel);
  led_panels_set_pixel(buffer, 0, 7, 0, green_pixel);
  led_panels_set_pixel(buffer, 0, 0, 7, blue_pixel);
  led_panels_set_pixel(buffer, 0, 7, 7, white_pixel);

  led_panels_send(buffer);

  delay();
}

TEST(led_panels_driver, test_vertical_lines)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    led_panels_set_pixel(buffer, 0, 2, i, red_pixel);
    led_panels_set_pixel(buffer, 0, 5, i, blue_pixel);
  }

  led_panels_send(buffer);

  delay();
}

TEST(led_panels_driver, test_horizontal_lines)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    led_panels_set_pixel(buffer, 0, i, 2, red_pixel);
    led_panels_set_pixel(buffer, 0, i, 5, blue_pixel);
  }

  led_panels_send(buffer);

  delay();
}
