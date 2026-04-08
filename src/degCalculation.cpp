#include <Arduino.h>
#include "sensorCheck.hpp"
#include "degCalculation.hpp"

static const int NOT_BALL_FOUND = 900;

static int irValue[11];
bool ballFound = false;
static float theta = 0.0;

float ballAngle;

static float deg_radian(int index)
{
    static const double Deg[] = {
    0.0, 45.0, 90.0, 135.0, 0.0, 0.0,   // 6,7番目は無意味
    180.0, 225.0, 270.0, 315.0};
    return Deg[index] * M_PI / 180.0;
}

void DegCalculation(){
    int minval = 900;

    for (int pin = IR_FIRST_PIN; pin <= IR_LAST_PIN; pin++)
    {
        if (pin == 6 || pin == 7)
        {
            continue;
        }
        int val = analogRead(pin);
        irValue[pin] = val;
        if (minval > val)
        {
        minval = val;
        }
    }

    if (minval > NOT_BALL_FOUND)
    {
        ballFound = false;
    }
    else
    {
        ballFound = true;
    }

    float vx = 0.0, vy = 0.0;

    for (int pin = IR_FIRST_PIN; pin <= IR_LAST_PIN; pin++)
    {
        if (pin == 6 || pin == 7)
        {
            continue;
        }
        float w = 1.0 / (irValue[pin] + 1);
        float ang = deg_radian(pin);
        vx += w * cos(ang);
        vy += w * sin(ang);
    }

    theta = atan2(vy, vx) * 180.0 / M_PI;
    if (theta < 0.0)
    theta += 360.0;

    ballAngle = (theta / 360) * 1024;   // C-styleに送る用

    Serial.print(theta);
}