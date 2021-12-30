# Kidsbox

This is a battery-powered rfid-tag-controlled audio player meant to be used by kids.
You can bundle audio-files to rfid tags and let your kids choose the music or audiobook of their favour by placing the corresponding tag atop of the box.

## Todo

- Volume buttons
- Playlists
- Pausing
- ... I have a lot of ideas :)

## Setup

1. Install ESP-ADF and ESP-IDF according to the [install guide](https://docs.espressif.com/projects/esp-adf/en/latest/get-started/index.html#installation-step-by-step).
2. Clone this repository and `cd` into the repository directory.
3. `git submodule init --recursive`
3. `cd device`
4. Make sure that you have `idf.py` available in your environment. This is described in the install guide.
5. Execute `idf.py build` to compile, `idf.py build flash monitor` to flash it to your board.

## Hardware

### Parts

_TBD_

|Part|Links|
|-|-|
|ESP32-LyraT 4.3|[Mouser](https://www.mouser.de/ProductDetail/Espressif-Systems/ESP32-LyraT?qs=MLItCLRbWsxPzPCja546ZA%3D%3D)|


### Hardware Setup

The peripherals must be configured for:

- SD Card
- RFID-module
- Vol+/- buttons 

The ESP32-LyraT board has only a few exposed GPIO pins. To save pins, the SD-Card reader runs only in 1-Wire mode. I have actually no idea what the limitations and differences are, but I didn't encounter any problems yet.

#### DIP-Switch

The switches must be exatly in the positions outlined in the table.

|Number|Setting|Purpose|
|-|-|-|
|1|OFF|Disconnects GPIO 12 from SD-D2|
|2|OFF|Disconnects GPIO 13 from SD-D3|
|3|OFF|Connects GPIO 15 to SD CMD|
|4|ON|Connects GPIO 13 to the JTAG header|
|5|ON|Connects GPIO 12 to the JTAG header|
|6|OFF|Connects GPIO 13 to SD CLK|
|7|OFF|Disconnects GPIO 12 from the AUX-in-detection|
|8|OFF| - |

#### RFID

_The MISO/MOSI pins are not inverted on host/chip_

Connection Table:

|RC522 Pin|ESP32 Lyrat v4.3 Pin|GPIO Number|
|-|-|-|
|3.3v|3.3v|-|
|RST|3.3v|-|
|GND|GND|-|
|SDA|I2C SDA|18|
|SCK|I2C SCL|23|
|MOSI|JTAG MTCK|13|
|MISO|JTAG MTDI|12|