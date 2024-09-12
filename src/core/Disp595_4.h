#pragma once
#include <Arduino.h>
#include <GyverIO.h>

#include "Driver595.h"
#include "utils/SegBuffer.h"

class Disp595_4 : public Driver595, public SegBuffer {
   public:
    // пины DIO, SCLK, RCLK
    Disp595_4(uint8_t DIO, uint8_t SCLK, uint8_t RCLK) : Driver595(DIO, SCLK, RCLK), SegBuffer(buffer, 4, 1) {
        update();
    }

    // обновить дисплей
    void update() {
        for (uint8_t i = 0; i < 4; i++) {
            _buf[i] = ~buffer[4 - 1 - i];
        }
    }
    
    // тикер динамической индикации, вызывать в loop постоянно
    uint8_t tick() {
        Driver595::tick(_buf, 4);
        return 0;
    }

    // тикер динамической индикации, вызывать по своему таймеру
    uint8_t tickManual() {
        Driver595::tickManual(_buf, 4);
        return 0;
    }

    uint8_t buffer[4] = {0};

   private:
    uint8_t _buf[4] = {0};
};