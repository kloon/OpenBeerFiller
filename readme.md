# Open Beer Filler
![](https://github.com/kloon/OpenBeerFiller/workflows/Compile%20Test/badge.svg)
This application is a Arduino/ESP based beer filling controller. The idea was started by Christopher Harrison-Hawkes AKA HarryBrew69 on YouTube. This application work by controlling a beer bottling line via a microcrontoller and various sensors.

## Configuration
Before uploading the sketch to your microcontroller be sure to modify the InputConfig.h and Config.h files to map it to your microcontroller pins.

### InputConfig.h Definitions
VARIABLE | DEFINITION
--- | ---
START_BUTTON | The pin on which the start button is attached
BEER_INLET_SOL_1 | The pin controlling the solenoid for beer tube 1
BEER_INLET_SOL_2 | The pin controlling the solenoid for beer tube 2
BEER_INLET_SOL_3 | The pin controlling the solenoid for beer tube 3
BEER_FILL_SENSOR_1 | The pin that senses when the fill sensor for beer tube 1 is triggered
BEER_FILL_SENSOR_2 | The pin that senses when the fill sensor for beer tube 2 is triggered
BEER_FILL_SENSOR_3 | The pin that senses when the fill sensor for beer tube 3 is triggered
CO2_PURGE_SOL | The pin controlling the solenoid for releasing CO2
FILL_RAIL_SOL | The pin controlling the solenoid for lifting and dropping the filler rail
BEER_BELT_SOL | The pin controlling the solenoid for moving the beer line forward

### Config.h Definitions
VARIABLE | DEFINITION
--- | ---
CO2_PURGE_PERIOD | How long in milliseconds to purge with CO2 before filling with beer
CO2_PURGE_RETRACTION_PERIOD | How long in milliseconds to purge with CO2 after filling with beer, this happes mid lifting the filler tubes.
CO2_PURGE_RETRACTION_DELAY | How long in milliseconds to wait before starting CO2 purge after starting to retract filler tubes.
FILLER_TUBE_MOVEMENT_DELAY | How long in milliseconds to wait after raising/lowering filler tube before starting the next action like opening the filling lines.
MOVE_BEER_BELT_PERIOD | How long in milliseconds to run the beer belt solenoid for, ie how long should the beer belt move.

## Schematic
![](.github/schematic.png)

## Troubleshooting

### Adding InputConfig.h & Config.h files to your build
In you Arduino Sketch folder there is a folder called Libraries, create a folder there and put your header file in that folder and restart the Arduino IDE.
e.g.
Documents/Arduino/Libraries/OpenBeerFiller/InputConfig.h
Documents/Arduino/Libraries/OpenBeerFiller/Config.h
