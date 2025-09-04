#include <TFT_eSPI.h>
#include <Multichannel_Gas_GMXXX.h>
#include <Wire.h>
GAS_GMXXX<TwoWire> gas;

TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);

// Calibration constants from PDF
float GM102B_init = 1.41, GM102B_dV = -1.03, GM102B_ppm = 5;
float GM302B_init = 0.94, GM302B_dV = -0.46, GM302B_ppm = 50;
float GM502B_init = 1.42, GM502B_dV = -0.89, GM502B_ppm = 50;
float GM702B_init = 1.22, GM702B_dV = 0.87, GM702B_ppm = 150;

float calibrate(float raw, float init, float dV, float ppm) {
  return (raw - init) * (ppm / dV);
}

void setup() {
  tft.begin();
  tft.setRotation(3);
  spr.createSprite(tft.width(), tft.height());
  gas.begin(Wire, 0x08);
}

void loop() {
  spr.fillSprite(TFT_BLACK);
  spr.setFreeFont(&FreeSansBoldOblique18pt7b);
  spr.setTextColor(TFT_BLUE);
  spr.drawString("Gas Terminal", 45, 10, 1);

  for (int8_t i = 0; i < 5; i++) {
    spr.drawLine(0, 50 + i, tft.width(), 50 + i, TFT_GREEN);
  }

  spr.setFreeFont(&FreeSansBoldOblique9pt7b);

  // NO2
  float raw = gas.getGM102B() / 100.0;
  float val = calibrate(raw, GM102B_init, GM102B_dV, GM102B_ppm);
  spr.setTextColor(TFT_WHITE);
  spr.drawString("NO2 (ppm):", 36, 76, 1);
  spr.drawRoundRect(36, 100, 80, 40, 5, TFT_WHITE);
  spr.drawNumber((int)val, 40, 110, 1);

  // Ethanol
  raw = gas.getGM302B() / 100.0;
  val = calibrate(raw, GM302B_init, GM302B_dV, GM302B_ppm);
  spr.drawString("C2H5OH (ppm):", 206, 76, 1);
  spr.drawRoundRect(206, 100, 80, 40, 5, TFT_WHITE);
  spr.drawNumber((int)val, 210, 110, 1);

  // VOC
  raw = gas.getGM502B() / 100.0;
  val = calibrate(raw, GM502B_init, GM502B_dV, GM502B_ppm);
  spr.drawString("VOC (ppm):", 36, 156, 1);
  spr.drawRoundRect(36, 180, 80, 40, 5, TFT_WHITE);
  spr.drawNumber((int)val, 40, 190, 1);

  // CO
  raw = gas.getGM702B() / 100.0;
  val = calibrate(raw, GM702B_init, GM702B_dV, GM702B_ppm);
  spr.drawString("CO (ppm):", 206, 156, 1);
  spr.drawRoundRect(206, 180, 80, 40, 5, TFT_WHITE);
  spr.drawNumber((int)val, 210, 190, 1);

  spr.pushSprite(0, 0);
  delay(100);
}