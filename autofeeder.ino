#include <OLED_I2C.h>
#include "button.h"

Button buttonRight(6);
Button buttonLeft(4);
Button buttonUp(5);
Button buttonDown(3);
Button buttonOk(2);

bool isSetHours = 1;
bool isSetMinutes = 0;
bool isSetSeconds = 0;

int hours = 0;
int minutes = 0;
int seconds = 0;


OLED oled(A4, A5, A4);
extern uint8_t SmallFont[]; 

void setup()
{
  Serial.begin(9600);
  oled.begin(); 
  oled.setFont(SmallFont);
  oled.clrScr();
  oled.update();
}

void loop()
{
  oled.print("Greeting", CENTER, 10);
  oled.print("set current time", CENTER, 20);
  printWaitingCursor();
  changePartTimeSetup();
  setupPartTime();
  oled.update();
}

bool isPassedMillis(int mills) {
  return round(millis() / mills) % 2 == 0;
}

void printWaitingCursor() {
  if (isPassedMillis(500)) 
      oled.print(getStrForSetupTime(getPosition()), CENTER, 35);
    else
      oled.print(" " + getStrWithTime(hours) + " : " + getStrWithTime(minutes) +" : " + getStrWithTime(seconds) +" ", CENTER, 35);
}

String getStrForSetupTime(int position) {
  if (position < 0 || position > 2)
    "position must be >= 0 and <= 2";

  return getOpenCharForCursor(position, 0) + getStrWithTime(hours) + getCloseCharForCursor(position, 0) + ":"
  + getOpenCharForCursor(position, 1) + getStrWithTime(minutes) + getCloseCharForCursor(position, 1) + ":"
  + getOpenCharForCursor(position, 2) + getStrWithTime(seconds) + getCloseCharForCursor(position, 2);
}

char getOpenCharForCursor(int position, int expectedPosition) {
  return position == expectedPosition ? '<' : ' ';
}

char getCloseCharForCursor(int position, int expectedPosition) {
  return position == expectedPosition ? '>' : ' ';
}

int getPosition() {
  if (isSetHours)
    return 0;
  if (isSetMinutes)
    return 1;
  return 2; 
}

String getStrWithTime(int time) {
  String strTime = String(time);

  if (strTime.length() == 2)
    return strTime;
  return "0" + strTime;
}

void setupPartTime() {
  setupPartTime(&isSetHours, &hours, 23);
  setupPartTime(&isSetMinutes, &minutes, 59);
  setupPartTime(&isSetSeconds, &seconds, 59);
}

void setupPartTime(bool *flag, int *countTime, int max) {
  if (*flag) {
    if (buttonUp.isClicked() && *countTime < max) {
      *countTime = *countTime + 1;
    }
    if (buttonDown.isClicked() && *countTime > 0) {
      *countTime = *countTime - 1;
    }
  }
}

void changePartTimeSetup() {
  if (isSetHours)
    changePartTimeSetup(&isSetHours, &isSetSeconds, &isSetMinutes);
  if (isSetMinutes)
    changePartTimeSetup(&isSetMinutes, &isSetHours, &isSetSeconds);
  if (isSetSeconds)
    changePartTimeSetup(&isSetSeconds, &isSetMinutes, &isSetHours);
}

void changePartTimeSetup(bool *currentPartTime, bool *leftPartTime, bool *rightPartTime) {
  if (buttonRight.isClicked()) {
    Serial.println("true");
    *currentPartTime = false;
    *rightPartTime = true;
  }
  if (buttonLeft.isClicked()) {
    Serial.println("true2");
    *currentPartTime = false;
    *leftPartTime = true;
  }
}
