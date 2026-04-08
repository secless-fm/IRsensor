#include <Arduino.h>
#include <math.h>

#include "option.hpp"
#include "sensorCheck.hpp"
#include "degCalculation.hpp"

#define ValueCheck 0 // 1: センサの値チェック 0: 角度を出す コメントアウト:3つ目の処理（未実装）

const int DAC_PIN = A0;


void setup()
{
  Serial.begin(115200);

  Serial_start();
}

void loop()
{
#ifdef ValueCheck

  #if ValueCheck
  SensorCheck();
  #else
  DegCalculation();
  #endif

#else

#endif

  if (ballFound)
  {
    analogWrite(DAC_PIN, ballAngle); // DAC_PINに値を送る。
  }
  else
  {
    Serial.print("ボールが見つかりません笑");
  }

  Serial.println();

  delay(100);
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