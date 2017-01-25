#ifndef PANTENGGXRAWDATA_H
#define PANTENGGXRAWDATA_H

#include <SoftwareSerial.h>

struct PantengGxRawData {
public:
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
public:
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

typedef void (*PSGCallBack)(void*);

class PantengSenserGx{
  public:
    PantengSenserGx(SoftwareSerial* uart, PSGCallBack on_data_ready = NULL);
	~PantengSenserGx();
	void Read();
//    bool IsDataReady();
    //int getDensity_PM1();
    //int getDensity_PM2_5();
    //int getDensity_PM10();
    //int getCFDensity_PM1_0();
    //int getCFDensity_PM2_5();
    //int getCFDensity_PM10();
    //int getCount_PM0_3();
    //int getCount_PM0_5();
    //int getCount_PM1();
    //int getCount_PM2_5();
    //int getCount_PM5();
    //int getCount_PM10();
    void getDataText(char* text);

  private:
    void formatData();
    
    SoftwareSerial* _uart = NULL;
    PantengGxRawData* _data = NULL;
    PantengGxFormatedData* _fdata;
    
    int _dataState = 0;
    int _dataBytes = 0;
//    bool _dataReady = false;
    PSGCallBack _onDataReady;
};

#endif
