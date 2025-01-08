#include <EEPROM.h>
#include <HX711_ADC.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Bounce2.h>

// hardware info
const int DATA_RATE = 9600;

const int SCALE_DOUT = 4; // replace with real pin #s later
const int SCALE_SCK_PIN = 5;
const int BUTTON_1_PIN = 2; 
const int BUTTON_2_PIN = 3; 
const int SERVO_PIN = 6;

Button2 button_1, button_2;

HX711_ADC LoadCell(SCALE_DOUT, SCALE_SCK_PIN);
Servo dispenserServo;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // replace with real pin #s later

const int CALIBRATION_ADDR = 0x0; 
const int SAVED_VOL_1_ADDR = 0x4; 
//const int SAVED_VOL_2_ADDR = 0x8; 

// saved values
const int CUTOFF_WEIGHT = 30; //grams or something

// state
typdef enum { initial, calibration, dispense } states;

float targetVolume = 0;
float currentWeight = 0;

states state = initial;
states prevState = initial;

void setup() {
    Serial.begin(POLLING_RATE);
    initialize_scale();
    dispenserServo.attach(SERVO_PIN);
    lcd.begin(16, 2);
    
    button_1.begin(BUTTON_1_PIN);
    button_1.setClickHandler(handler);
    button_1.setDoubleClickHandler(handler);

    button_2.begin(BUTTON_2_PIN);
    button_2.setClickHandler(handler);
    button_2.setDoubleClickHandler(handler);

    //loadSavedVolume will set saved1 and saved2 to correct amount
    programState = setup
}

void initialize_scale() {
  float calibrationValue;
  calibrationValue = 114.0; //default value
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


void loop() {
  currentTime = millis();

  readButtons();
  updateDisplay();
  updateServo();
}

void readButtons(){
  button_1.loop();
  button_2.loop();
}

updateDisplay();

void updateServo() {
  if (state != dispense)
    return;

  //logic for dispensing based on currentVolume
}

void handler(Button2& btn) {
  switch (btn.getType()) {
    case single_click:
      handleSingleClick(btn);
      break;
    case double_click:
      handleDoubleClick(btn);
      break;
    default:
      break;
  }
}

void handleSingleClick(Button2& btn) {
  Serial.print("single click on button ");
  Serial.println((btn == button_1) ? "1" : "2");

  switch (state) {
    case initial:
      changeState(dispense);

      targetVolume = (btn == button_1) ? savedVOL1 : savedVOL2;

      break;
    case calibration:
      break;
    case dispense:
      break;
    default:
      break;
  }
}

void handleDoubleClick(Button2& btn) {
  Serial.print("double click on button ");
  Serial.println((btn == button_1) ? "1" : "2");

  switch (state) {
    case initial:
      changeState(calibration);
      break;
    case calibration:
      break;
    case dispense:
      break;
    default:
      break;
  }
}

void changeState(states newState) {
  prevState = state;
  state = newState;
}

