#ifndef SENSOR_FUNCTIONS_H
#define SENSOR_FUNCTIONS_H

#include <Arduino.h>
#include "DFRobot_BMX160.h"
#include "VL6180X.h"
#include "IRArray.h"

class TOFSensor {
private:
    VL6180X sensor;
    
public:
    // Constructor - initializes TOF sensor
    TOFSensor() {
        sensor.init();
        sensor.configureDefault();
        sensor.setTimeout(500);
    }
    
    // Get TOF distance reading
    uint16_t getDistance() {
        return sensor.readRangeSingleMillimeters();
    }
    
    // Check if timeout occurred
    bool timeoutOccurred() {
        return sensor.timeoutOccurred();
    }
    
    // Set timeout value
    void setTimeout(uint16_t timeout) {
        sensor.setTimeout(timeout);
    }
};

class IMUSensor {
private:
    DFRobot_BMX160 sensor;
    
public:
    // Constructor - initializes IMU sensor
    IMUSensor() {
        if (sensor.begin() != true) {
            Serial.println("IMU init false");
            while(1);
        }
    }
    
    // Get accelerometer data
    void getAccelData(float &x, float &y, float &z) {
        sBmx160SensorData_t accelData;
        sensor.getAllData(NULL, NULL, &accelData);
        x = accelData.x;
        y = accelData.y;
        z = accelData.z;
    }
    
    // Get gyroscope data
    void getGyroData(float &x, float &y, float &z) {
        sBmx160SensorData_t gyroData;
        sensor.getAllData(NULL, &gyroData, NULL);
        x = gyroData.x;
        y = gyroData.y;
        z = gyroData.z;
    }
    
    // Get magnetometer data
    void getMagData(float &x, float &y, float &z) {
        sBmx160SensorData_t magData;
        sensor.getAllData(&magData, NULL, NULL);
        x = magData.x;
        y = magData.y;
        z = magData.z;
    }
    
    // Get all sensor data at once
    void getAllData(float &magX, float &magY, float &magZ,
                    float &gyroX, float &gyroY, float &gyroZ,
                    float &accelX, float &accelY, float &accelZ) {
        sBmx160SensorData_t magData, gyroData, accelData;
        sensor.getAllData(&magData, &gyroData, &accelData);
        
        magX = magData.x; magY = magData.y; magZ = magData.z;
        gyroX = gyroData.x; gyroY = gyroData.y; gyroZ = gyroData.z;
        accelX = accelData.x; accelY = accelData.y; accelZ = accelData.z;
    }
};

#endif