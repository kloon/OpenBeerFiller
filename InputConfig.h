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
 * Pin definitions
 */
const int BEER_INLET_SOL[] = {5, 6, 7}; //Pin numbers of the inlet solenoids
const byte BEER_FILL_SENSOR[] = {A0, A1, A2}; //Pin numbers of the fill sensors
#define START_BUTTON 8
#define CO2_PURGE_SOL 4
#define FILL_RAIL_SOL 3
#define BEER_BELT_SOL 2
