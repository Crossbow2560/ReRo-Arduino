#ifndef IRARRAY_H
#define IRARRAY_H

#include <Arduino.h>

class IRArray {
private:
    int* pinArray;
    int* values;
    int arraySize;

public:
    // Constructor with pin numbers
    IRArray(int IR1, int IR2, int IR3, int IR4, int IR5, int IR6, int IR7, int IR8) : arraySize(8) {
        pinArray = new int[arraySize];
        values = new int[arraySize];
        
        pinArray[0] = IR1; pinArray[1] = IR2; pinArray[2] = IR3; pinArray[3] = IR4;
        pinArray[4] = IR5; pinArray[5] = IR6; pinArray[6] = IR7; pinArray[7] = IR8;
        
        for(int i = 0; i < arraySize; i++) {
            pinMode(pinArray[i], INPUT);
            values[i] = digitalRead(pinArray[i]);
        }
    }
    
    // Destructor
    ~IRArray() {
        delete[] pinArray;
        delete[] values;
    }
    
    // Read all IR sensor values
    int* getValues() {
        for(int i = 0; i < arraySize; i++) {
            values[i] = digitalRead(pinArray[i]);
        }
        return values;
    }
    
    // Get specific sensor value by index
    int getValue(int index) {
        if(index >= 0 && index < arraySize) {
            return digitalRead(pinArray[index]);
        }
        return -1; // Error value
    }
    
    // Get array size
    int getSize() const {
        return arraySize;
    }
    
    // Print all values for debugging
    void printValues() {
        for(int i = 0; i < arraySize; i++) {
            Serial.print("IR[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.print(values[i]);
            if(i < arraySize - 1) Serial.print(", ");
        }
        Serial.println();
    }
};

#endif