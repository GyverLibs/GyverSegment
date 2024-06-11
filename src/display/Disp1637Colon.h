#pragma once
#include <Arduino.h>

#include "Driver1637.h"
#include "utils/SegBuffer.h"

// дисплей с 4 цифрами и двоеточием
class Disp1637Colon : public Driver1637, public SegBuffer {
   public:
    // пины DIO, CLK
    Disp1637Colon(uint8_t DIO, uint8_t CLK, bool USE_DOTS = false) : Driver1637(DIO, CLK), SegBuffer(buffer, 4, USE_DOTS) {
        update();
    }

    // вкл-выкл двоеточие
    void colon(bool show) {
        bitWrite(buffer[1], 7, show);
        update();
    }

    // обновить дисплей
    void update() {
        Driver1637::send(buffer, 4);
    }

    uint8_t buffer[4] = {0};

   private:
};
