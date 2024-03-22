# File for test cases
# Here you can add platform-specific files (main file, test, etc.)

CFLAGS = -DTEST -DUNITY_INCLUDE_CONFIG_H
TESTS_DIR = Tests
UNITY_DIR = External/Unity-2.5.2
LED_PANELS_DRIVER = External/led_panels_driver
FOR_TARGET = N

C_INCLUDES += \
-I$(LED_PANELS_DRIVER)/Inc \
-I$(UNITY_DIR)/src \
-I$(UNITY_DIR)/extras/fixture/src \
-I$(UNITY_DIR)/extras/memory/src \
-I$(TESTS_DIR)

C_SOURCES += \
$(LED_PANELS_DRIVER)/Src/led_panels_driver.c \
$(UNITY_DIR)/src/unity.c \
$(UNITY_DIR)/extras/fixture/src/unity_fixture.c \
$(UNITY_DIR)/extras/memory/src/unity_memory.c

ifeq ($(FOR_TARGET), Y)
C_SOURCES += \
$(TESTS_DIR)/target_tests.c \
$(TESTS_DIR)/target_tests/led_panels_driver/led_panels_test_runner.c \
$(TESTS_DIR)/target_tests/led_panels_driver/led_panels_test.c \
$(LED_PANELS_DRIVER)/Src/led_panels_io.c

include MakefileTestTarget.mk
else
C_INCLUDES += \
-I$(TESTS_DIR)/fakes/Inc
C_SOURCES += \
$(TESTS_DIR)/host_tests.c \
$(TESTS_DIR)/host_tests/led_panels_driver/led_panels_test_runner.c \
$(TESTS_DIR)/host_tests/led_panels_driver/led_panels_test.c \
$(TESTS_DIR)/fakes/Src/fake_led_panels_io.c

include MakefileTestHost.mk
endif
