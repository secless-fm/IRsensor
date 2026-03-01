#include <Arduino.h>
#include <math.h>

static const int IR_FIRST_PIN = 3;
static const int IR_LAST_PIN = 11;
static const int IR_COUNT = 8;
static const int NOT_BALL_FOUND = 900;

static int irValue[11];
static bool ballFound = false;
static float theta = 0.0;

const int IR_pin = 0;

static float deg_radian(int index)
{
  static const int Deg[IR_COUNT]{
    0.0, 45.0, 90.0, 135.0,
    180.0, 225.0, 270.0, 315.0
  };
  return Deg[index] * M_PI / 180.0; 
}


void setup() {
  Serial.begin(115200);


}

void loop() {
  int minval = 900;

  for (int pin = IR_FIRST_PIN; pin < IR_LAST_PIN; pin ++)
  {
    int val = analogRead(pin);
    irValue[pin] = val;
    if (minval > val) {
      minval = val;
    }
  }

  float vx = 0.0,vy = 0.0;

  for (int pin = IR_FIRST_PIN; pin < IR_LAST_PIN; pin ++)
  {
    float w = 1.0 / (irValue[pin] + 1);
    float ang = deg_radian(pin - IR_FIRST_PIN);
    vx += w * cos(ang);
    vy += w * sin(ang);
  }

  theta = atan2(vy,vx) * 180.0 / M_PI;
  if (theta < 0.0) theta += 360.0;

  float ballAngle = (theta / 360) * 100; // C-styleに送る用

  if(ballFound) {
    analogWrite(IR_pin, ballAngle); // IR_pinに値を送る。
  }else{
      Serial.print("ボールが見つかりません笑");
  }
}

/* ==変数・定数・関数一覧==
IR_FIRST_PIN = IRの最初のピン番号
IR_LAST_PIN = IRの最後のピン番号
IR_COUNT = IRセンサの総数
NOT_BOLL_FOUND = ボールが見つからないと判断する最小の仮値

irValue[] = その時々のセンサの値の格納庫。[]の中の数はピン番号
ballFound = ボールがあると、trueになる。ボールがないとfalseとなり、プログラムの最初に戻る。デフォルトはfalse
theta = ボールの角度。θ

deg_radian() = ()の中のピン番号と接続してあるセンサのIR_FIRST_PINを0°とした向いている角度。
あとはまあフィーリングで考えて。
== ==*/