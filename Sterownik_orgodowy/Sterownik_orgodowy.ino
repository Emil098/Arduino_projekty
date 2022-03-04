#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
//#include <TouchScreen.h>
#include "DHTStable.h"
#include "i2c.h"
#include "ds1307.h"
#include "menu_lib.h"

#define DHT22_PIN A5

#define YP A3
#define XM A2
#define YM 9
#define XP 8

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
i2c_class i2c;
ds_class ds;
DHTStable DHT;
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
screen ili9341;
ds_val DateNtime = {0}, last = {0};
screen_var recent_var;
menu_state btn_state;
ds_class ds1307;
float lastHum = 0, recHum, lastTemp = 0, recTemp;

void setup(void) {

  tft = ili9341.initScreen(tft);
  ili9341.print_main_screen();
  btn_state.screen_state = MAIN_SCREEN;
  pinMode(1, OUTPUT);
  digitalWrite(1,1);
  pinMode(10, OUTPUT);
  DateNtime = ds.readAll();
  if (DateNtime.year < 21) {
    ds1307.setSingleReg(yearReg, (2 << 4) | 1);
    btn_state.time.year = 21;
  }
}

void loop()
{
  int chk = DHT.read22(DHT22_PIN);
  float hum = DHT.getHumidity();
  float temp = DHT.getTemperature();

  DateNtime = ds.readAll();

  if ( hum > 0 ) {
    recHum = hum;
    recTemp = temp;
  }

  if (btn_state.screen_state == MAIN_SCREEN) {
    recent_var.Hum = recHum;
    recent_var.Temp = recTemp;
    recent_var.dateVar = DateNtime;
    ili9341.updateMainScreen(recent_var);
  }

  if (btn_state.pump_val.in_use ==  PUMP_WAITING_ST && DateNtime.mins >= btn_state.pump_val.start_time.mins && DateNtime.hour >= btn_state.pump_val.start_time.hour) {
    btn_state.pump_val.in_use = PUMP_TURN_ON;
    SET_PUMP_ON;
  }
  if (btn_state.pump_val.in_use ==  PUMP_TURN_ON && DateNtime.mins >= btn_state.pump_val.end_time.mins && DateNtime.hour >= btn_state.pump_val.end_time.hour) {
    btn_state.pump_val.in_use = PUMP_TURN_OFF;
    SET_PUMP_OFF;
  }

  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > 10 && p.z < 1000) {
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    p.x = 250 - p.x;
    btn_state.px = p.x;
    btn_state.py = p.y;
    btn_state = ili9341.check_btn_routine(btn_state);
  }
}
