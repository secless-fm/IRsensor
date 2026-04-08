#pragma once

#include <Arduino.h>

float NormalizeAngle(float angle); // 角度を 0°~360° → -180°~180° に変換

void Serial_start(); // シリアル起動確認

class MovingAverage
{
private:
    static const int num_elements = 10;      // サイズ定数(回数)
    float storage[num_elements] = {0}; // バッファを0で初期化
    int current_target = 0;
    int count = 0;

public:
    float update(float now_val)
    {
        // 1. 新しい値をバッファに書き込む (循環バッファ)
        storage[current_target] = now_val;
        current_target = (current_target + 1) % num_elements;

        // 2. データ点数のカウント（上限はウィンドウサイズ）
        if (count < num_elements)
            count++;

        // 3. 移動平均の計算
        int sum = 0;
        // 蓄積されたデータ点数分だけ合計する
        for (int i = 0; i < count; i++)
        {
            sum += storage[i];
        }

        // 整数除算で平均を出す
        return sum / count;
    }
};