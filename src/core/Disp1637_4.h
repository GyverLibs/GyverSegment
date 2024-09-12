#pragma once
#include <Arduino.h>

#include "Driver1637.h"
#include "utils/SegBuffer.h"

// дисплей с 4 цифрами и десятичными точками
class Disp1637_4 : public Driver1637, public SegBuffer {
   public:
    // пины DIO, CLK
    Disp1637_4(uint8_t DIO, uint8_t CLK) : Driver1637(DIO, CLK), SegBuffer(buffer, 4, 1) {
        update();
    }

    // обновить дисплей
    void update() {
        Driver1637::send(buffer, 4);
    }

    uint8_t buffer[4] = {0};

   private:
};