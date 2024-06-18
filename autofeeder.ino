#include <Servo.h>
#include "menu.h"
#include "fountain.h"

extern uint8_t SmallFont[]; 

uint8_t pinServo = 7;

Servo servo;
DS1307 rtc(A4, A5);
OLED oled(A2, A3);
Feeder feeder(&rtc, &servo);
Fountain fountain(1, 2, 3, &oled);
Settings settings(&rtc);
Menu menu(&oled, &settings, &feeder);

void setup()
{
  // settings.clearEEPROM();
  Serial.begin(9600);
  Serial.println("start setup");  
  setupOled();
  setupServo();
  setupMealTimes();
}

void setupOled() {
  oled.begin(); 
  oled.setFont(SmallFont);
  oled.clrScr();
  oled.update();
  Serial.println("oled setup successful");
}

void setupServo() {
  servo.attach(pinServo);
  servo.write(closeDegree);
  Serial.println("servo setup successful");
}

void loop()
{
  if (settings.isSetup()) {
    workflow();
  } 
  else {
    menu.setupTime();
    menu.setupMealTimes();
    menu.setupTimerOpenFlap();
    setupMealTimes();
  }

  oled.update();
}

void workflow() {
  currentTime();
  feeder.workflow();
  fountain.workflow();
} 

void currentTime() {
  Time now = rtc.getTime();
  menu.currentTime(now.hour, now.min, now.sec);
}

void setupMealTimes() {
  feeder.updateMealTimes(settings.getMealTimes());
  feeder.updateTimerOpenFlap(500);
}
