#ifndef COLOR_MANAGER_H
#define COLOR_MANAGER_H

#include "DFRobot_TCS34725.h"
#include "keypadManager.h"
#include <LiquidCrystal.h>
#include <Wire.h>

DFRobot_TCS34725 tcs = DFRobot_TCS34725(&Wire, TCS34725_ADDRESS, TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
uint16_t red, green, blue;
String hexColor;

class ColorManger {

private:



public:

  int redColor[4] = { 0, 0, 0, 0 };
  int blueColor[4] = { 0, 0, 0, 0 };
  int greenColor[4] = { 0, 0, 0, 0 };

  void readColor() {
    uint16_t clear;
    tcs.getRGBC(&red, &green, &blue, &clear);
    tcs.lock();  // Turn off LED or lock the sensor if needed

    uint32_t sum = clear;
    float r, g, b;
    red = (float)red / sum * 255;
    green = (float)green / sum * 255;
    blue = (float)blue / sum * 255;

    // Prepare the hex string
    char hexStr[8];
    sprintf(hexStr, "%02X%02X%02X", (int)red, (int)green, (int)blue);
    hexColor = String(hexStr);

    //Serial.print("\tHex: #"); Serial.println(hexColor);
  }
  int getRed() {
    return (int)red;
  }
  int getBlue() {
    return (int)blue;
  }
  int getGreen() {
    return (int)green;
  }
  void calibrateColor(keypadManager &keyPadManager, LiquidCrystal &lcd) {
    const char *colors[] = { "End Line", "Inside Wall", "Speed Up", "Slow Down", "Next Step" };
    int step = 0;
    Serial.println("statring calibrartion");

    while (step < 5) {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Calibrate your next Color: ");
      lcd.print(colors[step]);
      lcd.setCursor(0, 1);
      lcd.print("# = next ");

      char key = keyPadManager.getKey();


      if (key == '#') {
        if (step < 4) {
          readColor();
          Serial.print("hex code: ");
          Serial.println(hexColor);
          redColor[step] = red;
          blueColor[step] = blue;
          greenColor[step] = green;
        }
        step += 1;
      }
      delay(150);
    }
    lcd.clear();
  }
};
#endif