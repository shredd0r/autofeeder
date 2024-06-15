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

  bool isTimeSet() {
    return hours != -1;
  }

  bool isTimeEqual(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    return this->hours == hours &&
    this->minutes == minutes &&
    this->seconds == seconds;
  }
};