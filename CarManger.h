#ifndef CAR_MANAGER_H
#define CAR_MANAGER_H

#include <Wire.h>
#include <EEPROM.h>
#include "LidarManger.h"

// Motor Pin
#define R_Motor_P_port 9
#define L_Motor_P_port 10
#define R_Motor_D_port 8
#define L_Motor_D_port 7

// Encoder Pin
#define Channel_A 2
#define Channel_B 3

// Direction
#define forward 1  // corrected spelling from 'forwrad' to 'forward'
#define backward 0

// R for right -- L for left -- I for impulse.
static volatile int Channel_L_I_counter;
static volatile int Channel_R_I_counter;

class CarManager {
private:
  // Time buffer variable.
  int interval = 1000;
  int time_passed = 0;

  // Coefficient for movement over X and Y axes.
  int X_coefficient = 0;
  int Y_coefficient = 0;

  // Left and right motor power.
  int L_Motor_P = 0;
  int R_Motor_P = 0;

  // Speed
  int speed = 0;

  //rotation
  int currentAngle;
  int rotationSpeed;
  byte R_direction;
  byte L_direction;
  int rotationAngle = 1000;

  // encoder settings
  byte impulse_per_cm;



public:
  static void setupInterrupt() {
    pinMode(Channel_A, INPUT);
    pinMode(Channel_B, INPUT);
    attachInterrupt(digitalPinToInterrupt(Channel_A), Channel_A_Encoder, RISING);
    attachInterrupt(digitalPinToInterrupt(Channel_B), Channel_B_Encoder, RISING);
  }
  static void Channel_A_Encoder() {

    Channel_L_I_counter++;
  }
  static void Channel_B_Encoder() {
    Channel_R_I_counter++;
  }
  byte getImpulsePerCm() {
    return impulse_per_cm;
  }

  byte calibrate(ComposManger &compos, LidarManger &lidar) {
    int start_value = Channel_L_I_counter;

    int startingPoint = lidar.getlidarDist();
    if (startingPoint - 20 > 5) {
      int endPoint = startingPoint - 20;
      compos.setDireToHold(compos.readAngle());
      while (lidar.getlidarDist() > endPoint && ((lidar.getlidarDist() - endPoint) != 65535)) {
        if ((lidar.getlidarDist() - endPoint) > 5) {

          Move(compos, 140);

        } else {

          Move(compos, 30);
        }
      }
    }
    return ((Channel_L_I_counter - start_value) / 20);
  }

  void writeSavedValue(byte value, int EEPROM_ADDRESS) {
    EEPROM.update(EEPROM_ADDRESS, value);
  }

  byte readSavedValue(int EEPROM_ADDRESS) {
    return EEPROM.read(EEPROM_ADDRESS);
  }

  void calibrateEncoder(ComposManger &compos, LidarManger &lidar) {
    int address = 0;
    byte value = calibrate(compos, lidar);
    if (value > 5) {
      writeSavedValue(value, value);
    }
    impulse_per_cm = readSavedValue(address);
  }

  void Move(ComposManger &compos, int speed) {

    if (compos.direToGo == 1) {
      digitalWrite(R_Motor_D_port, backward);
      digitalWrite(L_Motor_D_port, forward);
    } else {
      digitalWrite(R_Motor_D_port, forward);
      digitalWrite(L_Motor_D_port, backward);
    }

    X_coefficient = (speed * 240) / 100;
    Y_coefficient = (speed * 240) / 100;

    if ((compos.direToHold - compos.readAngle()) > 2) {
      if (abs(compos.direToHold - compos.readAngle()) > 180) {
        Y_coefficient *= 0.8;
      } else {
        X_coefficient *= 0.8;
      }
    } else if ((compos.direToHold - compos.readAngle()) < -2) {
      if (abs(compos.direToHold - compos.readAngle()) > 180) {
        X_coefficient *= 0.8;
      } else {
        Y_coefficient *= 0.8;
      }
    }
    analogWrite(R_Motor_P_port, X_coefficient);
    analogWrite(L_Motor_P_port, Y_coefficient);
  }


  void stopMoving() {
    analogWrite(R_Motor_P_port, 0);
    analogWrite(L_Motor_P_port, 0);
  }

  void rotate(ComposManger &compos, int rotateAngle) {

    currentAngle = compos.readAngle();
    if (rotateAngle != 0) {
      if (rotateAngle != rotationAngle) {
        rotationAngle = rotateAngle;
        compos.angleToGo = rotateAngle;
        //Serial.print("angle to go from inside the rotate function current angel // before calculation:");
        //Serial.println(currentAngle);
        if (rotateAngle > 0) {
          R_direction = backward;
          L_direction = backward;
        } else {
          R_direction = forwrad;
          L_direction = forwrad;
        }
        compos.angleToGo += currentAngle;
        if (compos.angleToGo > 360) {
          compos.angleToGo -= 360;
        } else if (compos.angleToGo < 0) {
          compos.angleToGo += 360;
        }
        //Serial.print("angle to go from inside the rotate function after calculation: ");
        //Serial.println(compos.angleToGo);
      }
      if (abs(currentAngle - compos.angleToGo) > 15) {
        rotationSpeed = 120;

        digitalWrite(R_Motor_D_port, R_direction);
        digitalWrite(L_Motor_D_port, L_direction);
        analogWrite(R_Motor_P_port, rotationSpeed);
        analogWrite(L_Motor_P_port, rotationSpeed);
      }

      if (abs(currentAngle - compos.angleToGo) > 2 && abs(currentAngle - compos.angleToGo) < 15) {
        if ((currentAngle > compos.angleToGo)) {
          R_direction = forwrad;
          L_direction = forwrad;
          rotationSpeed = 85;

        } else {
          R_direction = backward;
          L_direction = backward;
          rotationSpeed = 85;
        }
        digitalWrite(R_Motor_D_port, R_direction);
        digitalWrite(L_Motor_D_port, L_direction);
        analogWrite(R_Motor_P_port, rotationSpeed);
        analogWrite(L_Motor_P_port, rotationSpeed);
      }
      /*
      if (abs(currentAngle - compos.angleToGo) < 2) {
        Serial.println("stop from the car manger");
        stopMoving();
      }
      */
    }
  }
};

#endif
