Your project documentation is quite clear and well-structured! Here are some suggestions for improving clarity and readability while maintaining your original intent:

---

# BLESA Final Project
This project was created as part of the ByteLab Embedded Software Academy 2024.

The project implements two main functionalities:
- **Oscilloscope**
- **Function Generator**

## Getting Started

### Required Version
ESP-IDF version 5.0 or greater is required.

### Build and Run the Project

1. Clone the project and run this instruction from `final-project` directory:
   ```bash
   git submodule update --init --recursive
   ```
2. Apply the patch for the `lvgl_esp32_drivers` submodule to update it to LVGL 8.3. Navigate to the `components/lvgl_esp32_drivers` directory and run:
   ```bash
   git apply ../lvgl_esp32_drivers_8-3.patch
   ```
3. Configure the project:
   - If using the Byte Lab Development Kit, copy `sdkconfig.defaults` to `sdkconfig`:
     ```bash
     cp sdkconfig.defaults sdkconfig
     ```
   - If using a custom board, configure the project by following the [Configuration section](#configuration).

4. Set up the required environmental variables by running:
   ```bash
   . $HOME/esp/esp-idf/export.sh
   ```
5. Build the project with:
   ```bash
   idf.py build
   ```
6. Flash the project with:
   ```bash
   idf.py flash /dev/ttyUSBX
   ```
   *(Replace X with the port to which the board is connected. This applies to step 7 as well.)*

   ❗ If using the Byte Lab Development Kit, ensure the TCH_IRQ switch on the peripheral module is OFF.

7. Monitor the project's output with:
   ```bash
   idf.py monitor /dev/ttyUSBX
   ```

## External Libraries
This project uses two external libraries:
- [LVGL](https://github.com/lvgl/lvgl) (Version: 8.3)
- [lvgl_esp32_drivers](https://github.com/lvgl/lvgl_esp32_drivers) (A patch is provided for LVGL 8.3 support)

### [LVGL](https://github.com/lvgl/lvgl)
This library is an embedded graphics library used to create beautiful UIs for any MCU, MPU, and display types. It provides APIs for creating buttons, switches, labels, etc. For displaying these objects on our ILI9341 display and utilizing the XPT2046 touch controller, we use the additional `lvgl_esp32_drivers` library.

### [LVGL_ESP32_DRIVERS](https://github.com/lvgl/lvgl_esp32_drivers)
This submodule allows us to utilize pre-implemented functions for displaying objects on various displays and receiving touch input (including our ILI9341 display and XPT2046 touch controller) on the ESP32. In `gui.c`, these functions are passed as callbacks (for flushing the screen and reading touch input) to the LVGL library.

Together, these two libraries simplify the process of creating embedded graphics on the ESP32.

## Features

### Oscilloscope
- Supports 2 channels (0 - 3.3 V)
- Real-time signal visual feedback
- Independent X-axis adjustment (5 ms/div - 1000 ms/div)
- Screenshot functionality
- Automatic peak-to-peak voltage measurements

### Function Generator
- Supports 4 different signal types:
  - Sine
  - Square
  - Sawtooth
  - Triangle
- Frequencies range from 1 kHz to 10 kHz
- Duty cycle adjustment for the square wave
- Save and load up to 4 different preset signals
- "Pause generating" option

### Additional Features
- Temperature and humidity monitoring
- Overheat protection (set at 30°C)
- Temperature and humidity history charts
- Interaction via both physical buttons and touchscreen
- Can connect to Wi-Fi
- State indication using LEDs

## Overview of Components
Here is a list of components used to provide all necessary functionalities for this project:
- **GUI**: Displays images on the LCD screen and receives inputs through the touchscreen
- **LED**: Signals main states
- **ADC**: Reads analog values from channels for the oscilloscope
- **DAC**: Outputs desired voltage levels for the waveform generator
- **Buttons**: Three buttons used to navigate through GUI windows
- **I2C**: Communicates with peripherals
- **Temperature and Humidity Sensor**: SHT31-DIS-F2
- **Wi-Fi**: For connectivity

## Configuration
The project is already set up for the Byte Lab Development Kit. To use it on a custom board, follow these steps:

1. Open the SDK configuration editor (menuconfig).
2. Navigate to the LVGL TFT Display controller and select the display model you are using.
3. Ensure to choose the appropriate communication bus and transfer modes, then assign pins in the Display Pin Assignments section.
4. Navigate to the LVGL Touch controller and select the touch panel you are using. Assign pins accordingly in the Touchpanel Pin Assignment section.
5. Adjust the horizontal and vertical screen resolution as necessary.