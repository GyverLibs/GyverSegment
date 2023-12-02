#pragma once
#include <Arduino.h>
#include <GyverIO.h>

#include "utils/SegBuffer.h"
#include "utils/SegDuty.h"

// задержка клока в мкс

#ifndef DISP595_CLK_DELAY
#define DISP595_CLK_DELAY 0
#endif

template <uint8_t amount>
class Disp595Static : public SegDuty, public SegBuffer {
   public:
    Disp595Static(uint8_t DIO, uint8_t SCLK, uint8_t RCLK) : SegBuffer(buffer, amount, 1) {
        _dio = DIO;
        _clk = SCLK;
        _latch = RCLK;
        pinMode(_dio, OUTPUT);
        pinMode(_clk, OUTPUT);
        pinMode(_latch, OUTPUT);
        update();
    }

    // управление питанием (true вкл, false выкл)
    void power(bool state) {
        if (state) update();
        else _clear();
        _power = state;
    }

    // обновить дисплей
    void update() {
        gio::low(_latch);
        for (uint8_t i = 0; i < amount; i++) {
            gio::shift::send_byte(_dio, _clk, MSBFIRST, ~buffer[amount - i - 1], DISP595_CLK_DELAY);
        }
        gio::high(_latch);
        _on_f = 1;
    }

    uint8_t tick() {
        if (!_power) return 0;
        if (SegDuty::isMax()) {
            if (!_on_f) update();
        } else {
            if (SegDuty::tout()) tickManual();
        }
        return 0;
    }

    uint8_t tickManual() {
        if (!_power) return 0;
        if (SegDuty::skip()) return 0;

        if (_on_f && SegDuty::begin()) _clear();
        else update();
        return 0;
    }

    uint8_t buffer[amount] = {0};

   private:
    uint8_t _dio, _clk, _latch;
    bool _power = 1;
    bool _on_f = 0;

    void _clear() {
        gio::low(_latch);
        for (uint8_t i = 0; i < amount; i++) {
            gio::shift::send_byte(_dio, _clk, MSBFIRST, 0xff, DISP595_CLK_DELAY);
        }
        gio::high(_latch);
        _on_f = 0;
    }
};