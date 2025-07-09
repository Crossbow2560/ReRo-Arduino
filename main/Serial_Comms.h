#ifndef SERIAL_COMMS_H
#define SERIAL_COMMS_H

#include <Arduino.h>

class SerialComms {
private:
    unsigned long baudRate;

public:
    // Constructor with baud rate
    SerialComms(unsigned long baud = 115200) : baudRate(baud) {
        Serial1.begin(baudRate);
    }
    
    // Send TOF sensor data
    void sendTOFData(uint16_t Value) {
        // Send start byte/header (0xFF indicates start of data packet)
        Serial1.write(0xFF);
        
        // Send data type identifier ('T' for TOF sensor data)
        Serial1.write('T');
        
        // Convert and send TOF value as single byte (0-100mm fits in uint8_t)
        Serial1.write((uint8_t)Value);
        
        // Send end byte (0xFE indicates end of data packet)
        Serial1.write(0xFE);
    }
    
    // Send IMU data
    void sendIMUData(float x, float y, float z) {
        Serial1.write(0xFF);
        Serial1.write('I');
        Serial1.write((uint8_t)(x * 100)); // Scale and convert to byte
        Serial1.write((uint8_t)(y * 100));
        Serial1.write((uint8_t)(z * 100));
        Serial1.write(0xFE);
    }
    
    // Send IR array data
    void sendIRArrayData(int* values, int size) {
        Serial1.write(0xFF);
        Serial1.write('R');
        Serial1.write((uint8_t)size);
        for(int i = 0; i < size; i++) {
            Serial1.write((uint8_t)values[i]);
        }
        Serial1.write(0xFE);
    }

    void send8BitData(int value){
        Serial1.write(0xFF);
        Serial.write('W');
        Serial1.write((uint8_t)value);
        Serial1.write(0xFE);
    }
    
    // Check if data is available
    bool isDataAvailable() {
        return Serial1.available() > 0;
    }
    
    // Read incoming data
    int readData() {
        return Serial1.read();
    }
};

#endif