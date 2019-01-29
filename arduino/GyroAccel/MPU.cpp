//
// Created by coleman on 1/27/19.
//

#include "MPU.h"

#include <SPI.h>
#include <stdint.h>

#include "Arduino.h"


void MPU::initMpu(int chipSelectPin) {

    setChipSelectPin(chipSelectPin);
    SPIClass::begin();

    Serial.println("Initializing MPU \n");

    writeReg(PWR_MGMT_1, RESET);
    delay(100);


    uint8_t c = readReg(WHO_AM_I, 0x00);

    Serial.print("MPU9250 I AM 0x");
    Serial.print(c, HEX);
    Serial.print(" I should be 0x");
    Serial.println(0x71, HEX);

    if (c == 0x71) {
        Serial.println("MPU9250 has successfully responded \n");

    } else {
        Serial.print("Incorrect response for MPU9250. Please check wiring.");

        while(true){
          delay(1000);
        }

    }

Serial.println("--- INITIALIZING REGISTERS ---");
    writeReg(USER_CTRL, 0x00);
    writeReg(USER_CTRL, 0x30);
Serial.println("--- END INITIALIZING REGISTERS ---");
    delay(100);

}


uint8_t MPU::writeReg(uint8_t registerAddress, uint8_t data, bool readFlag) {
    uint8_t temp_val = 0;

    if (readFlag) {
        Serial.println("--- READ ---");
        Serial.print("Reading from Register: 0x");
        // using bitwise AND to remove the read flag to get register value
        Serial.println(registerAddress & 0x7F, HEX);

    } else {
        Serial.println("--- WRITE ---");
        Serial.print("Writing to Register: 0x");
        Serial.println(registerAddress, HEX);
        Serial.print("Data to write: ");
        Serial.print(data, BIN);
        Serial.println("b");

    }

    select();
    SPIClass::transfer(registerAddress);
    temp_val = SPIClass::transfer(data);
    deselect();

    if (readFlag) {
        Serial.println("--- END READ ---");
        Serial.println();
    } else {
        Serial.print("temp_val after call: 0x");
        Serial.println(temp_val, HEX);
        Serial.println("--- END WRITE ---");
        Serial.println();
    }


    return temp_val;
}

uint8_t MPU::readReg(uint8_t registerAddress, uint8_t data) {
    // bitwise OR done to registerAddress to set read bit
    return writeReg(static_cast<uint8_t>(registerAddress | READ_FLAG), data, true);
}


void MPU::select() {
    //Set CS low to start transmission
    SPIClass::beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE3));
    digitalWrite(chipSelectPin, LOW);
}

void MPU::deselect() {
    //Set CS high to stop transmission
    digitalWrite(chipSelectPin, HIGH);
    SPIClass::endTransaction();
}
