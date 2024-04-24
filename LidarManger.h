#ifndef LIDAR_MANAGER_H
#define LIDAR_MANAGER_H

#include "LIDARLite_v4LED.h"
#include <Wire.h>

LIDARLite_v4LED myLIDAR;

class LidarManger {

private:
public:

  bool lidarBegin() {
    return (myLIDAR.begin());
  }

  int getlidarDist() {
    return (myLIDAR.getDistance());
  }


  int calculate_dist(int starting_point) {
    int distance = starting_point - myLIDAR.getDistance();

    if (distance < 0) {
      distance = -distance;
    }
    return distance;
  }
};
#endif