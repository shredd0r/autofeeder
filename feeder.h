#include <DS1307.h>
#include <Servo.h>
#include "times.h"

const uint8_t openDegree = 160;
const uint8_t closeDegree = 179;

class Feeder {

  public:
    Feeder(DS1307* rtc, Servo* servo) {
      this->_rtc = rtc;
      this->_servo = servo;
    }
 
    void updateMealTimes(MealTimes mealTimes) {
      this->_mealTimes = mealTimes;
    }

    void updateTimerOpenFlap(int timerOpenFlap) {
      this->_timerOpenFlap = timerOpenFlap;
    }

    bool openFlap(int timerOpenFlap) {
      if (_isFlapOpen) {
        if (_isTimeOpenedFlapPass(timerOpenFlap)) {
          _servo->write(closeDegree);
          _isFlapOpen = false;
          _isTimeToOpen = false;
          return true;
        }
      }
      else {
        _whenOpenFlap = millis();
        _servo->write(openDegree);
        _isFlapOpen = true;
      }

      return false;
    }

    void workflow() {
      if (_isTimeToOpen) {
        openFlap(_timerOpenFlap);
      }
      else {
        Time now = _rtc->getTime();
        _isTimeToOpen = _mealTimes.meals[index].isTimeEqual(now.hour, now.min, now.sec);
        incrementIndex();
      }
    }
  private:
    bool _isTimeToOpen = false;
    bool _isFlapOpen = false;
    long _whenOpenFlap = 0;
    MealTimes _mealTimes;
    int* _timerOpenFlap;
    uint8_t _closeDegree = 0;
    uint8_t index = 0;
    DS1307* _rtc;
    Servo* _servo;


    bool _isTimeOpenedFlapPass(int timerOpenFlap) {
      long difference = millis() - (_whenOpenFlap + long(timerOpenFlap));
      return difference >= 0;
    }

    void incrementIndex() {
      if (index < 5) {
        index++;
      } 
      else {
        index = 0;
      }
    }
};