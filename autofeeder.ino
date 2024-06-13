#include "menu.h"

extern uint8_t SmallFont[]; 

OLED oled(A3, A4);
DS1307 rtc(A1, A2);
Settings settings;
Menu menu(&oled, &settings);

void setup()
{
  Serial.begin(9600);
  setupOled();
}

void setupOled() {
  oled.begin(); 
  oled.setFont(SmallFont);
  oled.clrScr();
  oled.update();
}

void loop()
{
  menuSetupTime();
  menuSetupMealTime();
  menuSetupTimerOpenGrap();
  
  workflow();

  oled.update();
}


void menuSetupTime() {
  if (!settings.isTimeSetup()) {
    menu.setupTime();
  }
  else {
    oled.clrScr();
  }
}

void menuSetupMealTime() {
  menu.setupMealTimes()
}

void menuSetupTimerOpenGrap() {

}

void workflow() {

}
