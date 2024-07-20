/***************************************************************************
* File Name: PWFusion_TCA9548A.cpp
* Processor/Platform: Arduino Uno R3 (tested)
* Development Environment: Arduino 1.8.13
*
* Designed for use with with Playing With Fusion TCA9548A I2C multiplexer 
* Breakouts: IFB-10011 and IFB-10012
*
*   IFB-10011 8-ch with Qwiic connectors
*   ---> http://www.playingwithfusion.com/productview.php?pdid=113
*
*   IFB-10012 8-ch Compact breakout
*   ---> http://www.playingwithfusion.com/productview.php?pdid=114

* Copyright © 2020 Playing With Fusion, Inc.
* SOFTWARE LICENSE AGREEMENT: This code is released under the MIT License.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
* **************************************************************************
* REVISION HISTORY:
* Author        Date      Comments
* J. Leonard    2020Aug   Original version
* 
* Playing With Fusion, Inc. invests time and resources developing open-source
* code. Please support Playing With Fusion and continued open-source 
* development by buying products from Playing With Fusion!
**************************************************************************/
#include "PWFusion_TCA9548A.h"

#define BASE_ADDR 0x70

// Initialize the TCA9548A driver, where addr is an address between 0 and 7.
bool TCA9548A::begin(uint8_t addr, TwoWire &i2cPort) {
  _addr = BASE_ADDR | addr;
  _i2cPort = &i2cPort;
  return true;
}

// Enable one or more I2C buses.  Multiple buses may be or'd together through
// the channel parameter
void TCA9548A::setChannel(uint8_t channel) {
  _i2cPort->beginTransmission(_addr);
  _i2cPort->write(channel);            
  _i2cPort->endTransmission(); 
}

// Return a bit mask of the currently enabled I2C buses.
uint8_t TCA9548A::getChannel() {
  uint8_t channel = 0;
  
  if (_i2cPort->requestFrom(_addr, (uint8_t)1) != 0)
  {
    channel = _i2cPort->read();
  }

  return channel;
}
