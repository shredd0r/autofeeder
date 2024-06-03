#include <DS1307.h>
#include <EEPROM.h>

const uint8_t countMealTime = 5;

struct Times {
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;

  Times() {
    hours = -1;
    minutes = -1;
    seconds = -1;
  }

  Times(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    this->hours = hours;
    this->minutes = minutes;
    this->seconds = seconds;
  }
};

class Settings {
  public:
    bool isTimeSetup() {
      return this->_isTimeSet;
    }
    void setupTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
      //add call DS1307.setTime();
      int index = this->_getIndex(_indexOfIsTimeSetInMemory);
      
      this->_isTimeSet = true;
      EEPROM.put(index, this->_isTimeSet);
    }
    void setupMealTime(Times *mealTimes) {
      this->_mealTimes[countMealTime] = *mealTimes;
      EEPROM.put(this->_getIndex(_indexOfMealTimesInMemory), this->_mealTimes);
    }
    void editMealTime(uint8_t index, Times newMealTime) {
      this->_mealTimes[index] = newMealTime;
      uint8_t size = sizeof(this->_mealTimes[index]);

      EEPROM.put(index * size, newMealTime);
    }
    void deleteMealTime(uint8_t index) {
      Times *mealTimeForDel = &(this->_mealTimes[index]);
      this->_mealTimes[index] = Times();

      uint8_t size = sizeof(*mealTimeForDel);
      EEPROM.put(size * index, Times());
    }
    void removeMealTimes() {
      uint8_t size = sizeof(this->_mealTimes);
      delete _mealTimes;

      for (uint8_t i = _indexOfMealTimesInMemory; i < size + _indexOfMealTimesInMemory; i++) {
        EEPROM.write(i, 0);
      }
    }

    Times* getMealTimes() {
      return this->_mealTimes;
    }
    
    Times* getMealTime(uint8_t index) {
      return &this->_mealTimes[index];
    }
    
    uint8_t getTimerOpenFlap() {
      return this->_timerOpenFlap;
    }

    Settings(/*DS1307 *rtc*/) {
      _mealTimes = new Times[countMealTime];
      // this->_rtc = rtc;
    }

  private:
    const uint8_t _indexOfMealTimesInMemory = 0;
    const uint8_t _indexOfTimerOpenFlapInMemory = 1;
    const uint8_t _indexOfIsTimeSetInMemory = 2;

    bool _isTimeSet = false;
    uint8_t _timerOpenFlap = 0;
    Times* _mealTimes;
    // DS1307 *_rtc;

    int _getIndex(uint8_t indexOfParam) {
      switch(indexOfParam) {
        case 0:
          return 0;
          break;
        case 1:
          return sizeof(this->_mealTimes);
          break;
        case 2:
          return sizeof(this->_mealTimes) + sizeof(this->_timerOpenFlap);
          break;
        default:
          return sizeof(this->_mealTimes) + sizeof(this->_timerOpenFlap) + sizeof(this->_isTimeSet);
      }
    }
};
