#pragma once
#include <Arduino.h>

#ifndef DISP1637_CLK_DELAY
#define DISP1637_CLK_DELAY 100
#endif

#define _GSEG_1637_DATA 0x40
#define _GSEG_1637_DISP 0xC0
#define _GSEG_1637_ADDR 0x80

class Driver1637 {
   public:
    Driver1637(uint8_t dio, uint8_t clk) : _dio(dio), _clk(clk) {
        pinMode(_clk, INPUT);
        pinMode(_dio, INPUT);
        digitalWrite(_clk, LOW);
        digitalWrite(_dio, LOW);
    }

    // яркость, 0.. 7
    void brightness(uint8_t bright) {
        cfg = (cfg & 0xf8) | (bright & 0x7);
        update();
    }

    // управление питанием (true вкл, false выкл)
    void power(bool state) {
        bitWrite(cfg, 3, state);
        update();
    }

   protected:
    void send(uint8_t* buf, uint8_t size) {
        _start();
        _write(_GSEG_1637_DATA);
        _stop();
        _start();
        _write(_GSEG_1637_DISP);
        while (size--) _write(*buf++);
        _stop();
        _start();
        _write(cfg);
        _stop();
    }

    virtual void update() {}

   private:
    uint8_t _dio, _clk;
    uint8_t cfg = 0x8F;  // 1000<power:1><bright:3>

    void _start() {
        pinMode(_clk, INPUT);
        pinMode(_dio, INPUT);
        delayMicroseconds(DISP1637_CLK_DELAY);
        pinMode(_dio, OUTPUT);
    }

    void _stop() {
        pinMode(_clk, OUTPUT);
        delayMicroseconds(DISP1637_CLK_DELAY);
        pinMode(_dio, OUTPUT);
        delayMicroseconds(DISP1637_CLK_DELAY);
        pinMode(_clk, INPUT);
        delayMicroseconds(DISP1637_CLK_DELAY);
        pinMode(_dio, INPUT);
    }

    void _write(uint8_t data) {
        // send
        for (uint8_t i = 0; i < 8; i++) {
            pinMode(_clk, OUTPUT);
            pinMode(_dio, (data & 1) ? INPUT : OUTPUT);
            delayMicroseconds(DISP1637_CLK_DELAY);
            data >>= 1;
            pinMode(_clk, INPUT);
            delayMicroseconds(DISP1637_CLK_DELAY);
        }

        // ack
        pinMode(_clk, OUTPUT);
        pinMode(_dio, INPUT);
        uint8_t i = 50;
        while (i--) {
            delayMicroseconds(1);
            if (!digitalRead(_dio)) break;
        }
        pinMode(_clk, INPUT);
        delayMicroseconds(DISP1637_CLK_DELAY);
        pinMode(_clk, OUTPUT);
    }
};