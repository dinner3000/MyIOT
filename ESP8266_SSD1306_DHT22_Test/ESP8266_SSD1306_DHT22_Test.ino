/**
   The MIT License (MIT)

   Copyright (c) 2016 by Daniel Eichhorn

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

*/

#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN D6     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Include the correct display library
// For a connection via I2C using Wire include
// #include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
// or #include "SH1106.h" alis for `#include "SH1106Wire.h"`
// For a connection via I2C using brzo_i2c (must be installed) include
// #include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Brzo.h"
// #include "SH1106Brzo.h"
// For a connection via SPI include
// #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Spi.h"
// #include "SH1106SPi.h"

// Include custom images
// #include "images.h"

DHT dht(DHTPIN, DHTTYPE);

// Initialize the OLED display using SPI
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
SSD1306Spi        display(D0, D2, D8);
// or
// SH1106Spi         display(D0, D2);

// Initialize the OLED display using brzo_i2c
// D3 -> SDA
// D5 -> SCL
// SSD1306Brzo display(0x3c, D3, D5);
// or
// SH1106Brzo  display(0x3c, D3, D5);

// Initialize the OLED display using Wire library
// SSD1306  display(0x3c, D3, D5);
// SH1106 display(0x3c, D3, D5);

int state = LOW;

void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  Serial.println();
  Serial.println();

  dht.begin();

  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  
//  pinMode(9, OUTPUT); not working
//  pinMode(10, OUTPUT);
//  pinMode(11, OUTPUT); not working
//  pinMode(12, OUTPUT);
//  pinMode(13, OUTPUT); not working
//  pinMode(14, OUTPUT); not working
}

void draw(float h, float t, float f, float hif, float hic) {

  String buf = String(h) + "%," + String(t) + "*C," + String(hic) + "*C";

  display.drawString(0, 2, "Humi: " + String(h) + "%");
  display.drawString(0, 20, "Temp: " + String(t) + "*C");
  display.drawString(0, 40, "Index: " + String(hic) + "*C");
  Serial.println(buf);
}

void loop() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  // clear the display
  display.clear();
  draw(h, t, f, hif, hic);
  // write the buffer to the display
  display.display();

  state = (state == LOW ? HIGH : LOW);
//  digitalWrite(9, state);
/*  digitalWrite(10, state);
  digitalWrite(11, state);
  digitalWrite(12, state);
  digitalWrite(13, state);
  digitalWrite(14, state);
*/
  delay(1000);
}
