
#include "PantengSenserGx.h"

PantengSenserGx::PantengSenserGx(SoftwareSerial* uart, void (*on_data_ready)(void)) {
  this->_uart = uart;
  this->_onDataReady = on_data_ready;
  this->_data = new PantengGxRawData();
  this->_fdata = new PantengGxFormatedData();
}

PantengSenserGx::~PantengSenserGx() {
  delete this->_uart;
  this->_uart = null;
  delete this->_data;
  this->_data = null;
  delete this->_fdata;
  this->_fdata = null;
}

PantengSenserGx::Read() {
  unsigned char c = 0;
  if (this->_uart->available()) {
    c = this->_uart->read();
    switch (this->_dataState) {
      case 0:
        if (0x42 == c)
          this->_dataState = 1;
        break;
      case 1:
        if (0x4d == c) {
          this->_dataState = 2;
          this->_dataBytes = 0;
        }
        break;
      case 2:
        ((unsigned char *)this->_data)[this->_dataBytes] = c;
        this->_dataBytes++;
        if (this->_dataBytes > 28) {
          this->_dataState = 0;
          this->formatDta();
          (*this->_onDataReady)();
          break;
        }
        break;
      default:
        break;
    }
  }
}

void PantengSenserGx::formatData() {
  this->_fdata->pm1_ug_cf = this->_data->pm1_ug_cf[0] * 256 + this->_data->pm1_ug_cf[1];
  this->_fdata->pm2_5_ug_cf = this->_data->pm2_5_ug_cf[0] * 256 + this->_data->pm2_5_ug_cf[1];
  this->_fdata->pm10_ug_cf = this->_data->pm10_ug_cf[0] * 256 + this->_data->pm10_ug_cf[1];
  this->_fdata->pm1_ug = this->_data->pm1_ug[0] * 256 + this->_data->pm1_ug[1];
  this->_fdata->pm2_5_ug = this->_data->pm2_5_ug[0] * 256 + this->_data->pm2_5_ug[1];
  this->_fdata->pm10_ug = this->_data->pm10_ug[0] * 256 + this->_data->pm10_ug[1];

  this->_fdata->pm0_3_cnt = this->_data->pm0_3_cnt[0] * 256 + this->_data->pm0_3_cnt[1];
  this->_fdata->pm0_5_cnt = this->_data->pm0_5_cnt[0] * 256 + this->_data->pm0_5_cnt[1];
  this->_fdata->pm1_cnt = this->_data->pm1_cnt[0] * 256 + this->_data->pm1_cnt[1];
  this->_fdata->pm2_5_cnt = this->_data->pm2_5_cnt[0] * 256 + this->_data->pm2_5_cnt[1];
  this->_fdata->pm5_cnt = this->_data->pm5_cnt[0] * 256 + this->_data->pm5_cnt[1];
  this->_fdata->pm10_cnt = this->_data->pm10_cnt[0] * 256 + this->_data->pm10_cnt[1];
}

int PantengSenserGx::getDensity_PM1() {
  return this->_fdata_pm1_ug;
}
int PantengSenserGx::getDensity_PM2_5() {
  return this->_fdata_pm2_5_ug;
}
int PantengSenserGx::getDensity_PM10() {
  return this->_fdata_pm10_ug;
}
int PantengSenserGx::getCFDensity_PM1_0() {
  return this->_fdata_pm1_ug_cf;
}
int PantengSenserGx::getCFDensity_PM2_5() {
  return this->_fdata_pm2_5_ug_cf;
}
int PantengSenserGx::getCFDensity_PM10() {
  return this->_fdata_pm10_ug_cf;
}
int PantengSenserGx::getCount_PM0_3() {
  return this->_fdata->pm0_3_cnt;
}
int PantengSenserGx::getCount_PM0_5() {
  return this->_fdata->pm0_5_cnt;
}
int PantengSenserGx::getCount_PM1() {
  return this->_fdata->pm1_cnt;
}
int PantengSenserGx::getCount_PM2_5() {
  return this->_fdata->pm2_5_cnt;
}
int PantengSenserGx::getCount_PM5() {
  return this->_fdata->pm5_cnt;
}
int PantengSenserGx::getCount_PM10() {
  return this->_fdata->pm10_cnt;
}

void PantengSenserGx::getDataText(char* text) {
  sprintf(text, "Density(ug/m3): %d, %d, %d, %d(cf), %d(cf), %d(cf)\n",
          pm_ug[0], pm_ug[1], pm_ug[2], pm_ug[3], pm_ug[4], pm_ug[5]);
  sprintf(text, "Count: %d(>0.3), %d(>0.5), %d(>1.0), %d(>2.5), %d(>5), %d(>10)\n",
          pm_cnt[0], pm_cnt[1], pm_cnt[2], pm_cnt[3], pm_cnt[4], pm_cnt[5]);
  return text;
}

