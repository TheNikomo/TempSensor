/*
    Composite Video Temperature Sensor
    Copyright (C) 2014 Niko Montonen 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <Wire.h>
#include <TVout.h>
#include <fontALL.h>

TVout TV;

void setup(){
  TV.begin(PAL,120,96); // start TV output
  TV.select_font(font6x8); // set font size
  TV.print(24, 34, "Temperature"); // print initial text, never draw over it
  Wire.begin(1);                // join i2c bus with address #1
  Wire.onReceive(receiveEvent); // register receive event
}

void loop() {
  // empty loop, we don't need it
}

void receiveEvent(int howMany) //variable is number of bytes read from master
{
  TV.set_cursor(24, 50); // set cursor to required place every time we read temperature
  while(Wire.available()) //while data is available on I2C bus
  {
    char c = Wire.read(); // read data byte into character buffer
    TV.print(c); // print to screen
  }
}
