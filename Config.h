/**
 * Open Beer Filler
 * Copyright (c) 2020 Gerhard Potgieter [https://gerhardpotgieter.com]
 *
 * Based on an idea by Christopher Harrison-Hawkes [https://harrisonsbrewery.com]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once;

/**
 * General Config Options
 */
#define CO2_PURGE_PERIOD 2000
#define MOVE_BEER_BELT_PERIOD 5000
#define FILLER_TUBE_MOVEMENT_DELAY 2000
#define CO2_PURGE_RETRACTION_DELAY 1000
#define CO2_PURGE_RETRACTION_PERIOD 500
#define FILL_SENSORS_TIMER_FREQUENCY 100000 // 100ms This value needs to be defined in microseconds.
#define FILL_SENSORS_TRIGGER 400 // Int between 0 and 1023 used to trigger the fill sensor: operating voltage(5v or 3.3v) / 1024
#define VARIABLE_FILL_SENSOR_TRIGGER false // Use a potentiometer to adjust trigger value

/**
 * Feature flags
 */
//#define CONINUOUS_FILLING // Uncomment this to have the filling process repeat for new batch after the current batch has completed it filling.
