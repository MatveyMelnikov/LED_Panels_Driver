#include "unity_fixture.h"

TEST_GROUP_RUNNER(led_panels_driver)
{
  RUN_TEST_CASE(led_panels_driver, show_clear_panel);
  RUN_TEST_CASE(led_panels_driver, show_red_panel);
  RUN_TEST_CASE(led_panels_driver, show_green_panel);
  RUN_TEST_CASE(led_panels_driver, show_blue_panel);
  RUN_TEST_CASE(led_panels_driver, flush_panel);
  RUN_TEST_CASE(led_panels_driver, test_corners);
  RUN_TEST_CASE(led_panels_driver, test_vertical_lines);
  RUN_TEST_CASE(led_panels_driver, test_horizontal_lines);
}