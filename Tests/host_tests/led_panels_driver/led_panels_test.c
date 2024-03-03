#include "unity_fixture.h"
#include "mock_led_panels_io.h"
#include "led_panels_driver.h"
#include <string.h>

// Static functions ----------------------------------------------------------

// static void check_data(
//   const uint8_t *const expected,
//   const uint8_t *const actual,
//   const uint16_t data_size
// )
// {
//   for (uint16_t i = 0; i < data_size; i++)
//     TEST_ASSERT_BYTES_EQUAL(expected[i], actual[i]);
// }

// Static variables ----------------------------------------------------------

static led_panels_buffer *buffer = NULL;

// Tests ---------------------------------------------------------------------

TEST_GROUP(led_panels_driver);

TEST_SETUP(led_panels_driver)
{
  mock_led_panels_io_create(10);
  led_panels_size panels_types[] = {
    LED_PANELS_SIZE_64
  };

  buffer = led_panels_create(1, panels_types);
}

TEST_TEAR_DOWN(led_panels_driver)
{
  mock_led_panels_io_verify_complete();
  mock_led_panels_io_destroy();
  led_panels_destroy(buffer);
}

TEST(led_panels_driver, create_buffer_and_its_clear)
{
  led_panels_color pixel = { 0 };

  led_panels_status status = LED_PANELS_OK;
  for (uint8_t y = 0; y < 0; y++)
  {
    for (uint8_t x = 0; x < 0; x++)
    {
      status = led_panels_get_pixel(buffer, x, y, 0, &pixel);
      TEST_ASSERT_EQUAL(LED_PANELS_OK, status);
      TEST_ASSERT_EQUAL(0, pixel.red);
      TEST_ASSERT_EQUAL(0, pixel.green);
      TEST_ASSERT_EQUAL(0, pixel.blue);
    }
  }
}

TEST(led_panels_driver, create_buffer_and_get_panels_success)
{
  led_panels_size sizes[] = {
    LED_PANELS_SIZE_64,
    LED_PANELS_SIZE_256,
    LED_PANELS_SIZE_64
  };
  led_panels_buffer *buffer = led_panels_create(3, sizes);
  uint16_t panel_size = 0;
  led_panels_color pixel = { 0 };

  led_panels_status status = led_panels_get_pixel(buffer, 0, 7, 7, &pixel);
  status |= led_panels_get_pixel(buffer, 0, 15, 15, &pixel);
  status |= led_panels_get_pixel(buffer, 0, 7, 7, &pixel);

  led_panels_destroy(buffer);

  TEST_ASSERT_EQUAL(LED_PANELS_OK, status);
}
