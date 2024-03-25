#include <M5Core2.h>

enum pmic_t { pmic_unknown = 0, pmic_axp192, pmic_axp2101 };
pmic_t _pmic;

void core2Brightness(uint8_t lvl, bool overdrive = false) {
  if (_pmic == pmic_axp2101) {
    int v = lvl * 100 + 2300;
    if (v < 2300) v = 2300;
    if (overdrive) {
      if (v > 3500) v = 3500;
    } else {
      if (v > 3300) v = 3300;
    }
    if (v == 2300) {
      M5.Axp.axp2101.set_bldo1_on_off(false);
      return;
    } else {
      M5.Axp.axp2101.set_bldo1_on_off(true);
    }
      M5.Axp.axp2101.set_lcd_back_light_voltage(v);
  } else if (_pmic == pmic_axp192) {
    int v = lvl * 25 + 2300;
    if (v < 2300) v = 2300;
    if (overdrive) {
      if (v > 3200) v = 3200;
    } else {
      if (v > 2800) v = 2800;
    }
    if (v == 2300) {
      M5.Axp.SetDCDC3(false);
      return;
    } else {
      M5.Axp.SetDCDC3(true);
    }
    M5.Axp.SetDCVoltage(2, v);
  } else {
    return;
  }
}

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(WHITE);
  uint8_t val = Read8bit(0x03);
  if (val == 0x03) _pmic = pmic_axp192;
  else if (val == 0x4A) _pmic = pmic_axp2101;
  else _pmic = pmic_unknown;
}

void loop() {
  uint8_t maxSafe = _pmic == pmic_axp2101 ? 10 : _pmic == pmic_axp192 ? 20 : 0;

  for (uint8_t b = 0; b <= maxSafe; b++) {
    core2Brightness(b);
    delay(50);
  }

  for (uint8_t b = maxSafe - 1; b > 0; b--) {
    core2Brightness(b);
    delay(50);
  }
}