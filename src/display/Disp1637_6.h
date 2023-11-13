#pragma once
#include <Arduino.h>

#include "Driver1637.h"
#include "utils/SegBuffer.h"

class Disp1637_6 : public Driver1637, public SegBuffer {
   public:
    // пины DIO, CLK
    Disp1637_6(uint8_t DIO, uint8_t CLK) : Driver1637(DIO, CLK), SegBuffer(buffer, 6, 1) {
        update();
    }

    // обновить дисплей
    void update() {
        uint8_t _buffer[6];
        _buffer[0] = buffer[2];
        _buffer[1] = buffer[1];
        _buffer[2] = buffer[0];
        _buffer[3] = buffer[5];
        _buffer[4] = buffer[4];
        _buffer[5] = buffer[3];
        Driver1637::send(_buffer, 6);
    }

    uint8_t buffer[6] = {0};

   private:
};