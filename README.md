## USB Extended Display Example

Use [Crowpanel 10.1 inch esp32=p4 panel](https://www.elecrow.com/crowpanel-advanced-10-1inch-esp32-p4-hmi-ai-display-1024x600-ips-touch-screen-wifi-6.html) to flash this example.

This is the modified version of https://github.com/espressif/esp-iot-solution/tree/master/examples/usb/device/usb_extend_screen for  CrowPanel Advanced 10.1inch |ESP32-P4 HMI AI Display 1024x600 IPS Touch Screen

The USB extended display example turns a compatible ESP32-P4 board into a secondary display for Windows. 


The example supports the following features:

* **esp32-p4**: Supports a screen refresh rate of **1024×600@60FPS**.

* Supports up to **five-point touch input**.

## Required Hardware

### Crowpanel 10.1 inch Development Board

1. Use [Crowpanel 10.1 inch esp32=p4 panel](https://www.elecrow.com/crowpanel-advanced-10-1inch-esp32-p4-hmi-ai-display-1024x600-ips-touch-screen-wifi-6.html) development board.
2. A **1024×600** MIPI display from the development kit.
3. A speaker.


## Hardware Connection

1. Connect the high-speed USB port on the development board to the PC.

## Compilation and Flashing

### Device Side

Build the project, flash it to the board, and run the monitor tool to check the serial output:




1. Run `idf.py -p PORT flash monitor` to build, flash and monitor the project.

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.



### PC Side

For preparation, refer to [windows_driver](./windows_driver/README.md).

![Demo](https://dl.espressif.com/AE/esp-iot-solution/p4_usb_extern_screen.gif)

## Troubleshooting

### The touchscreen controls the wrong display

1. Open **Control Panel** and select **Tablet PC Settings**.
2. Under the **Display** section, select **Setup**.
3. Follow the on-screen instructions to choose the correct extended display.

### Adjusting JPEG Image Quality

* Modify `CONFIG_USB_EXTEND_SCREEN_JPEG_QUALITY`. A higher value increases image quality but also consumes more memory per frame.

### Changing the Secondary Screen Resolution

* The resolution and RGB565/RGB888 framebuffer format come from the selected board's `display_lcd` configuration; they are also used to build the USB vendor string, HID coordinate range, frame validation, and JPEG output buffers.
* To use another panel, select a matching board definition or apply/create a Board Manager amend, then run `idf.py bmgr` again.

**Note:** The driver currently does not support portrait-oriented screens. Please use a screen designed for landscape mode.

### ESP32-P4 Chip Revisions and JPEG Alignment

* ESP-IDF firmware built for ESP32-P4 revisions `<3.0` and `>=3.0` is mutually incompatible. Board Manager selects the board and target, but it does not select the silicon revision. For a `<3.0` chip, enable `CONFIG_ESP32P4_SELECTS_REV_LESS_V3` with `idf.py menuconfig` and perform a clean rebuild; do not put this setting in generic board defaults.
* Hardware JPEG output is rounded to the input JPEG's MCU size: 8×8 for YUV444, 16×8 for YUV422, and 16×16 for YUV420. The example parses each JPEG header, reserves worst-case 16×16-aligned storage, and removes row padding before drawing. A visible resolution such as 1024×600 therefore does not need to be changed to 1024×608.
* Revisions `<3.0` also restrict some YUV-to-YUV conversions. This example decodes directly to the display's RGB565 or RGB888 framebuffer format and does not use those restricted combinations.

### Adjusting Image Output Frame Rate

* Modify `CONFIG_USB_EXTEND_SCREEN_MAX_FPS`. Lowering this value effectively reduces USB bandwidth usage. If USB audio stuttering occurs, consider decreasing this value.

### Modifying the Maximum Frame Size

* Modify `CONFIG_USB_EXTEND_SCREEN_FRAME_LIMIT_B` to limit the maximum image size received from the PC driver.

### Disabling Touchscreen Functionality

* Set `CONFIG_HID_TOUCH_ENABLE` to `n`.
* This option is only available when the selected board provides `lcd_touch`.

### Disabling Audio Functionality

* Set `CONFIG_UAC_AUDIO_ENABLE` to `n`.
* This option is only available when the selected board provides Board Manager audio codec devices.

**Note:** If only the secondary screen function is enabled, change the PID to `0x2987`.
