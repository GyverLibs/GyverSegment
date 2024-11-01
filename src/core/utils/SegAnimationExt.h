#pragma once
#include <Arduino.h>

#include "SegBuffer.h"

// модуль анимаций

#define GS_ANIMATION_IDLE 0
#define GS_ANIMATION_STEP 1
#define GS_ANIMATION_END 2

extern const uint8_t _segef_prd[] PROGMEM;
extern const uint8_t _segef_twist[] PROGMEM;
extern const uint8_t _segef_slide[] PROGMEM;

enum class SegEffect : uint8_t {
    Blink,
    RollUp,
    RollDown,
    TwistFill,
    TwistClear,
    SlideUp,
    SlideDown,
    None,
};

class SegAnimationExt : public SegBuffer {
   public:
    SegAnimationExt(uint8_t* buffer,
                    uint8_t size,
                    SegBuffer* disp,
                    uint8_t from,
                    bool dec = 1) : SegBuffer(buffer, size, dec),
                                    buffer(buffer),
                                    _size(size),
                                    _disp(disp),
                                    _from(min(from, size)) {}

    // установить эффект и время его выполнения в мс
    void setEffect(SegEffect eff, uint16_t duration = 300) {
        _eff = eff;
        _prd = duration / pgm_read_byte(_segef_prd + (uint8_t)_eff);
        _count = 0;
    }

    // запустить
    void start() {
        _tmr = (uint16_t)millis();
        _state = 1;
    }

    // остановить анимацию
    void stop() {
        _state = 0;
        refresh();
    }

    // принудительно обновить дисплей из буфера эффекта
    void refresh() {
        memcpy(_disp->buffer + _from, buffer, _size);
        _disp->update();
    }

    // воспроизводить эффект на всех цифрах (умолч. откл)
    void forceAll(bool force) {
        _maskall = force;
    }

    // true - эффект воспроизводится
    bool running() {
        return _count;
    }

    // ждать окончания воспроизведения эффекта
    void waitEnd() {
        if (!_state || (!running() && !_differ())) return;
        while (tick() != GS_ANIMATION_END) {
            _disp->tick();
            yield();  // esp
        }
    }

    // тикер. Вернёт 0 в холостом, 1 при новом шаге, 2 при завершении анимации
    uint8_t tick() {
        if (_state && (uint16_t)((uint16_t)millis() - _tmr) >= _prd) {
            start();
            return tickManual();
        }
        return 0;
    }

    // ручной тикер. Вернёт 0 в холостом, 1 при новом шаге, 2 при завершении анимации
    uint8_t tickManual() {
        if (_eff == SegEffect::None) return 0;

        if (_count) {
            if (_maskall) {
                for (uint8_t i = 0; i < _size; i++) _apply(i);
            } else {
                for (uint8_t i = 0; i < min(_size, (uint8_t)32); i++) {
                    if ((_mask & (1ul << i))) _apply(i);
                }
            }
            _count--;
            if (!_count) refresh();  // update inside
            else _disp->update();    // skip
            return _count ? 1 : 2;

        } else {
            if (_differ()) {
                _count = pgm_read_byte(_segef_prd + (uint8_t)_eff);
                if (!_maskall) {
                    _mask = 0;
                    for (uint8_t i = 0; i < min(_size, (uint8_t)32); i++) {
                        if ((_disp->buffer[_from + i] & 0x7F) != (buffer[i] & 0x7F)) _mask |= (1ul << i);
                    }
                }
            }
        }

        return 0;
    }

    uint8_t* buffer = nullptr;

   private:
    uint8_t _size;
    SegBuffer* _disp;
    uint8_t _from;

    uint16_t _tmr = 0, _prd = 100;
    SegEffect _eff = SegEffect::None;
    uint8_t _count = 0;
    uint32_t _mask = 0;
    bool _maskall = 0;
    bool _state = 0;

    bool _differ() {
        for (uint8_t i = 0; i < _size; i++) {
            if ((_disp->buffer[_from + i] & 0x7F) != (buffer[i] & 0x7F)) return 1;
        }
        return 0;
        // return memcmp(_disp->buffer + _from, buffer, _size);
    }

    void _apply(uint8_t i) {
        uint8_t& disp_b = _disp->buffer[_from + i];
        uint8_t& eff_b = buffer[i];
        switch (_eff) {
            case SegEffect::Blink:
                disp_b = 0;
                break;

            case SegEffect::RollUp:
            case SegEffect::RollDown: {
                uint8_t newb = 0;
                if (_count == 4) _shiftSeg(newb, disp_b, _eff == SegEffect::RollDown);
                else if (_count == 2) _shiftSeg(newb, eff_b, _eff == SegEffect::RollUp);
                disp_b = newb;
            } break;

            case SegEffect::TwistFill:
            case SegEffect::TwistClear: {
                uint8_t idx = 14 - _count;
                uint8_t b = pgm_read_byte(_segef_twist + idx);
                if (_eff == SegEffect::TwistFill) {
                    if (idx < 7) disp_b |= b;
                    else if (!(eff_b & b)) disp_b &= ~b;
                } else {
                    if (idx < 7) disp_b &= ~b;
                    else disp_b |= (eff_b & b);
                }
            } break;

            case SegEffect::SlideUp:
            case SegEffect::SlideDown: {
                uint8_t idx = (_eff == SegEffect::SlideDown) ? (10 - _count) : (_count - 1);
                uint8_t b = pgm_read_byte(_segef_slide + ((idx < 5) ? idx : (idx - 5)));
                if ((_eff == SegEffect::SlideDown) ? (idx < 5) : (idx >= 5)) disp_b &= ~b;
                else disp_b |= (eff_b & b);
            } break;

            default:
                break;
        }
    }

    void _bitcpy(uint8_t& dst, uint8_t dpos, uint8_t& src, uint8_t spos) {
        if (bitRead(src, spos)) bitSet(dst, dpos);
    }
    void _shiftSeg(uint8_t& dst, uint8_t& src, bool dir) {
        if (dir) {
            _bitcpy(dst, 6, src, 0);
            _bitcpy(dst, 2, src, 1);
            _bitcpy(dst, 4, src, 5);
            _bitcpy(dst, 3, src, 6);
        } else {
            _bitcpy(dst, 0, src, 6);
            _bitcpy(dst, 1, src, 2);
            _bitcpy(dst, 5, src, 4);
            _bitcpy(dst, 6, src, 3);
        }
    }
};