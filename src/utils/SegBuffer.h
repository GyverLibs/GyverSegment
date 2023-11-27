#pragma once
#include <Arduino.h>
#include <Print.h>

#include "SegUtils.h"

class SegBuffer : public Print {
   protected:
    SegBuffer(uint8_t* buffer, uint8_t size, bool decimal) : buffer(buffer), _size(size), _dec(decimal) {}

   public:
    // установить курсор для печати, 0 (начало) слева
    void setCursor(int16_t pos) {
        _pos = pos;
    }

    // установить курсор в начало
    void home() {
        setCursor(0);
    }

    // установить курсор от конца дисплея (справа налево)
    void setCursorEnd(int16_t pos = 0) {
        _pos = _size - 1 - pos;
    }

    // получить позицию курсора
    int16_t getCursor() {
        return _pos;
    }

    // проверка уместится ли int число при текущем курсоре
    bool checkInt(int32_t val) {
        return (_pos >= 0) && (_pos + sseg::intLen(val) <= _size);
    }

    // проверка уместится ли float число при текущем курсоре
    bool checkFloat(double val, uint8_t dec = 2) {
        return (_pos >= 0) && (_pos + sseg::floatLen(val, dec) <= _size);
    }

    // установить символ в позицию
    void setChar(uint8_t pos, char symb) {
        if (pos < _size) buffer[pos] = sseg::getCharCode(symb);
    }

    // установить код в позицию
    void set(uint8_t pos, uint8_t data) {
        if (pos < _size) buffer[pos] = data;
    }

    // заполнить символом
    void fillChar(char symb) {
        memset((void*)buffer, sseg::getCharCode(symb), _size);
    }

    // заполнить кодом
    void fill(uint8_t data) {
        memset((void*)buffer, data, _size);
    }

    // очистить
    void clear() {
        fill(0);
    }

    // направление печати: false - печать слева, true - задвигание справа
    void printRight(bool right) {
        _rtol = right;
    }

    // с указанием длины смещения буфера
    void printRight(bool right, uint8_t shiftSize) {
        printRight(right);
        _shiftSize = shiftSize;
    }

    // установить или выключить десятичную точку на позиции
    void point(uint8_t pos, bool state = 1) {
        if (pos < _size && _dec) bitWrite(buffer[pos], 7, state);
    }

    // отобразить часы (час без нуля, минута с нулём)
    void showClock(uint8_t hour, uint8_t minute, uint8_t from = 0) {
        setCursor(from);
        if (hour < 10) print(' ');
        print(hour);
        setCursor(from + 2);
        if (minute < 10) print(0);
        print(minute);
    }

    // вывести символ в текущую позицию курсора
    size_t write(uint8_t data) {
        if (data == '.') {
            if (!_dec) return 1;
            if (_rtol) {
                if (_pos >= 0 && _pos < _size) bitSet(buffer[_pos], 7);
            } else {
                if (_pos > 0 && _pos <= _size) bitSet(buffer[_pos - 1], 7);
            }
            return 1;
        }

        if (_pos >= 0 && _pos < _size) {
            if (_rtol) {
                uint8_t from = (_shiftSize && _shiftSize <= _pos) ? (_pos - _shiftSize + 1) : 0;
                for (int8_t i = from; i < _pos; i++) buffer[i] = buffer[i + 1];
            }
            buffer[_pos] = sseg::getCharCode(data);
        }
        if (!_rtol) _pos++;
        return 1;
    }

    // получить размер дисплея
    uint8_t getSize() {
        return _size;
    }

    // аналог delay, но внутри него вызывается тикер динамической индикации
    void delay(uint32_t prd) {
        uint32_t ms = millis();
        while (millis() - ms < prd) {
            tick();
            yield();  // esp
        }
    }

    virtual void update() {}

    // пустой тикер, для совместимости с другими дисплеями
    virtual uint8_t tick() {
        return 0;
    }

    uint8_t* buffer;

   private:
    uint8_t _size;
    bool _dec;
    int16_t _pos = 0;
    bool _rtol = 0;
    uint8_t _shiftSize = 0;
};