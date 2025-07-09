#include <Wire.h>
#include "sensors.h"
#include "Serial_Comms.h"
#include "EncoderTimer.h"

void setup(){
    Serial.begin(9600);
    Wire.begin();
    TOFSensor tof;
    IMUSensor imu;
    IRArray irarray(5, 6, 7, 8, 9, 10, 11, 12);
    SerialComms comms(115200);
    // get IR Array Values by calling ```getIRArrayValues()```
}

void loop(){
    
}