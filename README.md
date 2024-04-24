# Arduino Maze Solver Car

## Project Overview
This Arduino Maze Solver Car is designed to autonomously navigate through a maze using a combination of sensors and control algorithms. The car uses an array of sensors including LIDAR, a color sensor, and a compass to detect its surroundings and make navigation decisions.

## Features
- **LIDAR Management**: Detects obstacles and helps in pathfinding.
- **Color Sensor**: Helps in recognizing specific markers or indications on the path.
- **Compass**: Used for orientation to maintain the desired direction.
- **Keypad Manager**: Allows for user input to control various settings.
- **LCD Display**: Provides a user interface to display status and settings.
- **Speed Control**: Adjusts the car's speed based on path conditions.
- **Autonomous Decision Making**: Makes turn decisions based on sensor inputs and pre-defined logic.

## Hardware Requirements
- Arduino Board
- LIDAR Sensor
- RGB Color Sensor
- Digital Compass
- Keypad
- LCD Display (20x4)
- Motor Driver and Motors

## Software Dependencies
- `LiquidCrystal.h` for LCD interface.
- Custom libraries for managing each component:
  - `ColorManager.h`
  - `CompassManager.h`
  - `CarManager.h`
  - `LidarManager.h`
  - `EspManager.h`
  - `KeypadManager.h`

## Setup and Installation
1. **Assemble the Hardware**: Connect all sensors, the LCD, and the keypad to the Arduino according to the circuit diagram (add link to circuit diagram here).
2. **Install Libraries**: Ensure all required libraries are installed in your Arduino IDE. Custom libraries should be placed in the Arduino's libraries folder.
3. **Upload the Code**: Open the provided Arduino sketch and upload it to your Arduino board.

## Usage
- Power on the device and use the keypad to navigate through the initial setup on the LCD.
- Place the car at the start of the maze.
- Use the keypad to start the maze navigation process.
- Monitor the LCD for status updates and sensor readings.

