#include <Arduino.h>
#include <DS1307.h>
#include <Servo.h>
#include "times.h"


class Feeder {

  public:
    Feeder(Times* mealTimes, int* timerOpenFlap, DS1307* rtc, Servo* servo) {
      this->_mealTimes = mealtTimes;
      this->_timerOpenFlap = timerOpenFlap;
      this->_rtc = rtc;
    }
    void workflow() {
      if (_isFlapOpen) {
        if (_isTimeOpenedFlapPass()) {
          servo.write(0);
        }
      }
      else {
        Time now = _rtc->getTime();
        for (uint8_t i = 0; i < countMealTime; i++) {
          if (*(_mealTimes + i)->isTimeEqual(now.hour, now.min, now.sec)) {
            _timeOpenFlap = millis();
            servo.write(12);
          }
        }
      }
    }

  private:
    bool _isFlapOpen = false;
    long _timeOpenFlap = 0;
    Times* _mealTimes;
    int* _timerOpenFlap;
    DS1307* _rtc;
    Servo* _servo;


    bool _isTimeOpenedFlapPass() {
      return millis() - (_timeOpenFlap + _timerOpenFlap) >= 0;
    }
}