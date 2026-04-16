#pragma once
#include <Arduino.h>

#define _GSEG_1637_DATA 0x40
#define _GSEG_1637_DISP 0x80
#define _GSEG_1637_ADDR 0xC0

// mode
#ifdef DISP1637_OPEN_DRAIN

#ifndef DISP1637_CLK_DELAY
#define DISP1637_CLK_DELAY 128
#endif

#define DP1637_INIT(pin) (pinMode(pin, INPUT), digitalWrite(pin, LOW))
#define DP1637_LOW(pin) pinMode(pin, OUTPUT)
#define DP1637_HIGH(pin) pinMode(pin, INPUT)
#else  // push-pull

#ifndef DISP1637_CLK_DELAY
#define DISP1637_CLK_DELAY 32
#endif

#define DP1637_INIT(pin) (digitalWrite(pin, HIGH), pinMode(pin, OUTPUT))
#define DP1637_LOW(pin) digitalWrite(pin, LOW)
#define DP1637_HIGH(pin) digitalWrite(pin, HIGH)
#endif
// mode

class Driver1637 {
   public:
    Driver1637(uint8_t dio, uint8_t clk) : _dio(dio), _clk(clk) {
        DP1637_INIT(_clk);
        DP1637_INIT(_dio);
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
        _write(_GSEG_1637_ADDR);
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
        DP1637_LOW(_dio);
        delayMicroseconds(DISP1637_CLK_DELAY);
    }

    void _stop() {
        DP1637_LOW(_clk);
        DP1637_LOW(_dio);
        delayMicroseconds(DISP1637_CLK_DELAY);

        DP1637_HIGH(_clk);
        delayMicroseconds(DISP1637_CLK_DELAY);
        DP1637_HIGH(_dio);
        delayMicroseconds(DISP1637_CLK_DELAY);
    }

    void _write(uint8_t data) {
        // send
        for (uint8_t i = 0; i < 8; i++) {
            DP1637_LOW(_clk);
            if (data & 1) DP1637_HIGH(_dio);
            else DP1637_LOW(_dio);
            delayMicroseconds(DISP1637_CLK_DELAY);

            DP1637_HIGH(_clk);
            delayMicroseconds(DISP1637_CLK_DELAY);

            data >>= 1;
        }

        // ACK
        DP1637_LOW(_clk);
        pinMode(_dio, INPUT);
        delayMicroseconds(DISP1637_CLK_DELAY);

        DP1637_HIGH(_clk);
        delayMicroseconds(DISP1637_CLK_DELAY);

        DP1637_LOW(_clk);

#ifndef DISP1637_OPEN_DRAIN
        digitalWrite(_dio, HIGH);
        pinMode(_dio, OUTPUT);
#endif
    }
};