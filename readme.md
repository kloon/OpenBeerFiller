# Open Beer Filler
This application is a Arduino/ESP based beer filling controller. The idea was started by Christopher Harrison-Hawkes AKA HarryBrew69 on YouTube. This application work by controlling a beer bottling line via a microcrontoller and various sensors.

## Configuration
Before uploading the sketch to your microcontroller be sure to modify the InputConfig.h and Config.h files to map it to your microcontroller pins.

## Adding InputConfig.h & Config.h files to your build
In you Arduino Sketch folder there is a folder called Libraries, create a folder there and put your header file in that folder and restart the Arduino IDE.
e.g.
Documents/Arduino/Libraries/OpenBeerFiller/InputConfig.h
Documents/Arduino/Libraries/OpenBeerFiller/Config.h
