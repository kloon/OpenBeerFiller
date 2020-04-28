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

#include <TimerOne.h>;
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
enum ProgramState {UNDEF,START,FILLING,STOP};
ProgramState currentState = UNDEF;

/**
 * ***************************************************************************
 * ******************************** FUNCTIONS ********************************
 * ***************************************************************************
 */
void setupPins() {
  // Filler solenoids.
  pinMode(BEER_INLET_SOL_1, OUTPUT);
  pinMode(BEER_INLET_SOL_2, OUTPUT);
  pinMode(BEER_INLET_SOL_3, OUTPUT);

  // CO2 solenoid.
  pinMode(CO2_PURGE_SOL, OUTPUT);

  // Fill rail solenoid.
  pinMode(FILL_RAIL_SOL, OUTPUT);

  // Beer belt solenoid.
  pinMode(BEER_BELT_SOL, OUTPUT);

  // Fill sensors.
  pinMode(BEER_FILL_SENSOR_1, INPUT);
  pinMode(BEER_FILL_SENSOR_2, INPUT);
  pinMode(BEER_FILL_SENSOR_3, INPUT);

  // Start/Stop button.
  pinMode(START_BUTTON, INPUT);
}

/**
 * Setup a non-blocking interrupt timer for checking the fill sensors.
 */
void setupFillSensorsTimer() {
  Timer1.initialize(FILL_SENSORS_TIMER_DELAY);
  Timer1.attachInterrupt(checkFillSensors);
}

/**
 * Check if the fill sensors have been triggered.
 */
void checkFillSensors() {
  if(digitalRead(BEER_FILL_SENSOR_1)) {
    triggerFullFillSensor1();
  }
  if(digitalRead(BEER_FILL_SENSOR_2)) {
    triggerFullFillSensor2();
  }
  if(digitalRead(BEER_FILL_SENSOR_3)) {
    triggerFullFillSensor3();
  }
}

/**
 * Fired when fill sensor 1 is triggered as full.
 */
void triggerFullFillSensor1() {
  closeBeerFillerTube(BEER_INLET_SOL_1);
  fillSensor1Triggered = true;
  Serial.println("Filler tube 1 closed");
}

/**
 * Fired when fill sensor 1 is triggered as full.
 */
void triggerFullFillSensor2() {
  closeBeerFillerTube(BEER_INLET_SOL_2);
  fillSensor2Triggered = true;
  Serial.println("Filler tube 2 closed");
}

/**
 * Fired when fill sensor 1 is triggered as full.
 */
void triggerFullFillSensor3() {
  closeBeerFillerTube(BEER_INLET_SOL_3);
  fillSensor3Triggered = true;
  Serial.println("Filler tube 3 closed");
}

/**
 * Return whether all fill sensors have been triggered or not.
 */
bool allFillSensorsTriggered() {
  return fillSensor1Triggered && fillSensor2Triggered && fillSensor3Triggered;
}

void resetFillSensorTriggers() {
  fillSensor1Triggered = fillSensor2Triggered = fillSensor3Triggered = false;
}

/**
 * Open a single beer filler solenoid.
 */
void openBeerFillerTube(int fillerTubePin) {
  digitalWrite(fillerTubePin, HIGH);
}

/**
 * Close a single beer filler solenoid.
 */
void closeBeerFillerTube(int fillerTubePin) {
  digitalWrite(fillerTubePin, LOW);
}

/**
 * Open all beer filler solenoids.
 */
void openAllBeerFillerTubes() {
  Serial.println("Opening all beer filler tubes");
  digitalWrite(BEER_INLET_SOL_1, HIGH);
  digitalWrite(BEER_INLET_SOL_2, HIGH);
  digitalWrite(BEER_INLET_SOL_3, HIGH);
}

/**
 * Close all beer filler solenoids.
 */
void closeAllBeerFillerTubes() {
  Serial.println("Closing all beer filler tubes");
  digitalWrite(BEER_INLET_SOL_1, LOW);
  digitalWrite(BEER_INLET_SOL_2, LOW);
  digitalWrite(BEER_INLET_SOL_3, LOW);
}

/**
 * Open the CO2 purge solenoid, wait a while and then close it again.
 */
void purgeCO2( bool retract = false ) {
  Serial.println("Purging CO2");
  digitalWrite(CO2_PURGE_SOL, HIGH);
  if(!retract) {
    delay(CO2_PURGE_PERIOD);
  } else {
    delay(CO2_PURGE_RETRACTION_PERIOD);
  }
  digitalWrite(CO2_PURGE_SOL, LOW);
}

/**
 * Raise the fillter tubes out of the bottles.
 */
void raiseFillerTubes() {
  Serial.println("Raising filler tubes");
  digitalWrite(FILL_RAIL_SOL, HIGH);
  delay(CO2_PURGE_RETRACTION_DELAY); // We use CO2_PURGE_RETRACTION_DELAY here as we want to start purging with CO2 as the fill rail raises.
}

/**
 * Lower the filler tubes into the bottles.
 */
void lowerFillerTubes() {
  Serial.println("Lowering filler tubes");
  digitalWrite(FILL_RAIL_SOL, LOW);
  delay(FILLER_TUBE_MOVEMENT_DELAY);
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
 * Code to run when we are in the START ProgramState.
 */
void startState() {
  moveBeerBelt();
  lowerFillerTubes();
  purgeCO2();
  openAllBeerFillerTubes();
  currentState = FILLING;
}

/**
 * Code to run when we are in the FILLING ProgramState.
 */
void fillingState() {
  // Check if we are done filling.
  if(allFillSensorsTriggered()){
    raiseFillerTubes();
    purgeCO2(true);
    resetFillSensorTriggers();
    // If done filling, reset ProgramState to START so it repeats the filling process again.
    currentState = START;
  }
}

/**
 * Code to run when we are in the STOP ProgramState.
 */
void stopState() {
  // Reset the ProgramState to UNDEF.
  currentState = UNDEF;
}

void readStartButton() {
  if(
    HIGH==digitalRead(START_BUTTON)
    && UNDEF==currentState
  ) {
    currentState = START;
  } else if(HIGH==digitalRead(START_BUTTON)) {
    // Handle a graceful stop when pressing the start button again while program is running.
    resetUnit();
    currentState = UNDEF;
  }
}

/**
 * Reset the unit,
 */
void resetUnit() {
  Serial.println("Reseting unit");
  closeAllBeerFillerTubes();
  digitalWrite(BEER_BELT_SOL, LOW);
  raiseFillerTubes();
  digitalWrite(CO2_PURGE_SOL, LOW);
  Serial.println("Done resetting unit");
}

/**
 * ***************************************************************************
 * ***************************** MAIN FUNCTIONS ******************************
 * ***************************************************************************
 */

/**
 * Main setup routine.
 */
void setup() {
  Serial.begin(9600);
  setupPins();
  setupFillSensorsTimer();
  resetUnit();
  Serial.println("Press Start Button to proceed");
}

/**
 * The main program loop, where all the magic comes togetger.
 */
void loop() {
  switch(currentState) {
    case START:
      startState();
      break;
    case FILLING:
      fillingState();
      break;
    case STOP:
      stopState();
      break;
    default:
      // Lets do our button reads here.
      readStartButton();
      break;
  }
}
