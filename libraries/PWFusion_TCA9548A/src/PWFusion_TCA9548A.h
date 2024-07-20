/***************************************************************************
* File Name: PWFusion_TCA9548A.h
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
#ifndef PWFUSION_TCA9548A_H
#define PWFUSION_TCA9548A_H

#include <Wire.h>
#include <Arduino.h>

#define CHAN_NONE  0x00
#define CHAN0      0x01
#define CHAN1      0x02
#define CHAN2      0x04
#define CHAN3      0x08
#define CHAN4      0x10
#define CHAN5      0x20
#define CHAN6      0x40
#define CHAN7      0x80


class TCA9548A
{
  public:
    bool begin(uint8_t addr = 0, TwoWire &i2cPort = Wire);
    void setChannel(uint8_t channel);
    uint8_t getChannel();
    
  private:
    TwoWire *_i2cPort;
    uint8_t _addr;
};

#endif // PWFUSION_TCA9548A_H
