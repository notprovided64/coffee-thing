#include <EEPROM.h>
#include <HX711_ADC.h>
#include <LiquidCrystal.h>
#include <Servo.h>

enum CupSize {
  medium = 0,
  large = 1
}

const int DATA_RATE = 9600;

const int SCALE_DOUT = 4; // replace with real pin #s later
const int SCALE_SCK_PIN = 5;
const int BUTTON_PIN = 2; 
const int SERVO_PIN = 6;

const int CALIBRATION_ADDRESS = 0x0; 
const int SIZE_ADDRESS = 0x4; 
 
HX711_ADC LoadCell(SCALE_DOUT, SCALE_SCK_PIN);
Servo dispenserServo;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // replace with real pin #s later

bool isCalibrating = false;
float emptyWeight = 0;
float targetWeight = 0;
float currentWeight = 0;

float savedWeight = 0; 

void initialize_scale() {
  float calibrationValue;
  calibrationValue = 114.0;
  EEPROM.get(CALIBRATION_ADDRESS, calibrationValue);

  LoadCell.begin();
  unsigned long stabilizingtime = 2000; // improves tare precision
  boolean _tare = true; //set to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
  }
  else {
    LoadCell.setCalFactor(calibrationValue);
    Serial.println("scale setup is complete");
  }
}

void setup() {
    Serial.begin(POLLING_RATE);
    initialize_scale();
    dispenserServo.attach(SERVO_PIN);
    lcd.begin(16, 2);
    pinMode(BUTTON_PIN, INPUT);
}

void loop() {

}
