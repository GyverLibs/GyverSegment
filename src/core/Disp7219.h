#pragma once
#include <Arduino.h>
#include <GyverIO.h>

#include "Driver7219.h"
#include "utils/SegBuffer.h"

template <uint8_t amount>
class Disp7219 : public Driver7219, public SegBuffer {
   public:
    // <количество микросхем> (DIN, CLK, CS, порядок цифр)
    Disp7219(uint8_t DIN, uint8_t CLK, uint8_t CS, bool reverse = true) : Driver7219(DIN, CLK, CS, amount), SegBuffer(buffer, amount * 8, 1) {
        _rev = reverse;
        update();
    }

    // обновить дисплей
    void update() {
        Driver7219::write(buffer, _rev);
    }

    uint8_t buffer[amount * 8] = {0};

   private:
    bool _rev;
};