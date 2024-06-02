#include "Settings.h"

bool Settings::isSettingsSetup() {

}

bool Settings::isTimeSetup() {

}

void Settings::setupTime() {

}

void Settings::setupMealTime(MealTime[countMealTime] mealTimes) {

}
    
void Settings::editMealTime(int index, MealTime newMealTime) {

}
    
void Settings::deleteMealTime(int index) {

}

void Settings::removeMealTimes() {

}
    
Settings::MealTime[Settings::countMealTime] Settings::getMealTimes() {

}
    
Settings::MealTime* Settings::get(int index) {
    return &this._mealTimes[index];
}