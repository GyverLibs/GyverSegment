#pragma once
#include <Arduino.h>
#include <GyverIO.h>

#include "Driver595.h"
#include "utils/SegBuffer.h"

class Disp595_8 : public Driver595, public SegBuffer {
   public:
    // пины DIO, SCLK, RCLK
    Disp595_8(uint8_t DIO, uint8_t SCLK, uint8_t RCLK) : Driver595(DIO, SCLK, RCLK), SegBuffer(buffer, 8, 1) {
        update();
    }

    // обновить дисплей
    void update() {
        for (uint8_t i = 0; i < 8; i++) {
            _buf[i] = ~buffer[7 - i];
        }
    }

    // тикер динамической индикации, вызывать в loop постоянно
    uint8_t tick() {
        Driver595::tick(_buf, 8);
        return 0;
    }

    // тикер динамической индикации, вызывать по своему таймеру
    uint8_t tickManual() {
        Driver595::tickManual(_buf, 8);
        return 0;
    }

    uint8_t buffer[8] = {0};

   private:
    uint8_t _buf[8] = {0};
};