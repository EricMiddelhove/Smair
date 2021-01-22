#include "Wire.h"
const byte X = 0, Y = 1, Z = 2;

class GyroSensor{
  private:
    byte sdaPin, sdcPin;
    
    void updateValueVariables(){
      Wire.beginTransmission(MPU_ADDR);
      Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
      Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
      Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
      // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
      accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
      accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
      accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
      temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
      gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
      gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
      gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
    }

  public:
    int accelerometer_x, accelerometer_y, accelerometer_z;
    int gyro_x, gyro_y, gyro_z;
    int temperature;
    char tmp_str[7];
    const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
 
    GyroSensor(){
      Wire.begin();
      Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
      Wire.write(0x6B); // PWR_MGMT_1 register
      Wire.write(0); // set to zero (wakes up the MPU-6050)
      Wire.endTransmission(true);
    }

    //returns an averages value for the specified axle over a time of ~100ms
    int getValue(byte axle){
      
      int avg = 0;
      byte runs = 10;
      
      for(byte i = 0; i < runs ; i++){
        updateValueVariables();

        switch(axle){
          case X: avg = avg + gyro_x; break;
          case Y: avg = avg + gyro_y; break;
          case Z: avg = avg + gyro_z; break;
          default: break;
        }
        
        delay(10);
      }

      return avg / runs;
    }
    
};


GyroSensor gyro;
int closedValue = 0;
int angledValue = closedValue + 100; //Placeholder Values

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.write(gyro.getValue(X));
}
