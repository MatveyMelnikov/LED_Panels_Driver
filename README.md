# LED panels driver
### Launch
* ```make``` - building a production version of the code (does not contain tests);
* ```make -f MakefileTest.mk``` - building a test version. Depending on the [FOR_TARGET](https://github.com/MatveyMelnikov/LED_Panels_Driver/blob/master/MakefileTest.mk) parameter,
either file [MakefileTestTarget.mk](https://github.com/MatveyMelnikov/LED_Panels_Driver/blob/master/MakefileTestTarget.mk) (for the microcontroller) or file [MakefileTestHost.mk](https://github.com/MatveyMelnikov/LED_Panels_Driver/blob/master/MakefileTestHost.mk) (for launching on the development system) is launched;

### Structure
The driver itself is located [here](https://github.com/MatveyMelnikov/LED_Panels_Driver/tree/master/External/led_panels_driver).
Tests using the [Unity library](https://github.com/MatveyMelnikov/LED_Panels_Driver/tree/master/External/Unity-2.5.2) are implemented [here](https://github.com/MatveyMelnikov/LED_Panels_Driver/tree/master/Tests). The tests are divided into tests for the host (development system) and target (stm32).
Target tests sequentially output test images at intervals of about half a second. Only one panel is tested

### Hardware
VCC - 5V. The panel used is WS2812B 8 by 8 pixels. The driver provides the ability to use a large panel - 16 by 16
![sheme_led_panel](https://github.com/MatveyMelnikov/LED_Panels_Driver/assets/55649891/6beb48f3-2d44-4ee2-a0c8-f7c26c1808d2)

