#pragma once
#include <Arduino.h>

namespace sseg {

// получить код для символа
uint8_t getCharCode(char symb);

// получить длину int числа
uint8_t intLen(int32_t val);

// получить длину uint числа
uint8_t intLen(uint32_t val);

// получить длину float числа при указанной точности
uint8_t floatLen(double val, uint8_t dec = 2);

}