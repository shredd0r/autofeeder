#include <OLED_I2C.h>
#include "button.h"
#include "settings.h"
#include "feeder.h"

class Menu {
  public:
    void setupTime() {
      if (!_settings->isTimeSetup()) {
        _oled->clrScr();

        if (!isTempTime) {
          _oled->print("Greeting", CENTER, 5);
          _oled->print("set current time", CENTER, 20);
          _oled->print("press 'OK' for save", CENTER, 35);
          _printWaitingCursorForTime();
          _changePartTimeSetup();
          _setupPartTime();
        }
        else {
          _oled->print("Is time correct?", CENTER, 10);
          _oled->print(_getStrTime(), CENTER, 25);
          _printWaitingCursorForYesNo();
          _changePartYesNoSetup();
          _setupTime();
        }
        if (buttonOk.isClicked()) {
          isTempTime = true;
        }
      }
    }

    void setupMealTimes() {
      if (!_settings->isMealTimesSetup() && _settings->isTimeSetup()) {
        _oled->clrScr();
        if (!isTempTime && currentMealTimeSetup <= 5) {
          _oled->print("Set meal time, max 5", CENTER, 5);
          _oled->print("current time: " + String(currentMealTimeSetup), CENTER, 20);
          _oled->print("press 'OK' for save", CENTER, 35);
          _printWaitingCursorForTime();
          _changePartTimeSetup();
          _setupPartTime();
        }
        else {
          _oled->print("Is time correct?", CENTER, 10);
          _oled->print(_getStrTime(), CENTER, 25);
          _printWaitingCursorForYesNo();
          _changePartYesNoSetup();
          _addMealTime();
          _setupMealTimes();
        }
        if (buttonOk.isClicked()) {
          isTempTime = true;
        }
      }
    }

    void setupTimerOpenFlap() {
      if (!_settings->isTimerOpenFlapSetup() && _settings->isMealTimesSetup()) {
        _oled->print("set time open flap", CENTER, 15);
        _oled->print("press 'OK' for save", CENTER, 35);
        _printWaitingCursorForTimerTest();
        _changePartTimerOrTest();
        _setupPartTime(&isTimer, &timerOpenFlap, 1000);
        _setupTimerOpenFlap();
      }
    }

    void clearEEPROM() {
      _settings->clearEEPROM();
    }

