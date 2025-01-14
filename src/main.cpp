#include <Arduino.h>
#include "leg.hpp"

int L1_IN_1 = 14;         // 高电平顺时针转动（往上抬）
int L1_IN_2 = 13;         // 高电平逆时针
int L1_POSITION_PIN = 9;  // 站立中值123 顺时针（往上抬）读数增加
int L1_STAND_VAL = 121;
int L1_PWM_CHANNEL1 = 0;
int L1_PWM_CHANNEL2 = 1;

int L2_IN_1 = 10;
int L2_IN_2 = 8;          // 高电平逆时针（往后摆）
int L2_POSITION_PIN = 5;  // 站立中值115
int L2_STAND_VAL = 124;
int L2_PWM_CHANNEL1 = 2;
int L2_PWM_CHANNEL2 = 3;

int R1_IN_1 = 17;  // 高电平往上抬
int R1_IN_2 = 15;
int R1_POSITION_PIN = 1;  // 站立中值173
int R1_STAND_VAL = 135;
int R1_PWM_CHANNEL1 = 4;
int R1_PWM_CHANNEL2 = 5;

int R2_IN_1 = 34;
int R2_IN_2 = 21;
int R2_POSITION_PIN = 4;  // 站立中值147
int R2_STAND_VAL = 66;
int R2_PWM_CHANNEL1 = 6;
int R2_PWM_CHANNEL2 = 7;

int DEVIATION = 3;

int FREQENCY = 500;
int RESOLUTION = 8;

Leg leg_L1(L1_IN_1, L1_IN_2, L1_POSITION_PIN, L1_STAND_VAL);
Leg leg_L2(L2_IN_1, L2_IN_2, L2_POSITION_PIN, L2_STAND_VAL);
Leg leg_R1(R1_IN_1, R1_IN_2, R1_POSITION_PIN, R1_STAND_VAL);
Leg leg_R2(R2_IN_1, R2_IN_2, R2_POSITION_PIN, R2_STAND_VAL);

void standup() {
  leg_L1.moving(255, L1_STAND_VAL);
  leg_L2.moving(255, L2_STAND_VAL);
  leg_R1.moving(255, R1_STAND_VAL);
  leg_R2.moving(255, R2_STAND_VAL);

  // if (analogRead(L1_POSITION_PIN) < L1_STAND_VAL) {
  //   digitalWrite(L1_IN_1, HIGH);
  //   digitalWrite(L1_IN_2, LOW);
  // } else if (analogRead(L1_POSITION_PIN) > (L1_STAND_VAL + DEVIATION)) {
  //   digitalWrite(L1_IN_1, LOW);
  //   digitalWrite(L1_IN_2, HIGH);
  // } else {
  //   digitalWrite(L1_IN_1, HIGH);
  //   digitalWrite(L1_IN_2, HIGH);
  // }

  // if (analogRead(L2_POSITION_PIN) < L2_STAND_VAL) {
  //   digitalWrite(L2_IN_1, HIGH);
  //   digitalWrite(L2_IN_2, LOW);
  // } else if (analogRead(L2_POSITION_PIN) > (L2_STAND_VAL + DEVIATION)) {
  //   digitalWrite(L2_IN_1, LOW);
  //   digitalWrite(L2_IN_2, HIGH);
  // } else {
  //   digitalWrite(L2_IN_1, HIGH);
  //   digitalWrite(L2_IN_2, HIGH);
  // }

  // if (analogRead(R1_POSITION_PIN) < R1_STAND_VAL) {
  //   digitalWrite(R1_IN_1, HIGH);
  //   digitalWrite(R1_IN_2, LOW);
  // } else if (analogRead(R1_POSITION_PIN) > (R1_STAND_VAL + DEVIATION)) {
  //   digitalWrite(R1_IN_1, LOW);
  //   digitalWrite(R1_IN_2, HIGH);
  // } else {
  //   digitalWrite(R1_IN_1, HIGH);
  //   digitalWrite(R1_IN_2, HIGH);
  // }

  // if (analogRead(R2_POSITION_PIN) < R2_STAND_VAL) {
  //   digitalWrite(R2_IN_1, HIGH);
  //   digitalWrite(R2_IN_2, LOW);
  // } else if (analogRead(R2_POSITION_PIN) > (R2_STAND_VAL + DEVIATION)) {
  //   digitalWrite(R2_IN_1, LOW);
  //   digitalWrite(R2_IN_2, HIGH);
  // } else {
  //   digitalWrite(R2_IN_1, HIGH);
  //   digitalWrite(R2_IN_2, HIGH);
  // }
}

void setup() {
  Serial.begin(115200);

  // pinMode(L1_IN_1, OUTPUT);
  // pinMode(L1_IN_2, OUTPUT);

  // pinMode(L2_IN_1, OUTPUT);
  // pinMode(L2_IN_2, OUTPUT);

  // pinMode(R1_IN_1, OUTPUT);
  // pinMode(R1_IN_2, OUTPUT);

  // pinMode(R2_IN_1, OUTPUT);
  // pinMode(R2_IN_2, OUTPUT);

  // analogReadResolution(8);


  // digitalWrite(R2_IN_1, HIGH);
  // delay(200);
  // digitalWrite(R2_IN_1, LOW);
  leg_L1.init(L1_PWM_CHANNEL1, L1_PWM_CHANNEL2, FREQENCY, RESOLUTION);
  leg_L2.init(L2_PWM_CHANNEL1, L2_PWM_CHANNEL2, FREQENCY, RESOLUTION);
  leg_R1.init(R1_PWM_CHANNEL1, R1_PWM_CHANNEL2, FREQENCY, RESOLUTION);
  leg_R2.init(R2_PWM_CHANNEL1, R2_PWM_CHANNEL2, FREQENCY, RESOLUTION);
}

void loop() {
  standup();

  // digitalWrite(L1_IN_1, HIGH);
  // digitalWrite(L1_IN_2, HIGH);
  // digitalWrite(L2_IN_1, HIGH);
  // digitalWrite(L2_IN_2, HIGH);
  // digitalWrite(R1_IN_1, HIGH);
  // digitalWrite(R1_IN_2, HIGH);
  // digitalWrite(R2_IN_1, HIGH);
  // digitalWrite(R2_IN_2, HIGH);

  // Serial.printf("L1: %d\n", analogRead(L1_POSITION_PIN));
  // Serial.printf("L2: %d\n", analogRead(L2_POSITION_PIN));
  // Serial.printf("R1: %d\n", analogRead(R1_POSITION_PIN));
  // Serial.printf("R2: %d\n", analogRead(R2_POSITION_PIN));
  // delay(5000);
}
