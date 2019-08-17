/*
 * AFE4404 Basic Read library example.
 * https://github.com/rakshithbk/AFE4404-Library
 * 
 * On Arduino Uno -  | AFE4404 | Uno Pin
 *                   | I2C_Dat | A4
 *                   | I2C_Clk | A5
 * get_led_val() will return uint32_t values.
 * 
 * Advanced - you can modify the sample period and other
 * parameters in the library files (refer AFE4404 datasheet)
 */


#include <Wire.h>
#include <AFE_connect.h>

AFE A;

void setup() {
  Serial.begin(11520);
  Serial.println("AFE4404 basic readings -\n");
  A.init();
}

void loop() {
  Serial.print("LED 1 Val = ");
  Serial.print(A.get_led1_val());
  Serial.print("\t LED 2 Val = ");
  Serial.print(A.get_led2_val());
  Serial.print("\t LED 3 Val = ");
  Serial.print(A.get_led3_val());
  Serial.println();
}
