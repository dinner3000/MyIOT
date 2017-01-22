#ifndef RALEY_H
#define RELAY_H

struct PantengGxRawData {
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
};

struct PantengGxFormatedData {
  unsigned int pm1_ug_cf;
  unsigned int pm2_5_ug_cf;
  unsigned int pm10_ug_cf;
  unsigned int pm1_ug;
  unsigned int pm2_5_ug;
  unsigned int pm10_ug;
  unsigned int pm0_3_cnt;
  unsigned int pm0_5_cnt;
  unsigned int pm1_cnt;
  unsigned int pm2_5_cnt;
  unsigned int pm5_cnt;
  unsigned int pm10_cnt;
};

typedef void (*FPPSGCB)(void);

class PantengSenserGx{
  public:
    PantengSenserGx(SoftwareSerial*uart, void (*on_data_ready)(void));
    void Read();
//    bool IsDataReady();
    int getDensity_PM1();
    int getDensity_PM2_5();
    int getDensity_PM10();
    int getCFDensity_PM1_0();
    int getCFDensity_PM2_5();
    int getCFDensity_PM10();
    int getCount_PM0_3();
    int getCount_PM0_5();
    int getCount_PM1();
    int getCount_PM2_5();
    int getCount_PM5();
    int getCount_PM10();
    void char* getDataText();

  private:
    voit formatData();
    SoftwareSerial* _uart;
    PantengGxRawData* _data;
    PantengGxFormatedData* _fdata;
    int _dataState = 0;
    int _dataBytes = 0;
//    bool _dataReady = false;
    void (*_onDataReady)(void);
}

#endif
