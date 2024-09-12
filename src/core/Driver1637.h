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
    Driver1637(uint8_t DIO, uint8_t CLK) : DIO(DIO), CLK(CLK) {
        pinMode(CLK, INPUT);
        pinMode(DIO, INPUT);
        digitalWrite(CLK, LOW);
        digitalWrite(DIO, LOW);
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
        for (uint8_t i = 0; i < size; i++) {
            _write(buf[i]);
        }
        _stop();
        _start();
        _write(cfg);
        _stop();
    }

    virtual void update() {}

   private:
    uint8_t DIO, CLK;
    uint8_t cfg = 0x8F;  // 1000<power:1><bright:3>

    void _start() {
        pinMode(CLK, INPUT);
        pinMode(DIO, INPUT);
        delayMicroseconds(DISP1637_CLK_DELAY);
        pinMode(DIO, OUTPUT);
    }

    void _stop() {
        pinMode(CLK, OUTPUT);
        delayMicroseconds(DISP1637_CLK_DELAY);
        pinMode(DIO, OUTPUT);
        delayMicroseconds(DISP1637_CLK_DELAY);
        pinMode(CLK, INPUT);
        delayMicroseconds(DISP1637_CLK_DELAY);
        pinMode(DIO, INPUT);
    }

    void _write(uint8_t data) {
        // send
        for (uint8_t i = 0; i < 8; i++) {
            pinMode(CLK, OUTPUT);
            if (data & 1) pinMode(DIO, INPUT);
            else pinMode(DIO, OUTPUT);
            delayMicroseconds(DISP1637_CLK_DELAY);
            data >>= 1;
            pinMode(CLK, INPUT);
            delayMicroseconds(DISP1637_CLK_DELAY);
        }

        // ack
        pinMode(CLK, OUTPUT);
        pinMode(DIO, INPUT);
        for (uint8_t i = 50; i; i--) {
            delayMicroseconds(1);
            if (!digitalRead(DIO)) break;
        }
        pinMode(CLK, INPUT);
        delayMicroseconds(DISP1637_CLK_DELAY);
        pinMode(CLK, OUTPUT);
    }
};