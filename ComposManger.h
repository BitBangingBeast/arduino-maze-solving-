#ifndef COMPOS_MANAGER_H
#define COMPOS_MANAGER_H

#include <Wire.h>


#define CMPS14_address 0x60

//Motor Pin
#define R_Motor_P_port 9
#define L_Motor_P_port 10
#define R_Motor_D_port 8
#define L_Motor_D_port 7

// direction
#define forwrad 1
#define backward 0


class ComposManger {

private:

  byte R_direction;
  byte L_direction;
public:
  int rotationSpeed;
  int rotationAngle = 1000;
  int angleToGo;
  int direToGo = 1;
  int direToHold;
  int X_coordinate;
  int X_ops_coordinate;
  int Y_coordinate;
  int Y_ops_coordinate;

  void setup() {

    pinMode(R_Motor_P_port, OUTPUT);
    pinMode(L_Motor_P_port, OUTPUT);
    pinMode(R_Motor_D_port, OUTPUT);
    pinMode(L_Motor_D_port, OUTPUT);
  }

  void setXandY(keypadManager &keyPadManager, LiquidCrystal &lcd) {
    const char *coordinate[] = { "X Axes", "Y Axes", "Next Step" };
    int step = 0;
    Serial.println("statring X and Y calibrartion");

    while (step < 3) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Calibrate your next Axes: ");
      lcd.print(coordinate[step]);
      lcd.setCursor(0, 1);
      lcd.print("# = next ");

      char key = keyPadManager.getKey();
      if (key == '#') {
        if (step == 0) {
          X_coordinate = readAngle();
          Serial.println(X_coordinate);
          X_ops_coordinate = calculateOpsAngel(X_coordinate);
          Serial.println(X_ops_coordinate);
        } else if (step == 1) {
          Y_coordinate = readAngle();
          Serial.println(Y_coordinate);

          Y_ops_coordinate = calculateOpsAngel(Y_coordinate);
          Serial.println(Y_ops_coordinate);
        }
        step += 1;
      }
      delay(150);
    }
    lcd.clear();
  }


  int readAngle() {
    Wire.beginTransmission(CMPS14_address);
    Wire.write(1);
    Wire.endTransmission(false);
    Wire.requestFrom(CMPS14_address, 1, true);

    if (Wire.available() >= 1) {
      int raw = Wire.read();
      int angle = map(raw, 0, 255, 0, 360);
      return angle;
    }

    // Return an invalid angle if data is not available
    return -1;
  }


  void setDireToHold(int dire) {
    direToHold = dire;
  }

  int calculateOpsAngel(int value) {
    int result = value + 180;
    if (result < 0) {
      result += 360;
    } else if (result > 360) {
      result -= 360;
    }
    return result;
  }
};
#endif
