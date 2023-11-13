#pragma once
#include <Arduino.h>

#include "SegBuffer.h"

#define GS_RUNNER_IDLE 0
#define GS_RUNNER_STEP 1
#define GS_RUNNER_END 2

// модуль бегущей строки

class SegRunner {
   public:
    SegRunner(SegBuffer* buf) : _buf(buf) {
        _x0 = 0;
        _x1 = _buf->getSize() - 1;
    }

    // установить текст const char*
    void setText(const char* str) {
        _str = str;
        _pgm = 0;
        _len = strlen(str);
    }

    // установить текст String
    void setText(String& str) {
        setText(str.c_str());
    }

    // установить текст из PROGMEM (глобальный)
    void setText_P(PGM_P str) {
        _str = str;
        _pgm = 1;
        _len = strlen_P(str);
    }

    // установить окно вывода (x0, x1)
    void setWindow(uint8_t x0, uint8_t x1) {
        _x0 = min(x0, _buf->getSize());
        _x1 = min(x1, _buf->getSize());
        if (_x0 > _x1) {
            uint8_t b = _x0;
            _x0 = _x1;
            _x1 = b;
        }
        if (running()) start();
    }

    // установить скорость (символов в секунду)
    void setSpeed(uint16_t symPerSec) {
        if (!symPerSec) symPerSec = 1;
        _prd = 1000 / symPerSec;
    }

    // установить скорость (период в мс)
    void setPeriod(uint16_t ms) {
        _prd = ms;
    }

    // запустить бегущую строку с начала
    void start() {
        _pos = -(_x1 - _x0);
        resume();
    }

    // остановить бегущую строку
    void stop() {
        _tmr = 0;
    }

    // продолжить движение с момента остановки
    void resume() {
        _tmr = (uint16_t)millis();
        if (!_tmr) _tmr = 1;
    }

    // true - строка движется
    bool running() {
        return _tmr;
    }

    // тикер. Вернёт 0 в холостом, 1 при новом шаге, 2 при завершении движения
    // Можно передать false, чтобы дисплей не обновлялся сам
    uint8_t tick(bool update = true) {
        if (!_str) return 0;
        if (_tmr && (uint16_t)((uint16_t)millis() - _tmr) >= _prd) {
            resume();
            return tickManual(update);
        }
        return 0;
    }

    // сдвинуть строку на 1 символ. Можно передать false, чтобы дисплей не обновлялся сам
    uint8_t tickManual(bool update = true) {
        if (!_str) return 0;
        memset(_buf->buffer + _x0, 0, _x1 - _x0 + 1);

        int16_t cpos = 0;
        for (uint8_t i = 0, j = 0; i <= _x1 - _x0;) {
            cpos = _pos + j;
            if (cpos >= 0 && cpos < _len) {
                char ch = _pgm ? pgm_read_byte(_str + cpos) : _str[cpos];
                if (ch == '.') {
                    if (i > 0) {
                        i--;
                        _buf->point(i + _x0);
                    } else {
                        _pos++;
                        continue;
                    }
                } else {
                    _buf->setChar(i + _x0, ch);
                }
            }
            i++;
            j++;
        }

        cpos++;
        if (cpos >= 0 && cpos < _len) {
            char ch = _pgm ? pgm_read_byte(_str + cpos) : _str[cpos];
            if (ch == '.') _buf->point(_x1);
        }

        if (update) _buf->update();
        if (++_pos > _len) {
            start();
            return 2;
        }
        return 1;
    }

   private:
    SegBuffer* _buf;
    const char* _str = nullptr;
    int16_t _len;
    uint8_t _x0, _x1;
    int16_t _pos;
    bool _pgm;
    uint16_t _tmr = 0, _prd = 400;
};