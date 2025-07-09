# ReRo Lab V1 - Sensor Control Library Documentation

## Project Overview
This project contains sensor control libraries for TOF (Time-of-Flight), IMU (Inertial Measurement Unit), IR sensor arrays, and serial communication for robotics applications.

## Project Structure
```
V1/
├── main/
│   ├── main.ino          # Main Arduino sketch
│   ├── IRArray.h         # IR sensor array class
│   ├── sensors.h         # TOF and IMU sensor classes
│   └── Serial_Comms.h    # Serial communication class
└── README.md             # This documentation file
```

## Header Files Documentation

### 1. IRArray.h
**Purpose:** Manages an array of 8 IR sensors for line following or obstacle detection.

#### Class: `IRArray`
**Constructor:**
```cpp
IRArray(int IR1, int IR2, int IR3, int IR4, int IR5, int IR6, int IR7, int IR8)
```
- **Parameters:** 8 digital pin numbers for IR sensors
- **Description:** Initializes IR sensor array with specified pins and sets pinMode to INPUT

**Methods:**
- `int* getValues()` - Read all 8 IR sensor values and return array pointer
- `int getValue(int index)` - Get specific sensor value by index (0-7)
- `int getSize() const` - Returns array size (always 8)
- `void printValues()` - Print all sensor values to Serial for debugging

**Usage Example:**
```cpp
IRArray irSensors(2, 3, 4, 5, 6, 7, 8, 9);
int* values = irSensors.getValues();
int sensor0 = irSensors.getValue(0);
irSensors.printValues();
```

### 2. sensors.h
**Purpose:** Provides classes for TOF distance sensor and IMU sensor management.

#### Class: `TOFSensor`
**Constructor:**
```cpp
TOFSensor()
```
- **Description:** Initializes VL6180X TOF sensor with default configuration and 500ms timeout

**Methods:**
- `uint16_t getDistance()` - Get distance reading in millimeters
- `bool timeoutOccurred()` - Check if last reading timed out
- `void setTimeout(uint16_t timeout)` - Set timeout value in milliseconds

**Usage Example:**
```cpp
TOFSensor tof;
uint16_t distance = tof.getDistance();
if (!tof.timeoutOccurred()) {
    Serial.println(distance);
}
```

#### Class: `IMUSensor`
**Constructor:**
```cpp
IMUSensor()
```
- **Description:** Initializes DFRobot_BMX160 IMU sensor, halts program if initialization fails

**Methods:**
- `void getAccelData(float &x, float &y, float &z)` - Get accelerometer data
- `void getGyroData(float &x, float &y, float &z)` - Get gyroscope data
- `void getMagData(float &x, float &y, float &z)` - Get magnetometer data
- `void getAllData(...)` - Get all sensor data in one call

**Usage Example:**
```cpp
IMUSensor imu;
float accelX, accelY, accelZ;
imu.getAccelData(accelX, accelY, accelZ);
```

### 3. Serial_Comms.h
**Purpose:** Handles serial communication with structured data packets.

#### Class: `SerialComms`
**Constructor:**
```cpp
SerialComms(unsigned long baud = 115200)
```
- **Parameters:** Baud rate (default: 115200)
- **Description:** Initializes Serial1 communication

**Methods:**
- `void sendTOFData(uint16_t Value)` - Send TOF sensor data with 'T' identifier
- `void sendIMUData(float x, float y, float z)` - Send IMU data with 'I' identifier
- `void sendIRArrayData(int* values, int size)` - Send IR array data with 'R' identifier
- `void send8BitData(int value)` - Send generic 8-bit data with 'W' identifier
- `bool isDataAvailable()` - Check if incoming data available
- `int readData()` - Read incoming byte

**Protocol Structure:**
```
[0xFF] [Data Type] [Payload] [0xFE]
```
- `0xFF` - Start byte
- Data Type: 'T' (TOF), 'I' (IMU), 'R' (IR Array), 'W' (Generic)
- Payload: Actual sensor data
- `0xFE` - End byte

**Usage Example:**
```cpp
SerialComms comms(115200);
comms.sendTOFData(distance);
comms.sendIMUData(x, y, z);
```

## Main Application (main.ino)

### Required Libraries
- `Wire.h` - I2C communication
- `DFRobot_BMX160.h` - IMU sensor library
- `VL6180X.h` - TOF sensor library

### Basic Usage Pattern
```cpp
#include "sensors.h"
#include "Serial_Comms.h"
#include "IRArray.h"

// Create sensor instances
TOFSensor tof;
IMUSensor imu;
IRArray irSensors(2, 3, 4, 5, 6, 7, 8, 9);
SerialComms comms(115200);

void setup() {
    Serial.begin(9600);
    Wire.begin();
    // All sensor initialization happens in constructors
}

void loop() {
    // Read and send sensor data
    uint16_t distance = tof.getDistance();
    comms.sendTOFData(distance);
    
    float accelX, accelY, accelZ;
    imu.getAccelData(accelX, accelY, accelZ);
    comms.sendIMUData(accelX, accelY, accelZ);
    
    int* irValues = irSensors.getValues();
    comms.sendIRArrayData(irValues, irSensors.getSize());
    
    delay(100);
}
```

## Notes
- All classes use constructor initialization - no separate setup functions needed
- Memory management is handled automatically by destructors
- Serial communication uses structured packets for reliable data transmission
- Error handling is built into sensor classes
- All sensor readings are non-blocking except IMU initialization failure
