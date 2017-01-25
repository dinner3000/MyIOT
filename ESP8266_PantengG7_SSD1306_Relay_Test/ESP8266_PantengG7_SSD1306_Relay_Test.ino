#include <SoftwareSerial.h>
#include <SSD1306Spi.h>
#include "SimpleTimer.h"
#include "PantengSenserGx.h"
#include "Relay.h"

void DisplayToOLed(PantengGxFormatedData* data);
void ProcessPMData(void* p);

// Initialize the OLED display using SPI
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
SSD1306Spi        display(D0, D2, D8);

SimpleTimer timer;

Relay relay1(D4, Off);
Relay relay2(D6, Off);

SoftwareSerial mySerial(D1, D3);
PantengSenserGx psg(&mySerial, ProcessPMData);

char strBuf[150];

void DisplayToOLed(PantengGxFormatedData* data) {

	display.clear();

	sprintf(strBuf, "PM10: %d, %d(cf)", data->pm10_ug, data->pm10_ug_cf);
	display.drawString(0, 2, strBuf);
	sprintf(strBuf, "PM2.5: %d, %d(cf)", data->pm2_5_ug, data->pm2_5_ug_cf);
	display.drawString(0, 20, strBuf);
	sprintf(strBuf, "PM1.0: %d, %d(cf)", data->pm1_ug, data->pm1_ug_cf);
	display.drawString(0, 40, strBuf);

	display.display();
}

void ProcessPMData(void* p) {
	PantengGxFormatedData* data = (PantengGxFormatedData*)p;
	DisplayToOLed(data);
	timer.run(p);
}


void SetRelayState(void* p) {
	
	static const float _VAL_AVG_LOW = 30; //Trigger relay to turn off
	static const float _VAL_AVG_HIGH = 50; //Trigger relay to turn on
	static const int _VAL_HIST_LENGTH = 6;
	static int val_hist[_VAL_HIST_LENGTH];
	static int val_hist_idx = 0;

	PantengGxFormatedData* data = (PantengGxFormatedData*)p;
	
	//Collect value
	val_hist[val_hist_idx] = data->pm2_5_ug;
	sprintf(strBuf, "PM2.5 value collected: %d", data->pm2_5_ug);
	Serial.println(strBuf);
	val_hist_idx++;
	if (val_hist_idx >= _VAL_HIST_LENGTH) {
		val_hist_idx = 0;
	}

	//Calculate AVG value
	int sum = 0;
	for (int i = 0; i < _VAL_HIST_LENGTH; i++) {
		sum += val_hist[i];
	}
	int avg = sum / _VAL_HIST_LENGTH;
	sprintf(strBuf, "PM2.5 AVG value: %d", avg);

	if (avg > _VAL_AVG_HIGH) {
		relay1.TurnOn();
		relay2.TurnOn();
		sprintf(strBuf, "%s, %s", strBuf, "set relay state => On");
	}
	else if (avg <= _VAL_AVG_LOW) {
		relay1.TurnOff();
		relay2.TurnOff();
		sprintf(strBuf, "%s, %s", strBuf, "set relay state => Off");
	}
	else {

	}
	Serial.println(strBuf);

}

void setup()
{
	// Open serial communications and wait for port to open:
	Serial.begin(9600);

	// set the data rate for the SoftwareSerial port
	mySerial.begin(9600);

	// Initialising the UI will init the display too.
	display.init();
	display.flipScreenVertically();
	display.setTextAlignment(TEXT_ALIGN_LEFT);
	display.setFont(ArialMT_Plain_16);

	timer.setInterval(5000, SetRelayState);

  pinMode(D4, OUTPUT);
  pinMode(D6, OUTPUT);
}


void loop() // run over and over
{
	psg.Read();
}
