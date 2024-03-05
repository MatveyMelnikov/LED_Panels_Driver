#include "unity_fixture.h"
#include "mock_led_panels_io.h"
#include "led_panels_driver.h"
#include <string.h>

// Defines -------------------------------------------------------------------

#define PWM_PIXEL_SIZE 24

// Static variables ----------------------------------------------------------

static led_panels_buffer *buffer = NULL;
const static led_panels_color pixel = (led_panels_color) {
  .red = 0xaa,
  .green = 0x55,
  .blue = 0x33
};

const uint8_t red_pwm[] = {
  LED_PANELS_1_VALUE, LED_PANELS_0_VALUE, LED_PANELS_1_VALUE,
  LED_PANELS_0_VALUE, LED_PANELS_1_VALUE, LED_PANELS_0_VALUE,
  LED_PANELS_1_VALUE, LED_PANELS_0_VALUE
};
const uint8_t green_pwm[] = {
  LED_PANELS_0_VALUE, LED_PANELS_1_VALUE, LED_PANELS_0_VALUE,
  LED_PANELS_1_VALUE, LED_PANELS_0_VALUE, LED_PANELS_1_VALUE,
  LED_PANELS_0_VALUE, LED_PANELS_1_VALUE
};
const uint8_t blue_pwm[] = {
  LED_PANELS_0_VALUE, LED_PANELS_0_VALUE, LED_PANELS_1_VALUE,
  LED_PANELS_1_VALUE, LED_PANELS_0_VALUE, LED_PANELS_0_VALUE,
  LED_PANELS_1_VALUE, LED_PANELS_1_VALUE
};

// Static functions ----------------------------------------------------------

static uint8_t *get_pwm_pixel(
  uint8_t *pwm_data,
  uint8_t size_x,
  uint8_t pos_x,
  uint8_t pos_y
)
{
  return pwm_data + (pos_x + (pos_y * size_x)) * PWM_PIXEL_SIZE;
}

void set_pwm_pixel(uint8_t *pwm_data)
{
  memcpy(pwm_data, green_pwm, 8);
  memcpy(pwm_data + 8, red_pwm, 8);
  memcpy(pwm_data + 16, blue_pwm, 8);
}

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
  status |= led_panels_get_pixel(buffer, 1, 15, 15, &pixel);
  status |= led_panels_get_pixel(buffer, 2, 7, 7, &pixel);

  led_panels_destroy(buffer);

  TEST_ASSERT_EQUAL(LED_PANELS_OK, status);
}

TEST(led_panels_driver, create_buffer_and_get_panels_fail)
{
  led_panels_size sizes[] = {
    LED_PANELS_SIZE_64,
    LED_PANELS_SIZE_256,
    LED_PANELS_SIZE_64
  };
  led_panels_buffer *buffer = led_panels_create(3, sizes);
  led_panels_color pixel = { 0 };

  led_panels_status status = led_panels_get_pixel(buffer, 0, 8, 7, &pixel);
  TEST_ASSERT_EQUAL(LED_PANELS_ERROR, status);
  status = led_panels_get_pixel(buffer, 0, 15, 20, &pixel);
  TEST_ASSERT_EQUAL(LED_PANELS_ERROR, status);
  status = led_panels_get_pixel(buffer, 4, 4, 1, &pixel);
  TEST_ASSERT_EQUAL(LED_PANELS_ERROR, status);

  led_panels_destroy(buffer);
}

TEST(led_panels_driver, send_one_panel_8_data)
{
  uint8_t output_data[64 * PWM_PIXEL_SIZE];
  memset(output_data, LED_PANELS_0_VALUE, 64 * PWM_PIXEL_SIZE);

  // GRB, high bit sent at first
  set_pwm_pixel(get_pwm_pixel(output_data, 8, 0, 0));
  set_pwm_pixel(get_pwm_pixel(output_data, 8, 7, 0));
  set_pwm_pixel(get_pwm_pixel(output_data, 8, 0, 7));
  set_pwm_pixel(get_pwm_pixel(output_data, 8, 7, 7));

  mock_led_panels_io_expect_send_data(output_data, sizeof(output_data));

  led_panels_status status = led_panels_set_pixel(buffer, 0, 0, 0, pixel);
  status |= led_panels_set_pixel(buffer, 0, 7, 0, pixel);
  status |= led_panels_set_pixel(buffer, 0, 0, 7, pixel);
  status |= led_panels_set_pixel(buffer, 0, 7, 7, pixel);

  status |= led_panels_send(buffer);

  TEST_ASSERT_EQUAL(LED_PANELS_OK, status);
}
