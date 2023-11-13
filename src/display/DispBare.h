#pragma once
#include <Arduino.h>
#include <GyverIO.h>

#include "utils/SegBuffer.h"
#include "utils/SegDuty.h"

template <uint8_t digits, bool decimal = 1, bool anode = 0>
class DispBare : public SegBuffer, public SegDuty {
   public:
    // указатель на массив цифр (по кол-ву цифр), указатель на массив пинов сегментов
    DispBare(uint8_t* dig, uint8_t* seg) : SegBuffer(buffer, digits, decimal) {
        _dig = dig;
        _seg = seg;
        for (uint8_t i = 0; i < digits; i++) {
            pinMode(_dig[i], OUTPUT);
            gio::write(_dig[i], !anode);
        }
        for (uint8_t i = 0; i < 7 + decimal; i++) {
            pinMode(_seg[i], OUTPUT);
            gio::write(_seg[i], anode);
        }
    }

    // управление питанием (true вкл, false выкл)
    void power(bool state) {
        if (!state) {
            for (uint8_t i = 0; i < digits; i++) gio::write(_dig[i], 0);
            for (uint8_t i = 0; i < 7 + decimal; i++) gio::write(_seg[i], 0);
        }
        _power = state;
    }

    // тикер динамической индикации, вызывать в loop
    void tick() {
        if (!_power) return;
        // no bright
        if (!getMaxDuty()) {
            gio::write(_dig[_count], !anode);  // off
            if (++_count >= digits) _count = 0;
            _write(_buf[_count]);
            gio::write(_dig[_count], anode);  // on
            return;
        }

        uint8_t br = SegDuty::tick();
        if (!br) return;

        if (br == 1) {  // next + on
            if (++_count >= digits) _count = 0;
            _write(_buf[_count]);
            gio::write(_dig[_count], anode);   // on
        } else {                               // cur off
            gio::write(_dig[_count], !anode);  // off
        }
    }

    // обновить дисплей
    void update() {
        memcpy(_buf, buffer, digits);
    }

    uint8_t buffer[digits] = {0};

   private:
    void _write(uint8_t data) {
        if (anode) data = ~data;
        for (uint8_t i = 0; i < 7 + decimal; i++) {
            gio::write(_seg[i], data & 1);
            data >>= 1;
        }
    }

    uint8_t _buf[digits] = {0};
    uint8_t *_dig, *_seg;
    uint8_t _count = 0;
    bool _power = 1;
};