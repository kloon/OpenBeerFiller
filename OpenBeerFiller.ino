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

#include "Config.h";
#include "InputConfig.h";

/**
 * ***************************************************************************
 * ******************************* VARIABLES *********************************
 * ***************************************************************************
 */
bool fillSensor1Triggered = false;
bool fillSensor2Triggered = false;
bool fillSensor3Triggered = false;
bool fillingInProgress = false;

void ICACHE_RAM_ATTR triggerFullFillSensor1();
void ICACHE_RAM_ATTR triggerFullFillSensor2();
void ICACHE_RAM_ATTR triggerFullFillSensor3();

/**
 * ***************************************************************************
 * ******************************** FUNCTIONS ********************************
 * ***************************************************************************
 */
void setupPins() {
  // Beer inlet solenoids
  pinMode(BEER_INLET_SOL_1, OUTPUT);
  pinMode(BEER_INLET_SOL_2, OUTPUT);
  pinMode(BEER_INLET_SOL_3, OUTPUT);

  // Other solenoids controlling various things.
  pinMode(CO2_PURGE_SOL, OUTPUT);
  pinMode(FILL_RAIL_SOL, OUTPUT);
  pinMode(BEER_BELT_SOL, OUTPUT);
}

/**
 * Use iterrupts to trigger when a fill sensor is triggered.
 */
void setupInterrupts() {
  // Fire these functions when bottles are full.
  attachInterrupt(digitalPinToInterrupt(BEER_FILL_SENSOR_1), triggerFullFillSensor1, RISING);
  attachInterrupt(digitalPinToInterrupt(BEER_FILL_SENSOR_2), triggerFullFillSensor2, RISING);
  attachInterrupt(digitalPinToInterrupt(BEER_FILL_SENSOR_3), triggerFullFillSensor3, RISING);
}

/**
 * Fired when fill sensor 1 is triggered as full.
 */
void triggerFullFillSensor1() {
  Serial.println( "Beer fill sensor 1 triggered" );
  // Close beer inlet solenoid.
  digitalWrite(BEER_INLET_SOL_1, LOW);
  fillSensor1Triggered = true;
}

/**
 * Fired when fill sensor 1 is triggered as full.
 */
void triggerFullFillSensor2() {
  Serial.println( "Beer fill sensor 2 triggered" );
  // Close beer inlet solenoid.
  digitalWrite(BEER_INLET_SOL_2, LOW);
  fillSensor2Triggered = true;
}

/**
 * Fired when fill sensor 1 is triggered as full.
 */
void triggerFullFillSensor3() {
  Serial.println( "Beer fill sensor 3 triggered" );
  // Close beer inlet solenoid.
  digitalWrite(BEER_INLET_SOL_3, LOW);
  fillSensor3Triggered = true;
}

/**
 * Return whether all fill sensors have been triggered or not.
 */
bool allFillSensorsTriggered() {
  return fillSensor1Triggered && fillSensor2Triggered && fillSensor3Triggered;
}

void resetFillSensorTriggers() {
  fillSensor1Triggered = fillSensor2Triggered = fillSensor3Triggered = fillingInProgress = false;
}

/**
 * Open all beer inlet solenoids.
 */
void openBeerInlets() {
  Serial.println( "Opening all beer inlets" );
  digitalWrite(BEER_INLET_SOL_1, HIGH);
  digitalWrite(BEER_INLET_SOL_2, HIGH);
  digitalWrite(BEER_INLET_SOL_3, HIGH);
  fillingInProgress = true;
}

/**
 * Open the CO2 purge solenoid, wait a while and then close it again.
 */
void purgeCO2() {
  Serial.println( "Purging CO2" );
  digitalWrite(CO2_PURGE_SOL, HIGH);
  delay(PURGE_CO2_PERIOD);
  digitalWrite(CO2_PURGE_SOL, LOW);
}

/**
 * Raise the fillter tubes out of the bottles.
 */
void raiseFillerTubes() {
  Serial.println("Raising filler tubes");
  digitalWrite(FILL_RAIL_SOL, LOW);
  // We might need to add a delay here depending on how long it takes to fully raise the tubes.
}

/**
 * Lower the filler tubes into the bottles.
 */
void lowerFillerTubes() {
  Serial.println("Lowering filler tubes");
  digitalWrite(FILL_RAIL_SOL, HIGH);
  delay (WAIT_FOR_TUBES);
  // We might need to add a delay here depending on how long it takes to fully lower the tubes.
}

/**
 * Move the beer belt, wait a while and then stop it again.
 */
void moveBeerBelt() {
  Serial.println( "Moving beer belt" );
  digitalWrite(BEER_BELT_SOL, HIGH);
  delay(MOVE_BEER_BELT_PERIOD);
  digitalWrite(BEER_BELT_SOL, LOW);
}


/**
 * Main setup routine.
 */
void setup() {
  Serial.begin(9600);
  setupPins();
  setupInterrupts();
}

/**
 * The main program loop, where all the magic comes togetger.
 */
void loop() {
  // What do we want to do when the program starts? We cannot just start pouring beer????

  // Start the process.
  // Lets assume the belt has bottles and there are empty bottles underneath the filler tubes.
  if ( ! allFillSensorsTriggered() && ! fillingInProgress ) {
    lowerFillerTubes();
    openBeerInlets();
    purgeCO2();
  }
  // If we are done filling, rase filling tubes, move the beer belt for next batch and reset the triggers to start all over again.
  if ( allFillSensorsTriggered() && fillingInProgress ) {
    raiseFillerTubes();
    moveBeerBelt();
    resetFillSensorTriggers();
  }
}
