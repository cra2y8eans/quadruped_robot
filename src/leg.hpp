#include <driver/ledc.h>

#pragma once
#include <Arduino.h>

class Leg {
  private:
  int       _out1, _out2, _positionPin, _channel1, _channel2, _resolution, _freqency, _standValue;
  int       _adcMax, _adcMin;
  const int _destinationMax = 40;
  const int _destinationMin = -40;

  public:
  Leg(int out1, int out2, int positionPin, int standValue);
  ~Leg();
  void init(int channel1, int channel2, int freqency, int resolution);
  int  getCurrentPosition();
  int  getMoveSteps(int joyStickValue);
  void motorBreak();
  void moving(int pwm, int joyStickValue);
};

Leg::Leg(int out1, int out2, int positionPin, int standValue) {
  _out1        = out1;
  _out2        = out2;
  _positionPin = positionPin;
  _standValue  = standValue;
}
Leg::~Leg() { }

void Leg::init(int channel1, int channel2, int freqency, int resolution) {
  _channel1   = channel1;
  _channel2   = channel2;
  _freqency   = freqency;
  _resolution = resolution;
  _adcMax     = pow(2, _resolution);
  _adcMin     = 0;
  analogReadResolution(_resolution);
  ledcSetup(_channel1, _freqency, _resolution);
  ledcSetup(_channel2, _freqency, _resolution);
  ledcAttachPin(_out1, _channel1);
  ledcAttachPin(_out2, _channel2);
}

int Leg::getCurrentPosition() {
  return analogRead(_positionPin);
}

int Leg::getMoveSteps(int joyStickValue) {
  int val;
  if (joyStickValue < 120) {
    val = map((joyStickValue - 127), _adcMin, _adcMax, _destinationMin, 0);
  } else if (joyStickValue > 135) {
    val = map((joyStickValue - 127), _adcMin, _adcMax, 0, _destinationMax);
  } else {
    val = 0;
  }
  return constrain(val, -_destinationMin, _destinationMax);
}

void Leg::motorBreak() {
  ledcWrite(_channel1, _adcMax);
  ledcWrite(_channel2, _adcMax);
}

void Leg::moving(int pwm, int joyStickValue) {
  int steps = this->getMoveSteps(joyStickValue);
  Serial.printf("步长：%d\n", steps);
  int current_position = this->getCurrentPosition();
  Serial.printf("当前位置：%d\n", current_position);
  if (current_position < (steps + _standValue)) {
    ledcWrite(_out1, pwm);
    ledcWrite(_out2, _adcMin);
  } else if (current_position > (steps + _standValue)) {
    ledcWrite(_out1, _adcMin);
    ledcWrite(_out2, pwm);
  } else {
    this->motorBreak();
  }
}
