#include <Arduino.h>
#include "option.hpp"

#include "sensorCheck.hpp"
#include "degCalculation.hpp"

static const int NOT_BALL_FOUND = 900;

bool ballFound = false;
float theta = 0.0;

class IR // 個々のIRもろもろ
{
public:
    int pin = -1;
    int val = -1;
    float deg = -1;
};
IR myIR[8];

const int IRsensor_pin[8] = {1, 2, 3, 4, 5, 8, 9, 10};

// 移動平均クラス新ファミリーの宣言
static MovingAverage vxMA;
static MovingAverage vyMA;
static MovingAverage degMA;

float ballAngle;
const int DAC_PIN = A0;

static float deg_radian(float degree)
{
    return degree * M_PI / 180.0;
}

void DegCalculation()
{
    //
    //=======================================================================================
    // 値の更新
    //=======================================================================================
    //
    IR minIR; // 最小値の決定
    minIR.val = 1023;

    for (int i = 0; i < 8; i++)
    {
        myIR[i].pin = IRsensor_pin[i];
        myIR[i].val = analogRead(myIR[i].pin);
        myIR[i].deg = i * 45;

        if (myIR[i].val < minIR.val)
        {
            minIR.pin = myIR[i].pin;
            minIR.val = myIR[i].val;
            minIR.deg = myIR[i].deg;
        }
    }

    for (int i = 0; i < IR_COUNT; i++)
    {
        Serial.print(myIR[i].val);
        Serial.print(" ");
    }
    Serial.print("-> ");
    Serial.print(minIR.val);
    Serial.print(" ");

    // -minval900よりも小さいならballFoundがtrueにする-
    if (minIR.val > NOT_BALL_FOUND)
    {
        ballFound = false;
    }
    else
    {
        ballFound = true;
    }

    //
    //=======================================================================================
    // ベクトル合成
    //=======================================================================================
    //

    float vx = 0.0, vy = 0.0;

    for (int pin = 0; pin < IR_COUNT; pin++)
    {
        float w = 1.0 / (myIR[pin].val + 1);
        float ang = deg_radian(myIR[pin].deg);
        vx += w * cos(ang);
        vy += w * sin(ang);
    }

    float smoothVX = vxMA.update(vx);
    float smoothVY = vyMA.update(vy);

    theta = atan2(smoothVY, smoothVX) * 180.0 / M_PI;
    if (theta < 0.0)
        theta += 360.0;

    ballAngle = (theta / 360.0) * 1024; // C-styleに送る用

    if (ballFound)
    {
        Serial.print(ballAngle);
        analogWrite(DAC_PIN, ballAngle); // DAC_PINに値を送る。
    }
    else
    {
        Serial.print("ボールが見つかりません笑");
    }
}