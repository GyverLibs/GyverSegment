#include <Arduino.h>
#include <GyverSegment.h>

#define DIO_PIN 2
#define CLK_PIN 3
#define LAT_PIN 4

// объявление дисплеев. Выбери любой
// Disp595_4 disp(DIO_PIN, CLK_PIN, LAT_PIN);
Disp595_8 disp(DIO_PIN, CLK_PIN, LAT_PIN);
// Disp1637_4 disp(DIO_PIN, CLK_PIN);
// Disp1637_6 disp(DIO_PIN, CLK_PIN);
// Disp1637Colon disp(DIO_PIN, CLK_PIN);
// Disp7219<1> disp(DIO_PIN, CLK_PIN, LAT_PIN);  // 1 чип - 8 цифр

uint8_t digs[] = {2, 3, 4, 5, A2, A3, A4, A5};  // пины цифр
uint8_t segs[] = {6, 7, 8, 9, 10, 11, 12, 13};  // пины сегментов
// 8 цифр, дсятичные точки есть, общий катод
// BareDisp<8, true, false> disp(digs, segs);

SegRunner run(&disp);
const char pstr[] PROGMEM = "hello progmem";

void setup() {
    SegRunner run_local(&disp);
    run_local.setText("01234567");
    run_local.start();
    run_local.waitEnd();
    delay(1000);
    
    run.setText_P(pstr);
    run.start();
}
void loop() {
    // нужен для дисплеев с динамической индикацией, но
    // для совместимости есть у всех дисплеев, даже если ничего не делает
    disp.tick();
    run.tick();
}