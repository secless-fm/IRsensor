#include "option.hpp"

float NormalizeAngle(float angle)
{
    while (angle > 180.0)
        angle -= 360.0;
    while (angle < -180.0)
        angle += 360.0;
    return angle;
}

void Serial_start()
{
    while (!Serial)
        ;

    Serial.println("Serial start    ");
}