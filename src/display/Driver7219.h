#pragma once
#include <Arduino.h>
#include <GyverIO.h>

// задержка клока в мкс
#ifndef DISP7219_CLK_DELAY
#define DISP7219_CLK_DELAY 0
#endif

#define _GSEG_7219_DECODE 0x09
#define _GSEG_7219_INTENSITY 0x0a
#define _GSEG_7219_SCAN_LIM 0x0b
#define _GSEG_7219_SHUTDOWDN 0x0c
#define _GSEG_7219_TEST_MODE 0x0f

class Driver7219 {
   public:
    // пины DIN, CLK, CS, количество микросхем (8 цифр на микросхему)
    Driver7219(uint8_t DIN, uint8_t CLK, uint8_t CS, uint8_t amount) {
        _cs = CS;
        _dat = DIN;
        _clk = CLK;
        _amount = amount;
        pinMode(_dat, OUTPUT);
        pinMode(_clk, OUTPUT);
        pinMode(_cs, OUTPUT);
        begin();
    }

    // инициализировать
    void begin() {
        _cmd(_GSEG_7219_TEST_MODE, 0x00);
        _cmd(_GSEG_7219_DECODE, 0x00);
        _cmd(_GSEG_7219_INTENSITY, 0x00);
        _cmd(_GSEG_7219_SCAN_LIM, 0x0f);
        _cmd(_GSEG_7219_SHUTDOWDN, 0x01);
    }

    // установить яркость (0.. 15)
    void brightness(uint8_t value) {
        _cmd(_GSEG_7219_INTENSITY, value);
    }

    // управление питанием (true вкл, false выкл)
    void power(bool state) {
        _cmd(_GSEG_7219_SHUTDOWDN, state);
    }

   protected:
    void write(uint8_t* data, bool reverse = 0) {
        for (int i = 0; i < 8; i++) {
            gio::low(_cs);
            for (int j = 0; j < _amount; j++) {
                uint8_t b = data[((_amount - 1 - j) << 3) + i];
                b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;  // reverse
                b = (b & 0xCC) >> 2 | (b & 0x33) << 2;  // reverse
                b = (b & 0xAA) >> 1 | (b & 0x55) << 1;  // reverse
                b = (b >> 1) | (b << 7);                // swap 7/1
                _write(reverse ? (8 - i) : (i + 1), b);
            }
            gio::high(_cs);
        }
    }

   private:
    uint8_t _dat, _clk, _cs;
    uint8_t _amount;

    void _write(uint8_t addr, uint8_t value) {
        uint8_t packet[2] = {addr, value};
        gio::shift::send(_dat, _clk, MSBFIRST, packet, 2, DISP7219_CLK_DELAY);
    }
    void _cmd(uint8_t addr, uint8_t value) {
        gio::low(_cs);
        for (uint16_t i = 0; i < _amount; i++) _write(addr, value);
        gio::high(_cs);
    }
};