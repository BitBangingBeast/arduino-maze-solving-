#include "ColorManger.h"
#include "ComposManger.h"
#include "CarManger.h"
#include "LidarManger.h"
#include "EspManger.h"
#include <LiquidCrystal.h>
#include "keypadManager.h"

ComposManger composManger;
CarManager carManger;
ColorManger colorManger;
LidarManger lidarManger;
keypadManager keyPadManager;

const int rs = 11, en = 12, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int padding = 10;
bool maze = true;
int speed = 50;
int head_to;
int turn_angle;
bool triger_turn = false;
bool triger_turn_color = false;
bool on_turn = false;
int buffer_dist = 15;
int prevouis_dist = -1;
int previousDist = 0;
bool checking = false;

bool Continue;
int angle_in_the_check;
int previous_angle;
int currentDist;
int interval = 10;





void setup() {

  Serial.begin(9600);
  Wire.begin();
  composManger.setup();
  lidarManger.lidarBegin();
  lcd.begin(20, 4);
  colorManger.calibrateColor(keyPadManager, lcd);
  composManger.setXandY(keyPadManager, lcd);
  composManger.direToHold = composManger.X_coordinate;
}

void loop() {

  
  composManger.setDireToHold(composManger.readAngle());
  while (maze) {
  
    colorManger.readColor();
    if (isWithinRange(colorManger.getRed(), colorManger.redColor[0], padding)
        && isWithinRange(colorManger.getGreen(), colorManger.greenColor[0], padding)
        && isWithinRange(colorManger.getBlue(), colorManger.blueColor[0], padding)) {
      Serial.println("Stoping");
      maze = false;
      carManger.stopMoving();
      break;
    } else if (isWithinRange(colorManger.getRed(), colorManger.redColor[1], padding)
               && isWithinRange(colorManger.getGreen(), colorManger.greenColor[1], padding)
               && isWithinRange(colorManger.getBlue(), colorManger.blueColor[1], padding)) {

      if (!checking) {
        Serial.println("triger turning due to colors");
        triger_turn = true;
        backOff();
      }
      triger_turn_color = true;
      Serial.print("triger turn color: ");
      Serial.println(triger_turn_color);
      carManger.stopMoving();
      delay(100);
    } else if (isWithinRange(colorManger.getRed(), colorManger.redColor[2], padding)
               && isWithinRange(colorManger.getGreen(), colorManger.greenColor[2], padding)
               && isWithinRange(colorManger.getBlue(), colorManger.blueColor[2], padding)) {
      speed = 75;
    } else if (isWithinRange(colorManger.getRed(), colorManger.redColor[3], padding)
               && isWithinRange(colorManger.getGreen(), colorManger.greenColor[3], padding)
               && isWithinRange(colorManger.getBlue(), colorManger.blueColor[3], padding)) {
      speed = 25;
    }
    if (lidarManger.getlidarDist() <= buffer_dist) {

      if (!on_turn && !checking) {
        Serial.print("triger turning due to lidar and on turn value: ");
        triger_turn = true;
        carManger.stopMoving();
        delay(100);
      }
    }

    if (triger_turn && !checking) {
      delay(50);
      Serial.println("triger turn");
      triger_turn = false;
      if (!on_turn) {
        Serial.println("triggred turn and we are not on turn");
        on_turn = true;
        if (abs(composManger.readAngle() - composManger.X_coordinate) < 20
            || abs(composManger.readAngle() - composManger.X_ops_coordinate) < 20) {

          head_to = composManger.Y_coordinate;
          composManger.direToHold = head_to;

        } else {
          head_to = composManger.X_coordinate;
          composManger.direToHold = head_to;
        }
        turn_angle = head_to - composManger.readAngle();
        Serial.print("turing setting normal current: ");
        Serial.print(composManger.readAngle());
        Serial.print("  head_to ");
        Serial.print(head_to);
        Serial.print(" turn_angle: ");
        Serial.println(turn_angle);
      } else {
        Serial.println("inside triger turn but we are turning back ");
        turnBack();
      }
    }

    if (on_turn) {
      carManger.rotate(composManger, turn_angle);
      if (abs(composManger.readAngle() - head_to) < 3) {
        on_turn = false;
        carManger.stopMoving();
        delay(200);
        Serial.print("distance after turning:  ");
        Serial.println(lidarManger.getlidarDist());
        Serial.print("dire after turning:  ");
        Serial.println(composManger.readAngle());
        if (lidarManger.getlidarDist() <= 20) {
          Serial.println("suppose to turn back");
          turnBack();
        }
        Serial.println("stop turning");
      }
    } else if ((abs(composManger.readAngle() - composManger.Y_coordinate) < 15
                || abs(composManger.readAngle() - composManger.Y_ops_coordinate) < 15 || checking)
               && !on_turn) {
      int interval = 10;
      if (!checking) {
        Serial.print("setting up and angle");
        previous_angle = head_to;
        angle_in_the_check = (composManger.X_coordinate - composManger.readAngle());
        Serial.print(composManger.readAngle());
        Serial.println(angle_in_the_check);
      }
      currentDist = lidarManger.getlidarDist();

  
      if ((currentDist - previousDist >= interval)) {
        checking = true;
        previousDist = currentDist;
        Continue = true;
        //Serial.println("why are we checking");
      }
      Serial.print("inside the checking and the value of triger color :  ");
      Serial.println(triger_turn_color);
      if (triger_turn_color || triger_turn) {
        Continue = false;
        carManger.stopMoving();
        Serial.print("continue value");
        Serial.println(Continue);
      } else if (abs(composManger.readAngle() - composManger.X_coordinate) < 3) {
        head_to = composManger.X_coordinate;
        composManger.direToHold = head_to;
        checking = false;
        Continue = true;
      }
      if (Continue) {
        carManger.rotate(composManger, angle_in_the_check);
      } else {
        angle_in_the_check = (composManger.readAngle() - previous_angle);
        carManger.rotate(composManger, angle_in_the_check);
        if (abs(composManger.readAngle() - previous_angle) < 3) {
          checking = false;
          Continue = true;
          carManger.stopMoving();
        }
      }
      triger_turn_color = false;
    } else if (!checking) {
      carManger.Move(composManger, speed);
    }
  }
}

