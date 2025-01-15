#include "batteryReading.hpp"
#include "leg.hpp"
#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <esp_now.h>
#include <esp_wifi.h>

/************************************************************* ESP NOW ******************************************************************/

uint8_t             controlPadAddress[] = { 0x08, 0xa6, 0xf7, 0x17, 0x6d, 0x84 };
esp_now_peer_info_t quadrupedRobot;

struct Pad {
  int joystickValue[4] = {}; // 0、右摇杆X轴（上下），1、右摇杆Y轴（左右），2、左摇杆X轴（油门），3、左摇杆Y轴（左右）。
};
Pad pad;

struct Robot {
  int position[4]      = {}; // L1 L2 R1 R2位置信息
  int batteryStatus[2] = {}; // 电压、电量
};
Robot robot;

bool esp_connected;

/**************************************************************** LEG ****************************************************************/

int L1_in_1         = 14; // 高电平顺时针转动（往上抬）
int L1_in_2         = 13; // 高电平逆时针
int L1_position_pin = 9;  // 站立中值123 顺时针（往上抬）读数增加
int L1_stand_val    = 121;
int L1_pwm_channel1 = 0;
int L1_pwm_channel2 = 1;

int L2_in_1         = 10;
int L2_in_2         = 8; // 高电平逆时针（往后摆）
int L2_position_pin = 5; // 站立中值115
int L2_stand_val    = 124;
int L2_pwm_channel1 = 2;
int L2_pwm_channel2 = 3;

int R1_in_1         = 17; // 高电平往上抬
int R1_in_2         = 15;
int R1_position_pin = 1; // 站立中值173
int R1_stand_val    = 135;
int R1_pwm_channel1 = 4;
int R1_pwm_channel2 = 5;

int R2_in_1         = 34;
int R2_in_2         = 21;
int R2_position_pin = 4; // 站立中值147
int R2_stand_val    = 66;
int R2_pwm_channel1 = 6;
int R2_pwm_channel2 = 7;

int deviation = 3;

int freqency   = 500;
int resolution = 8;

Leg leg_L1(L1_in_1, L1_in_2, L1_position_pin, L1_stand_val);
Leg leg_L2(L2_in_1, L2_in_2, L2_position_pin, L2_stand_val);
Leg leg_R1(R1_in_1, R1_in_2, R1_position_pin, R1_stand_val);
Leg leg_R2(R2_in_1, R2_in_2, R2_position_pin, R2_stand_val);

/*************************************************************** 电池 *****************************************************************/

int   batteryPin = 6;
int   r1         = 10000;
int   r2         = 10000;
int   avg_time   = 50;
float maxValue   = 4.2;
float minValue   = 3.2;

// typedef struct {
//   float batvolts, voltsPercentage;
// } Bat;

BatReading battery;

/********************************************************************************************************************************/

void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    esp_connected = true;
  } else {
    esp_connected = false;
  }
}
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  memcpy(&pad, incomingData, sizeof(pad));
}

void dataSend() {
  BatReading::Bat batteryStatus = battery.read(avg_time);
  robot.position[0]      = leg_L1.getCurrentPosition();
  robot.position[1]      = leg_L2.getCurrentPosition();
  robot.position[2]      = leg_R1.getCurrentPosition();
  robot.position[3]      = leg_R2.getCurrentPosition();
  robot.batteryStatus[0] = batteryStatus.voltage;
  robot.batteryStatus[1] = batteryStatus.voltsPercentage;
  // 发送
  esp_now_send(controlPadAddress, (uint8_t*)&robot, sizeof(robot));
}

void dataPrint() {
  Serial.printf("L1: %d\n", analogRead(L1_position_pin));
  Serial.printf("L2: %d\n", analogRead(L2_position_pin));
  Serial.printf("R1: %d\n", analogRead(R1_position_pin));
  Serial.printf("R2: %d\n", analogRead(R2_position_pin));
}

void standup() {
  leg_L1.moving(255, L1_stand_val);
  leg_L2.moving(255, L2_stand_val);
  leg_R1.moving(255, R1_stand_val);
  leg_R2.moving(255, R2_stand_val);
}

void setup() {
  Serial.begin(115200);
  esp_now_init();
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  memcpy(quadrupedRobot.peer_addr, controlPadAddress, 6); // 设置配对设备的MAC地址并储存，参数为拷贝地址、拷贝对象、数据长度
  quadrupedRobot.channel = 1;                             // 设置通信频道
  esp_now_add_peer(&quadrupedRobot);                      // 添加通信对象

  leg_L1.init(L1_pwm_channel1, L1_pwm_channel2, freqency, resolution);
  leg_L2.init(L2_pwm_channel1, L2_pwm_channel2, freqency, resolution);
  leg_R1.init(R1_pwm_channel1, R1_pwm_channel2, freqency, resolution);
  leg_R2.init(R2_pwm_channel1, R2_pwm_channel2, freqency, resolution);

  battery.init(batteryPin, resolution, r1, r2, maxValue, minValue);
}

void loop() {
  standup();
}
