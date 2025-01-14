#pragma once

#include <Arduino.h>

class PressedButton {

  private:
  int _debounceDelay     = 50;   // 去抖延迟
  int _longPressDuration = 1000; // 长按持续时间（毫秒）
  int _repeatDelay       = 500;  // 连续短按持续时间（毫秒）
  int _button_pin;               // 正在使用的按钮引脚号
  int           buttonState          = HIGH;  // 初始按键状态
  int           lastButtonState      = HIGH;  // 上一次的按键状态
  unsigned long lastDebounceTime     = 0;     // 上一次去抖的时间
  unsigned long pressStartTime       = 0;     // 按键按下的起始时间
  bool          longPressTriggered   = false; // 是否已经触发了长按
  bool          repeatPressTriggered = false;

  public:


  // 函数声明
  PressedButton(int pin);
  void ButtonInit(); // 引脚初始化
  int  GetPin();
  void SetPin(int pin);
  void SetDebounceDelay(int time);
  void SetLongPressDuration(int time);
  void SetRepeatDelay(int time);
  void ShortPress(void (*ptr)());
  void LongPress(void (*ptr)());
  void RepeatPress(void (*ptr)());
};

PressedButton::PressedButton(int pin) {
  _button_pin = pin;
}

void PressedButton::ButtonInit() {
  pinMode(_button_pin, INPUT_PULLUP);
}

int PressedButton::GetPin() {
  return _button_pin;
}

void PressedButton::SetPin(int pin) {
  _button_pin = pin;
}

void PressedButton::SetDebounceDelay(int time) {
  _debounceDelay = time;
}

void PressedButton::SetLongPressDuration(int time) {
  _longPressDuration = time;
}

void PressedButton::SetRepeatDelay(int time) {
  _repeatDelay = time;
}

void PressedButton::ShortPress(void (*ptr)()) {
  int reading = digitalRead(_button_pin); // 读取按键状态
  // 去抖处理
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if (millis() - lastDebounceTime > _debounceDelay) {
    // 更新按键状态
    if (reading != buttonState) {
      buttonState = reading;
      // 检测按键按下
      if (buttonState == LOW) {
        pressStartTime       = millis();
        longPressTriggered   = false;
        repeatPressTriggered = false;
      } else {
        if (!longPressTriggered && !repeatPressTriggered) {
          // 按键释放,短按触发
          // Serial.println("Short press");
          ptr();
        }
        longPressTriggered   = false;
        repeatPressTriggered = false;
      }
    }
  }
  lastButtonState = reading;
}

void PressedButton::LongPress(void (*ptr)()) {
  int reading = digitalRead(_button_pin); // 读取按键状态
  // 去抖处理
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if (millis() - lastDebounceTime > _debounceDelay) {
    // 检测长按
    if (!longPressTriggered && buttonState == LOW && millis() - pressStartTime > _longPressDuration) {
      longPressTriggered = true;
      // Serial.println("Long press");
      ptr();
    }
    repeatPressTriggered = false;
  }
  lastButtonState = reading;
}

void PressedButton::RepeatPress(void (*ptr)()) {
  int reading = digitalRead(_button_pin); // 读取按键状态
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if (millis() - lastDebounceTime > _debounceDelay) {
    if (!repeatPressTriggered && buttonState == LOW && millis() - pressStartTime > _repeatDelay) {
      repeatPressTriggered = true;
      // Serial.println("Repeat press");
      ptr();
    }
  }
  lastButtonState = reading;
}