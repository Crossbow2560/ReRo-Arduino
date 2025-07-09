#include <Wire.h>
#include "sensors.h"
#include "Serial_Comms.h"
#include "EncoderTimer.h"

// Create sensor instances globally
TOFSensor tof;
IMUSensor imu;
IRArray irarray(5, 6, 7, 8, 9, 10, 11, 12);
SerialComms comms(115200);

void setup(){
    Serial.begin(9600);
    Wire.begin();
    
    Serial.println("=== ReRo Lab V1 - Sensor Demo ===");
    Serial.println("All sensors initialized successfully!");
    
    // Test IR Array initialization
    Serial.print("IR Array size: ");
    Serial.println(irarray.getSize());
    
    // Test initial sensor readings
    Serial.println("\nInitial sensor readings:");
    
    // TOF sensor test
    uint16_t distance = tof.getDistance();
    Serial.print("TOF Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
    
    // IR Array test
    Serial.println("IR Array values:");
    irarray.printValues();
    
    delay(1000);
}

void loop(){
    Serial.println("\n--- Sensor Reading Cycle ---");
    
    // 1. TOF Sensor Usage Example
    uint16_t distance = tof.getDistance();
    if (!tof.timeoutOccurred()) {
        Serial.print("TOF Distance: ");
        Serial.print(distance);
        Serial.println(" mm");
        comms.sendTOFData(distance);
    } else {
        Serial.println("TOF timeout occurred!");
    }
    
    // 2. IMU Sensor Usage Examples
    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;
    float magX, magY, magZ;
    
    // Get accelerometer data
    imu.getAccelData(accelX, accelY, accelZ);
    Serial.print("Accel - X: ");
    Serial.print(accelX);
    Serial.print(", Y: ");
    Serial.print(accelY);
    Serial.print(", Z: ");
    Serial.println(accelZ);
    comms.sendIMUData(accelX, accelY, accelZ);
    
    // Get gyroscope data
    imu.getGyroData(gyroX, gyroY, gyroZ);
    Serial.print("Gyro - X: ");
    Serial.print(gyroX);
    Serial.print(", Y: ");
    Serial.print(gyroY);
    Serial.print(", Z: ");
    Serial.println(gyroZ);
    
    // Get magnetometer data
    imu.getMagData(magX, magY, magZ);
    Serial.print("Mag - X: ");
    Serial.print(magX);
    Serial.print(", Y: ");
    Serial.print(magY);
    Serial.print(", Z: ");
    Serial.println(magZ);
    
    // 3. IR Array Usage Examples
    // Get all IR values
    int* irValues = irarray.getValues();
    Serial.print("IR Array: ");
    for(int i = 0; i < irarray.getSize(); i++) {
        Serial.print(irValues[i]);
        if(i < irarray.getSize() - 1) Serial.print(", ");
    }
    Serial.println();
    
    // Alternative: Print using built-in method
    Serial.print("IR Array (formatted): ");
    irarray.printValues();
    
    // Get individual sensor values
    Serial.print("Individual IR readings - IR[0]: ");
    Serial.print(irarray.getValue(0));
    Serial.print(", IR[3]: ");
    Serial.print(irarray.getValue(3));
    Serial.print(", IR[7]: ");
    Serial.println(irarray.getValue(7));
    
    // Send IR array data via serial
    comms.sendIRArrayData(irValues, irarray.getSize());
    
    // 4. Serial Communication Examples
    // Send custom 8-bit data
    comms.send8BitData(distance > 100 ? 1 : 0);
    
    // Check for incoming data
    if (comms.isDataAvailable()) {
        int receivedData = comms.readData();
        Serial.print("Received data: ");
        Serial.println(receivedData);
    }
    
    // 5. Print IR Array values
    Serial.print("Final IR Array values: ");
    irarray.printValues();
    
    delay(500); // Wait 500ms between readings
}
