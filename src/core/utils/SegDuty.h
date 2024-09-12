#pragma once
#include <Arduino.h>

#ifndef GS_EXP_TIME
#define GS_EXP_TIME 100
#endif

#define GS_MAX_DUTY 15

// модуль яркости для динамической индикации

class SegDuty {
   public:
    // установить яркость (0.. 15)
    void brightness(uint8_t bright) {
        _top = GS_MAX_DUTY - min(bright, (uint8_t)GS_MAX_DUTY);
    }

    // получить яркость
    uint8_t getBrightness() {
        return GS_MAX_DUTY - _top;
    }

    // (устарело!) установить макс. значение яркости (умолч. 15)
    void maxDuty(uint8_t duty) {
    }

    // (устарело!) получить макс. значение яркости
    uint8_t getMaxDuty() {
        return GS_MAX_DUTY;
    }

   protected:
    // true - яркость не макс, нужно выключить дисплей
    bool begin() {
        if (_top) {
            _count = _top * (255 / GS_MAX_DUTY);
            _count = ((uint16_t)_count * _count + 255) >> 8;  // ^2 гамма
            _tmr = micros();
        }
        return _top;
    }

    // true - нужно пропустить обновление
    bool skip() {
        if (_top && _count) {
            _count--;
            return 1;
        }
        return 0;
    }

    // true - прошло время GS_EXP_TIME
    bool tout() {
        if (_top) {  // яркость не макс - таймер GS_EXP_TIME мкс
            uint16_t us = micros();
            if ((uint16_t)(us - _tmr) >= GS_EXP_TIME) {
                _tmr = us;
                return 1;
            }
        } else {  // яркость макс - таймер 1 мс
            uint16_t ms = millis();
            if (_tmr != ms) {
                _tmr = ms;
                return 1;
            }
        }
        return 0;
    }

    bool isMax() {
        return !_top;
    }

   private:
    // _top == 0 - макс яркость
    uint8_t _count = 0, _top = 0;
    uint16_t _tmr;
};