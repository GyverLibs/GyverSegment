#pragma once
#include <Arduino.h>

#include "SegAnimationExt.h"

template <uint8_t size>
class SegAnimation : public SegAnimationExt {
   public:
    SegAnimation(SegBuffer* disp, uint8_t from, bool dec = 1) : SegAnimationExt(_buf, size, disp, from, dec) {}

   private:
    uint8_t _buf[size] = {0};
};