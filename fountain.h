#include <Arduino.h>
#include <OLED_I2C.h>

class Fountain {

  public:

    Fountain(uint8_t pinMosfet, uint8_t pinWaterLevelSensor, uint8_t pinInfraredMotionSensor, OLED* oled) {
      this->_oled = oled;
      this->_pinMosfet = pinMosfet;
      this->_pinWaterLevelSensor = pinWaterLevelSensor;
      this->_pinInfraredMotionSensor = pinInfraredMotionSensor;
    }

    void workflow() {
      if (_isEnoughWater()) {
        if (_isInfraredMotionSensorWorked()) {
          digitalWrite(_pinMosfet, HIGH);

        } 
        else {
          digitalWrite(_pinMosfet, LOW);
        }
      } 
      else {
        _oled->clrScr();
        _oled->print("Water level low!!!", CENTER, 10);
        _oled->print("Add water", CENTER, 10);
        _oled->update();
      }
      _oled->update();
    }

  private:
    OLED* _oled;
    uint8_t _pinMosfet;
    uint8_t _pinWaterLevelSensor;
    uint8_t _pinInfraredMotionSensor;

    bool _isEnoughWater() {
      return analogRead(_pinWaterLevelSensor) > 100; // 0 ... 1023;

    } 

    bool _isInfraredMotionSensorWorked() {
      int value = digitalRead(_pinInfraredMotionSensor);

      return value == HIGH;
    }
};