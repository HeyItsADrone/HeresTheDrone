

#include <SPI.h>
#include <Wire.h>

// I2C Definitions
#define I2C_MST_CTRL 0x24
#define I2C_SLV0_ADDR 0x25
#define I2C_SLV0_REG 0x26
#define I2C_SLV0_CTRL 0x27
#define I2C_SLV1_ADDR 0x28
#define I2C_SLV1_REG 0x29
#define I2C_SLV1_CTRL 0x2A
#define I2C_SLV2_ADDR 0x2B
#define I2C_SLV2_REG 0x2C
#define I2C_SLV2_CTRL 0x2D
#define I2C_SLV3_ADDR 0X2E
#define I2C_SLV3_REG 0X2F
#define I2C_SLV3_CTRL 0x30
#define I2C_SLV4_ADDR 0x31
#define I2C_SLV4_REG 0x32
#define I2C_SLV4_DO 0x33
#define I2C_SLV4_CTRL 0x34
#define I2C_SLV4_DI 0x35
#define I2C_MST_STATUS 0x36
#define INT_PIN_CONFIG 0x37
#define INT_ENABLE 0x38
#define INT_STATUS 0x3A
#define I2C_SLV0_DO 0x63
#define I2C_SLV1_DO 0x64
#define I2C_SLV2_DO 0x65
#define I2C_SLV3_DO 0x66 
#define I2C_MST_DELAY_CTRL 0x67 






// GYRO Definitions
#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02

#define XG_OFFSET_H 0x13
#define XG_OFFSET_L 0X14
#define YG_OFFSET_H 0x15
#define YG_OFFSET_L 0X16
#define ZG_OFFSET_H 0x17
#define ZG_OFFSET_L 0X18

#define GYRO_CONFIG 0x1B

#define GYR_XOUT_H 0x43
#define GYR_XOUT_L 0x44
#define GYR_YOUT_H 0x45
#define GYR_YOUT_L 0x46
#define GYR_ZOUT_H 0x47
#define GYR_ZOUT_L 0x48




// ACCEL Definitions
#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F

#define ACCEL_CONFIG 0x1C
#define ACCEL_CONFIG2 0x1D

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

#define XA_OFFSET_H 0x77 
#define XA_OFFSET_L 0x78 
#define YA_OFFSET_H 0x7A 
#define YA_OFFSET_L 0x7B 
#define ZA_OFFSET_H 0x7D 
#define ZA_OFFSET_L 0x7E 

//TEMPERATURE Definitions
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42

// External Sensors
#define EXT_SENS_DATA_00 0x49
#define EXT_SENS_DATA_01 0x4A
#define EXT_SENS_DATA_02 0x4B
#define EXT_SENS_DATA_03 0x4C
#define EXT_SENS_DATA_04 0x4D
#define EXT_SENS_DATA_05 0x4E
#define EXT_SENS_DATA_06 0x4F
#define EXT_SENS_DATA_07 0x50
#define EXT_SENS_DATA_08 0x51
#define EXT_SENS_DATA_09 0x52
#define EXT_SENS_DATA_10 0x53
#define EXT_SENS_DATA_11 0x54
#define EXT_SENS_DATA_12 0x55
#define EXT_SENS_DATA_13 0x56
#define EXT_SENS_DATA_14 0x57
#define EXT_SENS_DATA_15 0x58
#define EXT_SENS_DATA_16 0x59
#define EXT_SENS_DATA_17 0x5A
#define EXT_SENS_DATA_18 0x5B
#define EXT_SENS_DATA_19 0x5C
#define EXT_SENS_DATA_20 0x5D
#define EXT_SENS_DATA_21 0x5E
#define EXT_SENS_DATA_22 0x5F
#define EXT_SENS_DATA_23 0x60


// General Definitions
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A

#define LP_ACCEL_ODR 0x1E
#define WOM_THR 0x1F
#define FIFO_EN 0x23

#define SIGNAL_PATH_RESET 0x68 //MPU9250_ADDRESS on Arduino
#define MOT_DETECT_CTRL 0x69 
#define USER_CTRL 0x6A 
#define PWR_MGMT_1 0x6B 
#define PWR_MGMT_2 0x6C 
#define FIFO_COUNTH 0x72  
#define FIFO_COUNTL 0x73  
#define FIFO_R_W 0x74 
#define WHO_AM_I 0x75 

#define READ_FLAG 0x80

#define RESET 0x80

/*
SPI Specs
The maximum frequency of SCLK is 1MHz
SPI read and write operations are completed in 16 or more clock cycles (two or more bytes)
The first byte contains the SPI Address, and the following byte(s) contain(s) the SPI data
The first bit of the first byte contains the Read/Write bit and indicates the Read (1) or Write (0) operation
The following 7 bits contain the Register Address
In cases of multiple-byte Read/Writes, data is two or more bytes:

SPI Address format

MSB                   LSB
R/W A6 A5 A4 A3 A2 A1 A0

SPI Data format
MSB               LSB
D7 D6 D5 D3 D2 D1 D0

*/

/*
  Board: Arduino MEGA 2560

             ----------------------------------------
  Connections| MOSI | MISO | SCK | SS | POWER | GND |
             ----------------------------------------
      Arduino|  51  |  50  |  52 | 45 |  5V   | GND |
             ----------------------------------------
      MPU9250|  SDI |  SDO | SCL |NCS |  VCC  | GND |
             ----------------------------------------
  
*/




#define CLOCK_SPEED 1000000


const int chipSelectPin = 45;


void setup() {

    Serial.begin(115200);
    pinMode(chipSelectPin, OUTPUT);

    digitalWrite(chipSelectPin, HIGH);

    
    SPI.begin();

    delay(1000);

    initMpu();

    delay(100);

    uint8_t c = readReg(WHO_AM_I, 0x00);

    Serial.print("MPU9250 "); Serial.print("I AM 0x"); Serial.print(c, HEX); Serial.print(" I should be 0x"); Serial.println(0x71, HEX);

    if (c == 0x71)
    {
        Serial.print("MPU9250 is ready");

    }

    else
    {
        Serial.print("Incorrect response for MPU9250");

    }

}

void loop() {
    // put your main code here, to run repeatedly:


}


unsigned int writeReg( uint8_t registerAddress, uint8_t data, bool readFlag=false )
{
    unsigned int temp_val = 0;
    
    if(readFlag){
      Serial.println("--- READ ---");
      Serial.print("Reading from Register: 0x");
      // using bitwise AND to remove the read flag to get register value
      Serial.println(registerAddress & 0x7F, HEX);
      
    } else{
      Serial.println("--- WRITE ---");
      Serial.print("Writing to Register: 0x");
      Serial.println(registerAddress, HEX);
      Serial.print("Data to write: ");
      Serial.print(data, BIN);
      Serial.println("b");
      
    }
    
    select();
    SPI.transfer(registerAddress);
    temp_val=SPI.transfer(data);
    deselect();

    if(readFlag){
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

unsigned int  readReg( uint8_t registerAddress, uint8_t data )
{
    // bitwise OR done to registerAddress to set read bit
    return writeReg(registerAddress | READ_FLAG, data, true);
}

void initMpu(){

  Serial.println("Initializing MPU \n");

  writeReg(PWR_MGMT_1, RESET);
  writeReg(USER_CTRL, 0x00);
  writeReg(USER_CTRL, 0x30);
  
}

void select() {
    //Set CS low to start transmission
    SPI.beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE3));
    digitalWrite(chipSelectPin, LOW);
}
void deselect() {
    //Set CS high to stop transmission
    digitalWrite(chipSelectPin, HIGH);
    SPI.endTransaction();
}