    void currentTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
      _oled->print(_getStrWithTime(hours) + " : " + _getStrWithTime(minutes) + " : " + _getStrWithTime(seconds), CENTER, 30);
    }

    Menu(OLED *oled, Settings *settings, Feeder *feeder) {
      this->_oled = oled;
      this->_settings = settings;
      this->_feeder = feeder;
    }
  private:
    OLED* _oled;
    Settings* _settings;
    Feeder* _feeder;

    Button buttonRight = Button(6);
    Button buttonLeft = Button(4);
    Button buttonUp = Button(5);
    Button buttonDown = Button(3);
    Button buttonOk = Button(2);

    bool isSetHours = 1;
    bool isSetMinutes = 0;
    bool isSetSeconds = 0;

    bool isTempTime = 0;
    bool isYes = 1;
    bool isNo = 1;
    bool isTimer = 1;
    bool isTest = 0;

    uint8_t currentMealTimeSetup = 1;
    int timerOpenFlap = 0;
    uint8_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;
    Times mealTimes[5];

    void _setupTime() {
      if (isYes && buttonOk.isClicked()) {
        _settings->setupTime(hours, minutes, seconds);
        _oled->clrScr();
        isTempTime = false;
      }
      if (isNo && buttonOk.isClicked()) {
        isTempTime = false;
      }
    }  

    void _addMealTime() {
      if (isYes && buttonOk.isClicked()) {
        Times newMealTime(hours, minutes, seconds);
        mealTimes[currentMealTimeSetup - 1] = newMealTime;
        
        currentMealTimeSetup++; 
        isTempTime = false;
      }
      if (isNo && buttonOk.isClicked()) {
        isTempTime = false;
      }
      
    }

    void _setupMealTimes() {
      if (currentMealTimeSetup > 5) {
        _settings->setupMealTime(mealTimes);
        currentMealTimeSetup = 1;
        _oled->clrScr();
      }
    }

    void _setupTimerOpenFlap() {
      if (isTimer && buttonOk.isClicked()) {
        _settings->setupTimerOpenFlap(timerOpenFlap);
        _oled->clrScr();
      }
      if (isTest) {
        isTest = !_feeder->openFlap(timerOpenFlap);
        isTimer = !isTest;
      }
    }

    bool _isPassedMillis(int mills) {
      return round(millis() / mills) % 2 == 0;
    }

    void _printWaitingCursorForTime() {
      _printWaitingCursor(_getStrForTimeWithCursor(_getPositionTime()), _getStrTime());
    }

    void _printWaitingCursorForYesNo() {
      _printWaitingCursor(_getStrYesNoWithCursor(_getPositionYesNo()), _getStrYesNo());
    }

    void _printWaitingCursorForTimerTest() {
      _printWaitingCursor(_getStrForTimerOpenFlapWithCursor(_getPositionTimerTest()), _getStrForTimerOpenFlap());
    }

    void _printWaitingCursor(String strWithCursor, String strWithoutCursor) {
      if (_isPassedMillis(100)) {
        _oled->print(strWithCursor, CENTER, 50);
      }
      else {
        _oled->print(strWithoutCursor, CENTER, 50);
      }
    }

    String _getStrForTimeWithCursor(uint8_t position) {
      if (position < 0 || position > 2)
        "position must be >= 0 and <= 2";

      return _getOpenCharForCursor(position, 0) + _getStrWithTime(hours) + _getCloseCharForCursor(position, 0) + ":"
      + _getOpenCharForCursor(position, 1) + _getStrWithTime(minutes) + _getCloseCharForCursor(position, 1) + ":"
      + _getOpenCharForCursor(position, 2) + _getStrWithTime(seconds) + _getCloseCharForCursor(position, 2);
    }

    String _getStrTime() {
      return " " + _getStrWithTime(hours) + " : " + _getStrWithTime(minutes) + " : " + _getStrWithTime(seconds) + " ";
    }

    String _getStrForTimerOpenFlapWithCursor(uint8_t position) {
      return _getOpenCharForCursor(position, 0) + String(timerOpenFlap) + _getCloseCharForCursor(position, 0) + "ms   "
       + _getOpenCharForCursor(position, 1) + "test" + _getCloseCharForCursor(position, 1);
    }

    String _getStrYesNoWithCursor(uint8_t position) {
      return _getOpenCharForCursor(position, 0) + String("Yes") + _getCloseCharForCursor(position, 0) + "/" 
      + _getOpenCharForCursor(position, 1) + String("No") + _getCloseCharForCursor(position, 1);
    }

    String _getStrYesNo() {
      return " Yes / No ";
    }

    String _getStrForTimerOpenFlap() {
      return " " + String(timerOpenFlap) + " ms    test ";
    }

    char _getOpenCharForCursor(uint8_t position, uint8_t expectedPosition) {
      return position == expectedPosition ? '<' : ' ';
    }

    char _getCloseCharForCursor(uint8_t position, uint8_t expectedPosition) {
      return position == expectedPosition ? '>' : ' ';
    }

    uint8_t _getPositionTime() {
      if (isSetHours)
        return 0;
      if (isSetMinutes)
        return 1;
      return 2; 
    }

    uint8_t _getPositionYesNo() {
      if (isYes)
        return 0;
      return 1;
    }

    uint8_t _getPositionTimerTest() {
      if (isTimer)
        return 0;
      return 1;
    }

    String _getStrWithTime(uint8_t time) {
      String strTime = String(time);

      if (strTime.length() == 2)
        return strTime;
      return "0" + strTime;
    }

    void _setupPartTime() {
      _setupPartTime(&isSetHours, &hours, 23);
      _setupPartTime(&isSetMinutes, &minutes, 59);
      _setupPartTime(&isSetSeconds, &seconds, 59);
    }

    void _setupPartTime(bool *flag, uint8_t *countTime, uint8_t max) {
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

    void _setupPartTime(bool *flag, int *countTime, int max) {
      if (*flag) {
        if (buttonUp.isClicked()) {
          if (*countTime < max)
            *countTime = *countTime + 5;
          else
            *countTime = 0;
        }
        if (buttonDown.isClicked()) {
          if (*countTime > 0)
            *countTime = *countTime - 5;
          else
            *countTime = max;
        }
      }
    }

    void _changePartTimeSetup() {
      if (isSetHours)
        _changeLeftRightMenuPart(&isSetHours, &isSetSeconds, &isSetMinutes);
      if (isSetMinutes)
        _changeLeftRightMenuPart(&isSetMinutes, &isSetHours, &isSetSeconds);
      if (isSetSeconds)
        _changeLeftRightMenuPart(&isSetSeconds, &isSetMinutes, &isSetHours);
    }

    void _changePartYesNoSetup() {
      if (isYes)
        _changeLeftRightMenuPart(&isYes, &isNo, &isNo);
      if (isNo)
        _changeLeftRightMenuPart(&isNo, &isYes, &isYes);
    }

    void _changePartTimerOrTest() {
      if (isTimer)
        _changeLeftRightMenuPart(&isTimer, &isTest, &isTest);
      if (isTest)
        _changeLeftRightMenuPart(&isTest, &isTimer, &isTimer);
    }

    void _changeLeftRightMenuPart(bool *currentPart, bool *leftPart, bool *rightPart) {
      if (buttonRight.isClicked()) {
        *currentPart = false;
        *rightPart = true;
      }
      if (buttonLeft.isClicked()) {
        *currentPart = false;
        *leftPart = true;
      }
    }
};