bool isWithinRange(int value, int target, int padding) {
  return value >= (target - padding) && value <= (target + padding);
}



void checkForOpening() {
  int interval = 10;
  if (!checking) {
    int currentDist = lidarManger.getlidarDist();
    previous_angle = head_to;
    if (head_to == composManger.Y_coordinate) {
      angle_in_the_check = (composManger.X_coordinate - composManger.readAngle());
    } else {
      angle_in_the_check = (composManger.readAngle() - composManger.X_coordinate);
    }
  }
  /*
  Serial.print("current: ");
  Serial.println(composManger.readAngle());
  Serial.print("turn angle:  ");
  Serial.println(angle_in_the_check);
  */
  if (currentDist - previousDist >= interval) {
    checking = true;
    previousDist = currentDist;
    Serial.println("why are we checking");
    if (triger_turn_color) {
      Serial.println("should stop");
      Continue = false;
    } else if (abs(composManger.readAngle() - composManger.X_coordinate) < 3) {
      head_to = composManger.X_coordinate;
      composManger.direToHold = head_to;
      checking = false;
      Continue = true;
    }
  }
  if (!Continue) {
    angle_in_the_check = (composManger.readAngle() - previous_angle);
    carManger.rotate(composManger, angle_in_the_check);
    if (abs(composManger.readAngle() - previous_angle) < 3) {
      checking = false;
      Continue = true;
    }
  }
}


void turnBack() {
  Serial.print("turning back: was heading to ");
  Serial.println(head_to);
  if (turn_angle < 0) {
    turn_angle = (head_to + 180 - composManger.readAngle());
  } else {
    turn_angle = -(360 - (head_to + 180 - composManger.readAngle()));
  }
  Serial.println(turn_angle);

  if (head_to == composManger.Y_coordinate) {
    head_to = composManger.Y_ops_coordinate;
    composManger.direToHold = head_to;
  } else if (head_to == composManger.Y_ops_coordinate) {
    head_to = composManger.Y_coordinate;
    composManger.direToHold = head_to;
  } else if (head_to == composManger.X_ops_coordinate) {
    head_to = composManger.X_coordinate;
    composManger.direToHold = head_to;
  } else {
    head_to = composManger.X_ops_coordinate;
    composManger.direToHold = head_to;
  }
  Serial.print("head_to ");
  Serial.println(head_to);
  on_turn = true;
}

void backOff() {
  Serial.println("backing off");
  int current = lidarManger.getlidarDist();
  int end = lidarManger.getlidarDist() + 5;
  composManger.direToGo = 0;
  delay(150);
  while (current < end) {
    current = lidarManger.getlidarDist();
    //Serial.print("current dist: ");
    //Serial.println(current);
    carManger.Move(composManger, 40);
  }
  carManger.stopMoving();
  composManger.direToGo = 1;
  delay(150);
}
