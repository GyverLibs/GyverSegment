#pragma once
#include <Arduino.h>

// модуль яркости для динамической индикации

class SegDuty {
   public:
    // установить яркость (0.. maxDuty, умолч 15)
    void brightness(uint8_t bright) {
        _duty = min(bright, _maxDuty);
    }

    // установить макс. значение яркости (умолч. 15)
    // 0 чтобы отключить модуль яркости (каждый tick меняет сегмент)
    void maxDuty(uint8_t duty) {
        _maxDuty = duty;
        brightness(_duty);
    }

    // получить яркость
    uint8_t getBrightness() {
        return _duty;
    }

    // получить макс. значение яркости
    uint8_t getMaxDuty() {
        return _maxDuty;
    }

   protected:
    uint8_t tick() {
        if (_count) {
            if (_count-- == _duty) return 1;  // next+on
        } else {
            _count = _maxDuty;
            return 2;  // off
        }
        return 0;  // idle
    }

   private:
    uint8_t _count = 0, _duty = 15, _maxDuty = 15;
};