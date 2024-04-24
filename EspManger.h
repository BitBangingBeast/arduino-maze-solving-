#ifndef ESP_MANAGER_H
#define ESP_MANAGER_H

#include <Wire.h>


class EspManger {


private:
public:
  /*
  void Remote_Control() {
  if (Serial.available() > 0 ) {
    String dire = " ";
    String Buffer_var = Serial.readStringUntil('x');
    String Buffer_var_name = Buffer_var.substring(1, 5);
    if (Buffer_var_name == "dist") {
      
  String Buffer_var_num = Buffer_var.substring(6);
      int dist = Buffer_var_num.toInt();
     
      if (dist < 0) {
        dire = "B";
        dist = -dist;

      } else {
        dire = "A";
      }

      move_to(dist, dire);

    } else if (Buffer_var_name == "dire") {
      Buffer_var = Buffer_var.substring(6);
      rotate(Buffer_var);
    }
  }
}
*/
  void sendDataToESP(int myInt1, int myInt2) {
    // Convert the String to a char array before sending.
    String num1 = String(myInt1);
    String num2 = String(myInt2);
    String data = num1 + "," + num2;
    char buf[data.length() + 1];  // +1 to accommodate the null terminator.
    data.toCharArray(buf, sizeof(buf));

    // Use Serial.write to send the data as a byte array.
    Serial.write(buf, sizeof(buf) - 1);  // -1 to exclude the null terminator from being sent.

    // Explicitly send a Line Feed (LF) character to signify the end of the message.
    Serial.write('\n');
  }

  void sendHexCodeToWeb(String hexCode) {
    String message = "Hex: " + hexCode;

    char buf[message.length() + 1];  // +1 for the null terminator.
    message.toCharArray(buf, sizeof(buf));
    Serial.write(buf, sizeof(buf) - 1);
    Serial.write('\n');
  }
};
#endif
