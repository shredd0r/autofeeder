#ifndef settings_h
#define settings_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define countMealTime = 5;

class Settings {
  public:
    bool isSettingsSetup();
    bool isTimeSetup();
    void setupTime();
    void setupMealTime(MealTime[countMealTime] mealTimes);
    void editMealTime(int index, MealTime newMealTime);
    void deleteMealTime(int index);
    void removeMealTimes();
    MealTime[countMealTime] getMealTimes();
    MealTime* get(int index);

    Settings() {}

  private:
    int _timerOpenFlap;
    MealTime[countMealTime] _mealTimes;
}

struct MealTime {
  int hours;
  int minutes;
  int seconds;
}