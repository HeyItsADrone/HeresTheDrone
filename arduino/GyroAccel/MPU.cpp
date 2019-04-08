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

Serial.println("--- INITIALIZING REGISTERS ---\n");
    writeReg(USER_CTRL, 0x00);
    writeReg(USER_CTRL, 0x30);
Serial.println("--- END INITIALIZING REGISTERS ---\n");
    delay(100);

}

void MPU::testMpu(float * destination) {
  
   uint8_t rawData[6] = {0, 0, 0, 0, 0, 0};
   uint8_t selfTest[6];
   int32_t gAvg[3] = {0}, aAvg[3] = {0}, aSTAvg[3] = {0}, gSTAvg[3] = {0};
   float factoryTrim[6];
   uint8_t FS = 0;
   
  writeReg(SMPLRT_DIV, 0x00);    // Set gyro sample rate to 1 kHz
  writeReg(CONFIG, 0x02);        // Set gyro sample rate to 1 kHz and DLPF to 92 Hz
  writeReg(GYRO_CONFIG, FS<<3);  // Set full scale range for the gyro to 250 dps
  writeReg(ACCEL_CONFIG2, 0x02); // Set accelerometer rate to 1 kHz and bandwidth to 92 Hz
  writeReg(ACCEL_CONFIG, FS<<3); // Set full scale range for the accelerometer to 2 g

  for( int ii = 0; ii < 200; ii++) {  // get average current values of gyro and acclerometer
  
  readReg(ACCEL_XOUT_H, &rawData[0]);        // Read the six raw data registers into data array
  aAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  aAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
  aAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
  
  readReg(GYR_XOUT_H, &rawData[0]);       // Read the six raw data registers sequentially into data array
  gAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  gAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
  gAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
  
}
  
  for (int ii =0; ii < 3; ii++) {  // Get average of 200 values and store as average current readings
  aAvg[ii] /= 200;
  gAvg[ii] /= 200;

}
  
// Configure the accelerometer for self-test
   writeReg(ACCEL_CONFIG, 0xE0); // Enable self test on all three axes and set accelerometer range to +/- 2 g
   writeReg(GYRO_CONFIG,  0xE0); // Enable self test on all three axes and set gyro range to +/- 250 degrees/s
   delay(25);  // Delay a while to let the device stabilize

  for( int ii = 0; ii < 200; ii++) {  // get average self-test values of gyro and acclerometer
  
  readReg(ACCEL_XOUT_H, &rawData[0]);  // Read the six raw data registers into data array
  aSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  aSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
  aSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
  
  readReg(GYR_XOUT_H, &rawData[0]);  // Read the six raw data registers sequentially into data array
  gSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  gSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
  gSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
  }
  
  for (int ii =0; ii < 3; ii++) {  // Get average of 200 values and store as average self-test readings
  aSTAvg[ii] /= 200;
  gSTAvg[ii] /= 200;
  }   
  
 // Configure the gyro and accelerometer for normal operation
   writeReg(ACCEL_CONFIG, 0x00);  
   writeReg(GYRO_CONFIG,  0x00);  
   delay(25);  // Delay a while to let the device stabilize
   
   // Retrieve accelerometer and gyro factory Self-Test Code from USR_Reg
   selfTest[0] = readReg(SELF_TEST_X_ACCEL, 0x00); // X-axis accel self-test results
   selfTest[1] = readReg(SELF_TEST_Y_ACCEL, 0x00); // Y-axis accel self-test results
   selfTest[2] = readReg(SELF_TEST_Z_ACCEL, 0x00); // Z-axis accel self-test results
   selfTest[3] = readReg(SELF_TEST_X_GYRO, 0x00);  // X-axis gyro self-test results
   selfTest[4] = readReg(SELF_TEST_Y_GYRO, 0x00);  // Y-axis gyro self-test results
   selfTest[5] = readReg(SELF_TEST_Z_GYRO, 0x00);  // Z-axis gyro self-test results

  // Retrieve factory self-test value from self-test code reads
   factoryTrim[0] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[0] - 1.0) )); // FT[Xa] factory trim calculation
   factoryTrim[1] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[1] - 1.0) )); // FT[Ya] factory trim calculation
   factoryTrim[2] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[2] - 1.0) )); // FT[Za] factory trim calculation
   factoryTrim[3] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[3] - 1.0) )); // FT[Xg] factory trim calculation
   factoryTrim[4] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[4] - 1.0) )); // FT[Yg] factory trim calculation
   factoryTrim[5] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[5] - 1.0) )); // FT[Zg] factory trim calculation
 
 // Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
 // To get percent, must multiply by 100
   for (int i = 0; i < 3; i++) {
     destination[i]   = 100.0*((float)(aSTAvg[i] - aAvg[i]))/factoryTrim[i] - 100.;   // Report percent differences
     destination[i+3] = 100.0*((float)(gSTAvg[i] - gAvg[i]))/factoryTrim[i+3] - 100.; // Report percent differences
}
  
}


uint8_t MPU::writeReg(uint8_t registerAddress, uint8_t data) {
    uint8_t temp_val = 0;

    select();
    SPIClass::transfer(registerAddress);
    temp_val = SPIClass::transfer(data);
    deselect();


    return temp_val;
}

uint8_t MPU::readReg(uint8_t registerAddress, uint8_t data) {
    // bitwise OR done to registerAddress to set read bit
    return writeReg(static_cast<uint8_t>(registerAddress | READ_FLAG), data);
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

void MPU::gyroSelfTests(float * destination) {

  Serial.println("--- START Gyro Self Tests ---");

  uint8_t xGyroValue = readReg(SELF_TEST_X_GYRO, 0x00);

  Serial.print("Factory XG Value: ");
  Serial.println(xGyroValue, HEX);

  
  uint8_t yGyroValue = readReg(SELF_TEST_Y_GYRO, 0x00);

  Serial.print("Factory YG Value: ");
  Serial.println(yGyroValue, HEX);
  

  
  uint8_t zGyroValue = readReg(SELF_TEST_Z_GYRO, 0x00);

  Serial.print("Factory ZG Value: ");
  Serial.println(zGyroValue, HEX);

  Serial.println("--- END Gyro Self Tests---\n");
  
}

void MPU::accelSelfTests(float * destination) {

  Serial.println("--- START Accel Self Tests ---");

  uint8_t xAccelValue = readReg(SELF_TEST_X_ACCEL, 0x00);

  Serial.print("Factory XA Value: ");
  Serial.println(xAccelValue, HEX);

  
  uint8_t yAccelValue = readReg(SELF_TEST_Y_ACCEL, 0x00);

  Serial.print("Factory YA Value: ");
  Serial.println(yAccelValue, HEX);
  

  
  uint8_t zAccelValue = readReg(SELF_TEST_Z_ACCEL, 0x00);

  Serial.print("Factory ZA Value: ");
  Serial.println(zAccelValue, HEX);

  Serial.println("--- END Accel Self Tests ---\n");
  
}
