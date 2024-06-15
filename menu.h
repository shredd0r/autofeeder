#include <OLED_I2C.h>
#include "button.h"
#include "settings.h"

class Menu {
  public:
    void setupTime() {
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
    void setupMealTimes() {
      _oled->clrScr();
    }
    void editMealTimes();

    Menu(OLED *oled, Settings *settings) {
      this->_oled = oled;
      this->_settings = settings;
    }
  private:
    OLED* _oled;
    Settings* _settings;

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

    uint8_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;

    void _setupTime() {
      if (isYes && buttonOk.isClicked()) {
        _settings->setupTime(hours, minutes, seconds);
      }

      if (isNo && buttonOk.isClicked()) {
        isTempTime = false;
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

    void _printWaitingCursor(String strWithCursor, String strWithoutCursor) {
      if (_isPassedMillis(100)) 
          _oled->print(strWithCursor, CENTER, 50);
        else
          _oled->print(strWithoutCursor, CENTER, 50);
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

    String _getStrYesNoWithCursor(uint8_t position) {
      return _getOpenCharForCursor(position, 0) + String("Yes") + _getCloseCharForCursor(position, 0) + "/" 
      + _getOpenCharForCursor(position, 1) + String("No") + _getCloseCharForCursor(position, 1);
    }

    String _getStrYesNo() {
      return " Yes / No ";
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

    void _changeLeftRightMenuPart(bool *currentPartTime, bool *leftPartTime, bool *rightPartTime) {
      if (buttonRight.isClicked()) {
        *currentPartTime = false;
        *rightPartTime = true;
      }
      if (buttonLeft.isClicked()) {
        *currentPartTime = false;
        *leftPartTime = true;
      }
    }
};