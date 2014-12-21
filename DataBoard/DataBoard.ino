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
#include <OneWire.h>

OneWire ds(11);

void setup()
{
  Wire.begin(); //init I2C communications
}

void loop(){
  //init required values for temperature math
  int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
  byte i;
  byte present = 0;
  byte addr[8];
  byte data[12];
  int target;
  String temperature;
  char tempCharArray[16];

  if (!ds.search(addr)){
    ds.reset_search();
    return;
  }
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // Start temperature measurement
  
  delay(1000); // wait 1 second for sensor to measure temperature
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // read value from device

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most significant bit
  
  if (SignBit) // iff negative
  {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25

  Whole = Tc_100 / 100;  // separate off the whole and fractional portions
  Fract = Tc_100 % 100;
  
  
  //Build a String object for easy char array concatenate
  temperature = String("");
  if (SignBit){ // If negative
     temperature += String("-");
  }
  temperature += String(Whole);
  temperature += String(".");
  if (Fract < 10){
    temperature += String("0");
  }
  temperature += String(Fract);
  temperature += String (" C  ");
  temperature.toCharArray(tempCharArray, 16);
  
  Wire.beginTransmission(1); // transmit to device #1
  Wire.write(tempCharArray);        // sends char array to device #1
  Wire.endTransmission();    // stop transmitting

}
