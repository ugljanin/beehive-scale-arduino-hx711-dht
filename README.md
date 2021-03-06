# Beehive scale with Arduino, HX711 and DHT

![Device](images/device.png)

This Beehive scale is made with Wemos D1 mini (ESP8266 based device), which is similar to Arduino in terms of how you need to configure it.
The code is written in C, and [Arduino IDE](https://www.arduino.cc/en/software) is used.

# Hardware

I used HX711 chip, [Wemos D1 mini](https://www.wemos.cc/en/latest/d1/d1_mini.html) and [DHT 22 sensor shield for Wemos D1 mini](https://www.wemos.cc/en/latest/d1_mini_shield/dht.html)

I used board 7x5cm, which is not mandatory, but I used it to make it all compact. Red and green led diods are connected to act as indicators to know if the device is connected to WiFi and if it successfully publishes the measurements.

It is necesarry to provide WiFi access to make it all work.

Power source with 5v is need, I used car battery that has 12v and purchased converter from 12v to 5v from Aliexpress.

Scale is made of steel pipes which are connected with load cell sensor (up to 100KG) that contains strain gauge.

![Load cell](images/load-cell.png)

## Load Cell calibration

Once you connect Load Cell to the scale, you need to calibrate the Load Cell. For calibration you will use `calibration.ino` scatch, where you will load some known weight on your scale and follow instructions from the code.

## Connect HX711 to Wemos

- gnd -> gnd
- dt -> d2
- sck -> d3
- vcc -> 5v

In my case load cell had 4 wires (white, black, green and red) and I connected it to HX711 this way

- black -> e+
- white -> e-
- green -> a-
- red -> a+

![HX711](images/HX711.png)

# Configuring Arduino IDE

1. To enable support for ESP8266 devices, you first need to go to `File > Preferences`.
Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into the "Additional Boards Manager URLs" field as shown in the figure below. Then, click the "OK" button:
![Configuring Arduino](images/Install-ESP8266-Board-add-on-in-Arduino-IDE-enter-URL.png)

2. Go to `Tools > Board` and select Lolin(Wemos) D1 mini.

3. Install `ESP8266` library by going to `Scatch > Include library > Manage Libraries`, search for `ESP8266 by ESP8266 Community` and install it.

# Uploading script on the Wemos D1 mini device

Once hardware is prepared, and Arduino IDE is configured, open `beehive-scale.ino` in Arduino, connect your device with a usb cable to your computer, choose `com port` that is used for your device, make sure you selected correct board as instructed above.

Configure in the scatch:
- WiFi parameters `ssid` and `password`
- Calibration factor `calibrationFactor` and zero factor `zeroFactor` to make sure scale will measure correctly
- ThingSpeak channel number `myChannelNumber` and write API `myWriteAPIKey`
- Double check if you are using same pin numbers for led diods and DHT and HX711 sensors.

When all is done `upload` scatch on your board. You can monitor debug messages in your Serial Minitor inside Arduino IDE, and make sure all work as expected.

# Monitoring measurements

For monitoring you can use web based tool on ThingSpeak, or install free version of mobile app for Android or Iphone. Free version is limited in some functionalities, and if you want to have more details you can purchase full version. This is a screenshot from full version for Iphone.

![thingview iphone app](images/thingview-landscape.jpeg)

