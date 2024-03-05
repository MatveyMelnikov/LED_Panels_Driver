#include "unity_fixture.h"

TEST_GROUP_RUNNER(led_panels_driver)
{
  RUN_TEST_CASE(led_panels_driver, show_clear_panel);
  RUN_TEST_CASE(led_panels_driver, show_red_panel);
  RUN_TEST_CASE(led_panels_driver, show_green_panel);
  RUN_TEST_CASE(led_panels_driver, show_blue_panel);
}