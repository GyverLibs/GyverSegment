#include <Arduino.h>
#include <GyverSegment.h>

#define DIO_PIN 2
#define CLK_PIN 3
#define LAT_PIN 4

// объявление дисплеев. Выбери любой
Disp595_4 disp(DIO_PIN, CLK_PIN, LAT_PIN);
// Disp595_8 disp(DIO_PIN, CLK_PIN, LAT_PIN);
// Disp595_8v2 disp(DIO_PIN, CLK_PIN, LAT_PIN);
// Disp595Static<4> disp(DIO_PIN, CLK_PIN, LAT_PIN);
// Disp1637_4 disp(DIO_PIN, CLK_PIN);
// Disp1637_6 disp(DIO_PIN, CLK_PIN);
// Disp1637Colon disp(DIO_PIN, CLK_PIN);
// Disp7219<1> disp(DIO_PIN, CLK_PIN, LAT_PIN);  // 1 чип - 8 цифр

uint8_t digs[] = {2, 3, 4, 5, A2, A3, A4, A5};  // пины цифр
uint8_t segs[] = {6, 7, 8, 9, 10, 11, 12, 13};  // пины сегментов
// 8 цифр, дсятичные точки есть, общий катод
// BareDisp<8, true, false> disp(digs, segs);

void setup() {
    // используется disp.delay() для дисплеев с дин. индикацией

    // текст
    disp.setCursor(0);
    disp.print("hello");
    disp.update();
    disp.delay(1000);

    // целое число
    disp.setCursor(0);
    disp.print(1234);
    disp.update();
    disp.delay(1000);

    // float
    disp.setCursor(0);
    disp.print(3.14, 3);  // точность 3 знака
    disp.update();
    disp.delay(1000);

    // вывод числа справа с заполнением нулями
    disp.setCursorEnd();
    disp.printRight(true);
    disp.fillChar('0');
    disp.print(3.14, 3);
    disp.update();
    disp.delay(1000);

    disp.printRight(false);
}
void loop() {
    // нужен для дисплеев с динамической индикацией, но
    // для совместимости есть у всех дисплеев, даже если ничего не делает
    disp.tick();
}