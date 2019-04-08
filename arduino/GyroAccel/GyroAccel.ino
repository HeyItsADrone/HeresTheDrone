

#include <SPI.h>
#include "MPU.h"

const int chipSelectPin = 45;

void setup() {

    Serial.begin(115200);
    MPU mpu;

    pinMode(chipSelectPin, OUTPUT);

    digitalWrite(chipSelectPin, HIGH);

    
    SPI.begin();

    delay(1000);


    
    mpu.initMpu(chipSelectPin);
    mpu.testMpu(SelfTest);

}

void loop() {
    // put your main code here, to run repeatedly:


}
