class Button {
  public:
    Button (byte pin) {
      _pin = pin;
      pinMode(_pin, INPUT_PULLUP);
    }
    bool isClicked() {
      bool btnState = digitalRead(_pin);
      if (!btnState && !_flag && millis() - _timer >= 10) {
        _flag = true;
        _timer = millis();
        return true;
      }
      if (!btnState && _flag && millis() - _timer >= 30) {
        _timer = millis();
        return true;
      }
      if (btnState && _flag) {
        _flag = false;
        _timer = millis();
      }
      return false;
    }
  private:
    byte _pin;
    uint32_t _timer;
    bool _flag;
};
