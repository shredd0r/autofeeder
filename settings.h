/*
  In EEPROM store data like:
  - isTimeSet           bool  [0]
  - isMealTimesSet      bool  [1]
  - isTimerOpenFlapSet  bool  [2]
  - timerOpenFlap       int   [3..4]
  - mealTime[0]         Times [5..7] 
  - mealTime[1]         Times [8..10]
  - mealTime[2]         Times [11..13]
  - mealTime[3]         Times [14..16]
  - mealTime[4]         Times [17]
*/

#include <DS1307.h>
#include <EEPROM.h>
#include "times.h"

const uint8_t addressIsTimeSet = 0;

class Settings {
  public:
    bool isTimeSetup() {
      return this->_isTimeSet;
    }
    bool isMealTimesSetup() {
      return this->_isMealTimesSet;
    }
    bool isTimerOpenFlapSetup() {
      return this->_isTimerOpenFlapSet;
    }
    bool isSetup() {
      return _isTimeSet && _isMealTimesSet && _isTimerOpenFlapSet;
    }
    void setupTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
      _rtc->setTime(hours, minutes, seconds);
      
      this->_isTimeSet = true;
      EEPROM.put(addressIsTimeSet, this->_isTimeSet);
    }
    void setupMealTime(Times* meals) {
      uint8_t addressIsMealTimesSet = _getAddressForIsMealTimesSet();

      MealTimes mealTimes;

      for(uint8_t i = 0; i < countMealTimes; i++) {
        mealTimes.meals[i] = *(meals + i);
      }

      _isMealTimesSet = true;
      _mealTimes = mealTimes;

      EEPROM.put(addressIsMealTimesSet, _isMealTimesSet);
      EEPROM.put(_getAddressForMealTimes(), mealTimes);
    }
    
    void setupTimerOpenFlap(int timeInMillis) {
      _isTimerOpenFlapSet = true;
      _timerOpenFlap = timeInMillis;

      EEPROM.put(_getAddressForIsTimerOpenFlapSet(), _isTimerOpenFlapSet);
      EEPROM.put(_getAddressForTimerOpenFlap(), _timerOpenFlap);
    }

    MealTimes getMealTimes() {
      return this->_mealTimes;
    }
    
    Times* getMeal(uint8_t index) {
      return &_mealTimes.meals[index];
    }
    
    int getTimerOpenFlap() {
      return this->_timerOpenFlap;
    }

    void clearEEPROM() {
      for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
      }
    }

    MealTimes _getMealTimesFromEEPROM() {
      MealTimes mts;
      uint8_t address = _getAddressForMealTimes();

      EEPROM.get(address, mts);
      
      return mts;
    }

    Settings(DS1307 *rtc) {

      _rtc = rtc;
      _isTimeSet = _getIsTimeSetFromEEPROM();
      _isMealTimesSet = _getIsMealTimesSetFromEEPROM();
      _isTimerOpenFlapSet = _getIsTimerOpenFlapSetFromEEPROM();
      _timerOpenFlap = _getTimerOpenFlapFromEEPROM();
      _mealTimes = _getMealTimesFromEEPROM();
    }

  private:
    bool _isTimeSet = false;
    bool _isMealTimesSet = false;
    bool _isTimerOpenFlapSet = false;
    int _timerOpenFlap = 0;
    MealTimes _mealTimes;
    DS1307 *_rtc;

    uint8_t _getAddressForIsMealTimesSet() {
      return sizeof(bool); //its "isTimeSet"
    }

    uint8_t _getAddressForIsTimerOpenFlapSet() {
      return _getAddressForIsMealTimesSet() + sizeof(bool); // its isMealTimeSet 
    }

    uint8_t _getAddressForTimerOpenFlap() {
      return _getAddressForIsTimerOpenFlapSet() + sizeof(bool); // its IsTimerOpenFlapSet; 
    }


    uint8_t _getAddressForMealTimes() {
      return _getAddressForTimerOpenFlap() + sizeof(int); // its TimerOpenFlap 
    }

    bool _getIsTimeSetFromEEPROM() {
      bool isTimeSet = false;

      EEPROM.get(addressIsTimeSet, isTimeSet);

      return isTimeSet;
    }

    bool _getIsMealTimesSetFromEEPROM() {
      bool isMealTimesSet = false;

      EEPROM.get(_getAddressForIsMealTimesSet(), isMealTimesSet);

      return isMealTimesSet;
    }

    bool _getIsTimerOpenFlapSetFromEEPROM() {
      bool isTimerOpenFlapSet = false;

      EEPROM.get(_getAddressForIsTimerOpenFlapSet(), isTimerOpenFlapSet);

      return isTimerOpenFlapSet;
    }

    uint8_t _getTimerOpenFlapFromEEPROM() {
      uint8_t timerOpenFlap;

      EEPROM.get(_getAddressForTimerOpenFlap(), timerOpenFlap);

      return timerOpenFlap;
    }
};