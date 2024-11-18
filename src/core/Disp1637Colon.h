#pragma once
#include <Arduino.h>

#include "Driver1637.h"
#include "utils/SegBuffer.h"

// дисплей с 4 цифрами и двоеточием
class Disp1637Colon : public Driver1637, public SegBuffer {
   public:
    // пины DIO, CLK
    Disp1637Colon(uint8_t DIO, uint8_t CLK, bool dots = false) : Driver1637(DIO, CLK), SegBuffer(buffer, 4, dots) {
        update();
    }

    // вкл-выкл двоеточие
    void colon(bool show) {
        bitWrite(buffer[1], 7, show);
        update();
    }

    //отображать перевернуто
    void setFlip(bool f){
        flip = f;
        update();
    }

    // обновить дисплей
    void update() {
        if(flip){
            uint8_t rbuffer[4] = {0};
            for(byte i = 0; i<4; i++){
                bitWrite(rbuffer[i],0,bitRead(buffer[3-i],3));
                bitWrite(rbuffer[i],1,bitRead(buffer[3-i],4));
                bitWrite(rbuffer[i],2,bitRead(buffer[3-i],5));
                bitWrite(rbuffer[i],3,bitRead(buffer[3-i],0));
                bitWrite(rbuffer[i],4,bitRead(buffer[3-i],1));
                bitWrite(rbuffer[i],5,bitRead(buffer[3-i],2));
                bitWrite(rbuffer[i],6,bitRead(buffer[3-i],6));
            }
            bitWrite(rbuffer[1],7,bitRead(buffer[1],7)); //двоеточие
            Driver1637::send(rbuffer, 4);
        }else{
            Driver1637::send(buffer, 4);
        }
    }

    uint8_t buffer[4] = {0};
    bool flip = 0;

   private:
};
