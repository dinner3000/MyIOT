#include "SoftwareSerial.h"
#include "SSD1306Spi.h"

struct _panteng {
  unsigned char fixed[2];
  unsigned char pm1_ug_cf[2];
  unsigned char pm2_5_ug_cf[2];
  unsigned char pm10_ug_cf[2];
  unsigned char pm1_ug[2];
  unsigned char pm2_5_ug[2];
  unsigned char pm10_ug[2];
  unsigned char pm0_3_cnt[2];
  unsigned char pm0_5_cnt[2];
  unsigned char pm1_cnt[2];
  unsigned char pm2_5_cnt[2];
  unsigned char pm5_cnt[2];
  unsigned char pm10_cnt[2];
  unsigned char reverved[2];
  unsigned char crc[2];
} panteng;

SoftwareSerial mySerial(D1, D2); // RX, TX

// Initialize the OLED display using SPI
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
SSD1306Spi        display(D0, D2, D8);

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
}

void loop() // run over and over
{
  unsigned char c;
  char str[100];
  static int state = 0;
  static int count = 0;
  static int time = 0;
  int pm_ug[3];        //PM1.0, 2.5, 10 in ug/cm3
  int pm_cnt[6];        //PM0.3, 0.5, 1.0, 2.5, 5, 10 in count
  int i;

  if (mySerial.available()) {
    c = mySerial.read();
    switch (state) {
      case 0:
        if (0x42 == c)
          state = 1;
        break;
      case 1:
        if (0x4d == c) {
          state = 2;
          count = 0;
        }
        break;
      case 2:
        ((unsigned char *) &panteng)[count] = c;
        count++;
        if (count > 28) {
          state = 0;
          pm_ug[0] = panteng.pm1_ug[0] * 256 + panteng.pm1_ug[1];
          pm_ug[1] = panteng.pm2_5_ug[0] * 256 + panteng.pm2_5_ug[1];
          pm_ug[2] = panteng.pm10_ug[0] * 256 + panteng.pm10_ug[1];
          
          pm_cnt[0] = panteng.pm0_3_cnt[0] * 256 + panteng.pm0_3_cnt[1];
          pm_cnt[1] = panteng.pm0_5_cnt[0] * 256 + panteng.pm0_5_cnt[1];
          pm_cnt[2] = panteng.pm1_cnt[0] * 256 + panteng.pm1_cnt[1];
          pm_cnt[3] = panteng.pm2_5_cnt[0] * 256 + panteng.pm2_5_cnt[1];
          pm_cnt[4] = panteng.pm5_cnt[0] * 256 + panteng.pm5_cnt[1];
          pm_cnt[5] = panteng.pm10_cnt[0] * 256 + panteng.pm10_cnt[1];

          sprintf(str, "%d: PM1.0: %d ug/m3, PM2.5: %d ug/m3, PM10: %d ug/m3; %d(>0.3), %d(>0.5), %d(>1.0), %d(>2.5), %d(>5), %d(>10)", time++, pm_ug[0], pm_ug[1], pm_ug[2], pm_cnt[0], pm_cnt[1], pm_cnt[2], pm_cnt[3], pm_cnt[4], pm_cnt[5]);
          Serial.println(str);
          break;
        }
        break;
      default:
        break;
    }
  }
}
