# Kidsbox

### RFID

_The MISO/MOSI pins are not inverted on host/chip_

Connection Table:

|RC522 Pin|ESP32 Lyrat v4.3 Pin|GPIO Number|
|-|-|-|
|3.3v|3.3v|-|
|RST|3.3v|-|
|GND|GND|-|
|SDA|SCLK|5|
|SCK|SCK (I2C header)|23|
|MOSI|LRCK|25|
|MISO|DSDIN|26|