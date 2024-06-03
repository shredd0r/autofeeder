#include <OLED_I2C.h>
#include <DS1307.h>
#include "button.h"
#include "settings.h"

extern uint8_t SmallFont[]; 

Button buttonRight(6);
Button buttonLeft(4);
Button buttonUp(5);
Button buttonDown(3);
Button buttonOk(2);

bool isSetHours = 1;
bool isSetMinutes = 0;
bool isSetSeconds = 0;

bool isTempTime = 0;
bool isYes = 1;
bool isNo = 1;

uint8_t hours = 0;
uint8_t minutes = 0;
uint8_t seconds = 0;

OLED oled(A3, A4);
// DS1307 rtc(A1, A2);
Settings settings;

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
  if (!settings.isTimeSetup()) {
    oled.clrScr();
    oled.print("Greeting", CENTER, 5);
    oled.print("set current time", CENTER, 20);
    oled.print("press 'OK' for save", CENTER, 35);
    
    if (buttonOk.isClicked()) {
      isTempTime = true;
    } 

    if (isTempTime) {
      oled.clrScr();
      oled.print("Is time correct?", CENTER, 10);
      oled.print(getStrTime(), CENTER, 25);
      printWaitingCursorForYesNo();
      changePartYesNoSetup();
      setupTime();
    }
    else {
      printWaitingCursorForTime();
      changePartTimeSetup();
      setupPartTime();
    }
    
    oled.update();
  }
}

void setupTime() {
  if (isYes && buttonOk.isClicked()) {
    Serial.println("setup time");
  }

  if (isNo && buttonOk.isClicked()) {
    isTempTime = false;
  }
}  

bool isPassedMillis(int mills) {
  return round(millis() / mills) % 2 == 0;
}

void printWaitingCursorForTime() {
  printWaitingCursor(getStrForTimeWithCursor(getPositionTime()), getStrTime());
}

void printWaitingCursorForYesNo() {
  printWaitingCursor(getStrYesNoWithCursor(getPositionYesNo()), getStrYesNo());
}

void printWaitingCursor(String strWithCursor, String strWithoutCursor) {
  if (isPassedMillis(100)) 
      oled.print(strWithCursor, CENTER, 50);
    else
      oled.print(strWithoutCursor, CENTER, 50);
}

String getStrForTimeWithCursor(uint8_t position) {
  if (position < 0 || position > 2)
    "position must be >= 0 and <= 2";

  return getOpenCharForCursor(position, 0) + getStrWithTime(hours) + getCloseCharForCursor(position, 0) + ":"
  + getOpenCharForCursor(position, 1) + getStrWithTime(minutes) + getCloseCharForCursor(position, 1) + ":"
  + getOpenCharForCursor(position, 2) + getStrWithTime(seconds) + getCloseCharForCursor(position, 2);
}

String getStrTime() {
  return " " + getStrWithTime(hours) + " : " + getStrWithTime(minutes) + " : " + getStrWithTime(seconds) + " ";
}

String getStrYesNoWithCursor(uint8_t position) {
  return getOpenCharForCursor(position, 0) + String("Yes") + getCloseCharForCursor(position, 0) + "/" 
  + getOpenCharForCursor(position, 1) + String("No") + getCloseCharForCursor(position, 1);
}

String getStrYesNo() {
  return " Yes / No ";
}

char getOpenCharForCursor(uint8_t position, uint8_t expectedPosition) {
  return position == expectedPosition ? '<' : ' ';
}

char getCloseCharForCursor(uint8_t position, uint8_t expectedPosition) {
  return position == expectedPosition ? '>' : ' ';
}

uint8_t getPositionTime() {
  if (isSetHours)
    return 0;
  if (isSetMinutes)
    return 1;
  return 2; 
}

uint8_t getPositionYesNo() {
  if (isYes)
    return 0;
  return 1;
}

String getStrWithTime(uint8_t time) {
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

void setupPartTime(bool *flag, uint8_t *countTime, uint8_t max) {
  if (*flag) {
    if (buttonUp.isClicked()) {
      if (*countTime < max)
        *countTime = *countTime + 1;
      else
        *countTime = 0;
    }
    if (buttonDown.isClicked()) {
      if (*countTime > 0)
        *countTime = *countTime - 1;
      else
        *countTime = max;
    }
  }
}

void changePartTimeSetup() {
  if (isSetHours)
    changeLeftRightMenuPart(&isSetHours, &isSetSeconds, &isSetMinutes);
  if (isSetMinutes)
    changeLeftRightMenuPart(&isSetMinutes, &isSetHours, &isSetSeconds);
  if (isSetSeconds)
    changeLeftRightMenuPart(&isSetSeconds, &isSetMinutes, &isSetHours);
}

void changePartYesNoSetup() {
  if (isYes)
    changeLeftRightMenuPart(&isYes, &isNo, &isNo);
  if (isNo)
    changeLeftRightMenuPart(&isNo, &isYes, &isYes);
}

void changeLeftRightMenuPart(bool *currentPartTime, bool *leftPartTime, bool *rightPartTime) {
  if (buttonRight.isClicked()) {
    *currentPartTime = false;
    *rightPartTime = true;
  }
  if (buttonLeft.isClicked()) {
    *currentPartTime = false;
    *leftPartTime = true;
  }
}
