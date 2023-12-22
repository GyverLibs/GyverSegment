This is an automatic translation, may be incorrect in some places. See sources and examples!

# Gyversegment
The most powerful library for working with displays on 7-segment indicators
- Support for all popular Chinese modules (74HC595 4/8 digits, TM1637 4/6 digits + absorption, MAX7219 CASCAD of any length)
- symbolic processor and built -in font: a convenient output on the display of any data
- A single API for all displays
- Running line, you can display several on one display
- 7 animated effects of symbol switching
- Convenient API to support all opportunities on any other display
- Setting up brightness, including for dynamic displays
- Quick Bitbang based on Gyverio - sending data on the display by 10 times faster than in other libraries

## compatibility
Compatible with all arduino platforms (used arduino functions)

### Dependencies
- [gyverio] (https://github.com/gyverlibs/gyverio)

## Content
- [documentation] (#docs)
  - [displays] (# Disp)
  - [buffer] (#buf)
  - [Running line] (# Run)
  - [Effects] (# EFF)
  - [utilities] (#util)
  - [Add display] (# Add)
- [Examples] (# ExamPles)
- [versions] (#varsions)
- [installation] (# Install)
- [bugs and feedback] (#fedback)

<a id="docs"> </a>
<a id="disp"> </a>
## Documentation
### supported displays
> If you have found a good display module that is not supported by the library, write.I will order, test, add.

|Photo |Controller |Size |AliExpress |Class |
| ------------------------------------------------ | ----------- | ------- | --------------------------------------------------------------------------------------------------------------------------------------------- | ---------------- |
|! [Disp1637colon-0.36] (/IMG/Disp1637Colon-0.36.WebP) |TM1637 |0.36 "| [link] (https://fas.st/heysd?erid=latgbbqo6), [link] (https://fas.st/kiitut?erid=latgbbqo6), [link] (https: // fas.ST/pqw6b? Erid = Latgbbqo6) | `Disp1637Colon` |
|! [Disp1637colon-0.56] (/IMG/Disp1637Colon-0.56.WebP) |TM1637 |0.56 "| [link] (https://fas.st/y9x-ei?erid=latgbbqo6), [link] (https://fas.st/i9wnt?erid=latgbbqo6) |` disp1637colon` |
|! [Disp1637_4-0.36] (/IMG/Disp1637_4-0.36.webp) |TM1637 |0.36 "| [link] (https://fas.st/6ntg-?erid=latgbbqo6), [link] (https://fas.st/zfmia?erid=latgbbqo6), [link] (https: // //fas.st/s60yu?erid=latgbbqo6) | `Disp1637_4` |
|! [Disp1637_4-0.56] (/IMG/Disp1637_4-0.56.webp) |TM1637 |0.56 "| [link] (https://fas.st/i9wnt?erid=latgbbqo6), [link] (https://fas.st/vbgw8?erid=latgbbqo6) |` Disp1637_4` |
|! [Disp1637_6-0.36] (/IMG/Disp1637_6-0.36.webp) |TM1637 |0.36 "| [link] (https://fas.st/4uhocd?erid=latgbbqo6), [link] (https://fas.st/upp5p?erid=latgbbqo6), [link] (https: // fas.ST/tt1PEX? ERID = Latgbbqo6) | `Disp1637_6` |
|! [Disp1637_6-0.56] (/IMG/Disp1637_6-0.56.webp) |TM1637 |0.56 "| [link] (https://fas.st/i0dmi?erid=latgbbqo6)|`Disp1637_6` |
|! [Disp595_4] (/IMG/Disp595_4.webp) |74hc595 |0.36 "| [link] (https://fas.st/wc-ee?erid=latgbbqo6), [link] (https://fas.st/5d6jh?erid=latgbbqo6), [link] (https:///fas.st/ut6rQccccc?
|! [Disp595_8] (/IMG/Disp595_8.Webp) |74hc595 |0.36 "| [link] (https://fas.st/szv0w?erid=latgbbqo6), [link] (https://fas.st/ym7fu?erid=latgbbqo6) |` disp595_8`* |
|! [Disp595_8V2] (/IMG/Disp595_8V2.WebP) |74hc595 |0.36 "<br/> 0.56" |[link] (https://fas.st/6x9q9r?erid=latgbbqo6) |`Disp595_8V2`* |
|! [Disp595_8S] (/IMG/Disp595static.Webp) |74hc595 |0.56 "| [link] (https://fas.st/wlojs?erid=latgbbqo6), [link] (https://fas.st/9ea7pc?erid=latgbbqo6), [link] (https: // fas.ST/u6eenQ
|! [Disp7219] (/IMG/DISP7219.webp) |Max7219 |0.36 "| [link] (https://fas.st/_ugxv1?erid=latgbbqo6), [link] (https://fas.st/iqqly3?erid=latgbbqo6) |` Disp7219` |
|! [Dispbare] (/img/dispbare.webp) |- |- ||`Dispbare`* |

> `*` - displays with dynamic indication

### How the library works
- All displays work in a software buffer mode - after making changes to the buffer, you need to call `update ()`.The running line and switching effects themselves cause `update ()` when they need it
- All displays inherit the `segbuffer` class, which is responsible for the output of data, so the conclusion is the same for all displays
- Tools `segrunner` (running line) and` seganimation` (switching effects) also work the same for all displays
- some displays have additional methods

### General
`` `CPP
VOID Power (Bool State);// Power supply (True VKL, FALSE Off)
VOID update ();// update the display
uint8_t* buffer;// access to the buffer
`` `

### tm1637
`` `CPP
Disp1637COLON (Uint8_t Dio, Uint8_T CLK);// module 4 digits + colon
Disp1637_4 (Uint8_t Dio, Uint8_T CLK);// module 4 digits + points
Disp1637_6 (uint8_t dio, uint8_t clk);// module 6 digits + points

// everyone has
VOID Brightness (Uint8_T Bright);// brightness, 0 .. 7

// Disp1637Colon
Void Colon (Bool Show);// VCL-Opl colon

// defines of settings (announce before connecting the library)
#define Disp1637_Clk_DLAY 100 // Interface delay in ISS
`` `

## 74hc595
`` `CPP
Disp595_4 (Uint8_t Dio, Uint8_t SCLK, UINT8_T RCLK);// module 4 digits
Disp595_8 (Uint8_t Dio, Uint8_t Sclk, Uint8_T RClk);// module 8 digits
Disp595_8V2 (Uint8_t Dio, Uint8_T SCLK, Uint8_T RCLK);// module 8 digits, other wiring
DISP595STATIC <INT AMOUNT> (UINT8_T SDI, UINT8_T SCLK, UINT8_T LOAD);// Any number of numbers.Static indication!

// everyone has
uint8_t tick ();// Dynamic indication ticker, call in loop
uint8_t Tickmanual ();// ticker of dynamic indication, call in its timer
VOID Brightness (Uint8_T Bright);// Set brightness (0 .. 15)

// defines of settings (announce before connecting the library)
#define Disp595_Clk_Delay 0 // Interface delay in ISS
#define gs_exp_time 100 // Family exhibiting time
`` `

> `Disp595static` works without call` tick`.But if you need to control brightness, you need to call `tick`!

### max7219
`` `CPP
// module 8 digits
Disp7219 <int amount> (uint8_t din, uint8_t clk, uint8_t cs, bool reverse = true);

VOID Begin ();// initialized (it is necessary after the diet of the module)
Void Brightness (Uint8_T Value);// Set brightness (0 .. 15)

// defines of settings (announce before connecting the library)
#deFine Disp7219_Clk_Delay 0 // Interface Delay in the ISS
`` `
- `amount` - quantity * microcircuits * max7219, each chip supports 8 indicators, chip can be connected by a cascade to increase the length of the running line
- `Reverse` unfolds the output data from right to left for Chinese modules (silence).If you assemble your display on the datasheet, when the announcement, you need to put `false`

### Naked display
`` `CPP
Dispbare <uint8_t digits, bool decimal = 1, bool anode = 0> (uint8_t* dig, uint8_t* seg);

uint8_t tick ();// Dynamic indication ticker, call in loop
uint8_t Tickmanual ();// ticker of dynamic indication, call in its timer
VOID Brightness (Uint8_T Bright);// Set brightness (0 .. 15)

// defines of settings (announce before connecting the library)
#define gs_exp_time 100 // Family exhibiting time
`` `
- `digits` - the number of indicators
- `decimal` - indicator with decimal dp dp, which are connected to Pina
- `anode` -` true` - common anode, `false` - general cathode
- `dig` - an array of Pino indicators (coincides in the amount of` digits`)
- `seg` - an array of pins of segments (7 pins without a point` decimal`, 8 with a dot)

### Interface speed
In almost all supported displays, you can configure the speed of the interface through the define `Dispxxx_Clk_Delay` - this is a delay in microseconds when transmitting data, standard values are indicated above.If the display does not work - try to increase the delay by 50-100 μs!For example, some Chinese TM1637 modules work steadily with a zero delay, and some need 100 μs (it costs by default).Also, when the display is connected with long wires and/or if there are interference sources, you will have to increase the delay so that the data transfer is more reliable.

`` `CPP
#define Disp7219_Clk_Delay 70
#include <gyversegment.h>
`` `

### Dynamic displays
#### Tick ()
In dynamic displays (74HC595, DispBare), the ticker `tick ()` `, becauseHe displays all the numbers in turn *according to his timer *.If the program has delays that interfere with the ticker to call, artifacts may appear on the display (uneven brightness of numbers, the disappearance of numbers and other “glitches”).If the program has deaf cycles, the ticker must also be called inside them:
`` `CPP
VOID loop () {
  Disp.Tick ();

  While (1) {
    Disp.Tick ();
    // .... Break;
  }
}
`` `

In a simple code "on the deals" you can use not built -in `demelay ()`, but a delay in the display.The library will cause a ticker inside it:
`` `CPP
Disp.print ("Hello");
Disp.update ();
Disp.Delay (1000);// The ticker itself is called here
Disp.print ("1234");
Disp.update ();
`` `

How often the library updates the display:
- with maximum brightness - change of digit every 1 millisecond
- at another brightness - the change of numbers is every `gs_exp_time` (silence 100) microsecond + waiting time corresponding to brightness

### Tickmanual ()
The display can be drawn independently by interrupting the timer or in other ways, for this there is a ticker `Tickmanual ()` - without a built -in timer.How often it needs to be called:
- with the maximum brightness (`15`), it is enough to call the ticker every 2 milliseconds.This is enough so that flickering is not visible (checked on an 8-bit display, on 4-bit you can increase the time).
- If you need to control the brightness - the ticker will have to be called more often - every 100 microseconds.At longer periods, flickering on minimal brightness will appear.

<a id="buf"> </a>

### Segbuffer - Conclusion on the display
The display is concluded in the same way as in 99% of other displays and libraries: you need to install the cursor in `setcursor (x)` and display the necessary data in `Print (data)`:
- You can install the cursor, among other things, "for the display" to create your animations and other scenarios
- `Print ()` ` - the standard interface [libraries arduino] (https://www.arduino.cc/reference/en/language/function/communication/serial/Print/), withdraws any data (whole numbers, with a floating point, lines of any type)
- the cursor automatically shifts for lengthsat the text
- To update the display, call `update ()`
- A rather readable font with all Latin letters and symbols (32 .. 126 ASCII) is built into the library.Little letters are equivalent to large
- The points in the text are processed separately and are displayed into the decimal display separator if it is
- There is a `Printright (True)` regime, in which the data will be printed from right to left from the current position of the cursor.The cursor in this case does not shift during print, instead the entire display shifts to the left

! [font] (img/font.png)

`` `CPP
  Dispxxx Disp;// Any display from the library
  // ...
  Disp.Setcursor (0);
  Disp.print ("Hello");
  Disp.update ();

  // alignment on the right edge
  // The number of unknown length
  int Val = 123;
  disp.setcursorend (Sseg :: intlen (val) - 1);
  Disp.print (val);

  // or so
  Disp.Printright (True);// Print on the right
  Disp.Setcursorend ();// cursor to the end
  // Disp.Fillchar ('0');// with zero filling
  Disp.print (val);
  DISP.PRINTRIGHT (FALSE);// Disable the seal to the right
`` `

<details>
<summary> description of the class </summary>

`` `CPP
// Designer.decimal - whether the display supports decimal points
Segbuffer (Uint8_t* Buffer, Uint8_t Size, Bool Decimal);

// install the cursor for printing, 0 (beginning) on the left
VOID setcursor (int16_t pos);

// install the cursor at the beginning
VOID Home ();

// install the cursor from the end of the display (right to left)
VOID setcursorend (int16_t pos = 0);

// Get the position of the cursor
int16_t getcursor ();

// Checking will I integrate the number in the current cursor
Bool Checkint (Int32_t Val);

// Checking will Float fit the number in the current cursor
Bool Checkfloat (Double Val, Uint8_t Dec = 2);

// Install the symbol in position
VOID setchar (uint8_t pos, char Symb);

// set the code in position
VOID set (uint8_t pos, uint8_t data);

// Fill in a symbol
VOID Fillchar (Char Symb);

// Fill in the code
Void Fill (Uint8_t Data);

// Clean
Void Clear ();

// Press Direction: FALSE - Print on the left, True - sliding on the right
VOID Printright (Bool Right);

// indicating the length of the boofer displacement
VOID Printright (Bool Right, Uint8_T Shiftsize);

// install or turn off the decimal point in position
VOID POINT (UINT8_T POS, BOOL STATE = 1);

// display hours (hour without zero, minute from zero) starting with the specified position of the cursor
VOID ShowClock (Uint8_t Hour, Uint8_t Minute, Uint8_t From = 0);

// Bring the symbol to the current position of the cursor
Size_t Write (Uint8_t Data);

// get the size of the display
uint8_t getsize ();

// analogue DELAY, but inside it is caused by a ticker of dynamic indication
VOID DELAY (UINT32_T PRD);
`` `
</details>

<a id="run"> </a>

## segrunner - running line
Asynchronous output on the running line display in the set borders of the cursor
- Since the output is asynchronous, the line should be either declared globally, or in the field of determining the ticker during the running line of the running line.
- During movement, the line can be changed, it will work correctly.But if the line has a length - you need to call `settext ()` for its recalculation.The movement will not start again!
- After exiting the left edge of the display, the line automatically starts first.You can track this by the result `tick ()` (see below)

`` `CPP
Dispxxx Disp;// Any display from the library
Segrunner Run (& Disp);// transmit the address of the display

conster cstr_p [] progmem = "Progmem Cstring";
COST Char* CSTR [] = "CSTring Pointer";
Char cstr_a [] = "Global Char Array";
String str = "Global String";

VOID setup () {
  // so it is possible
  run.settext_p (cstr_p);
  run.settext (CSTR);
  run.settext (cstr_a);
  run.settext (str);
  // String Literal are initially global
  run.settext ("HELLO");

  // you can not do it this way
  String LSTR = "Local String";
  run.settext (lstr);
  Char lcstr_a [] = "Local Char Array";
  run.settext (lcstr_a);

  run.start ();// Launch
  // run.setwindow (1, 6);// boundaries in which the text moves
}

VOID loop () {
  // Movement takes place here along the timer
  run.tick ();
}
`` `

You can work locally like this:
`` `CPP
VOID FOO () {
  Segrunner rUn (& Disp);
  String str = "Local string";
  run.settext (s);
  run.start ();
  run.waitend ();
  // it works, including for dynamic displays
}
`` `

<details>
<summary> description of the class </summary>

`` `CPP
// Designer
Segrunner (Segbuffer* Buf);

// Install the text COST Char*
VOID Settext (Consta Char* str);

// Install the text String
VOID Settext (String & STR);

// Install the text from Progmem (global)
VOID settext_p (pgm_p str);

// Set the output window (x0, x1)
VOID Setwindow (Uint8_t X0, Uint8_T X1);

// set speed (characters per second)
VOID SetSpeed (Uint16_T Sympersec);

// set speed (period in MS)
VOID Setperiod (Uint16_T MS);

// Run the running line from the beginning
VOID Start ();

// Stop the running line
VOID Stop ();

// continue moving from the moment of stop
VOID Resume ();

// True - the line moves
Bool Running ();

// wait for the end of the movement of the line
VOID WAITEND ();

// ticker.Will return 0 in idle, 1 at a new step, 2 at the end of movement
// you can transfer false so that the display is not updated by yourself
uint8_t tick (bool update = true);

// Put the line 1 symbol.You can convey FALSE so that the display is not updated by yourself
uint8_t Tickmanual (bool update = true);

// Return statuses Tick:
Gs_runner_idle
Gs_runner_step
Gs_runner_end
`` `
</details>

<a Id="EFP"> </a>

### Seganimation - Switching effects
Asynchronous animations of data change on the display
- Animation creates its own buffer like `segbuffer`, into which new data needs to display *instead of the output on the display *
- the system itself will see a change in data and reproduce the established effect for changing the image to the current
- animation is reproduced only in the given boundaries of the display (beginning, length), the rest of the display does not affect

Effects:
`` `CPP
Segeffect :: Blink
Segeffect :: rollup
Segeffect :: Rolldown
Segeffect :: Twistfill
Segeffect :: TwistClear
Segeffect :: Slideup
Segeffect :: slideDown
`` `

`` `CPP
Dispxxx Disp;// Any display from the library
// indicate the number of characters, display and position of the display cursor
Seganimation <3> Anim (& Disp, 0);

VOID setup () {
  Anim.SetEffect (Segeffect :: TwistClear);
  Anim.start ();

  // Bring new data and wait for the end of the effect
  // Waiting works correctly, includingFor dynamic displays
  Anim.setcursor (0);
  Anim.print (Random (1000));
  Anim.waitend ();
  Disp.Delay (1000);

  Anim.setcursor (0);
  Anim.print (Random (1000));
  Anim.waitend ();
  Disp.Delay (1000);
}

VOID loop () {
  Anim.tick ();
  Disp.Tick ();

  Static uint32_t tmr;
  if (millis () - tmr> = 1000) {
    // If somewhere change the data of the animator -
    // He himself will update the display
    Anim.setcursor (0);
    Anim.print (Random (1000));
  }
}
`` `

<details>
<summary> description of the class </summary>

`` `CPP
// Designer
Seganimation (Segbuffer* Disp, Uint8_t From, Bool Dec = 1);

// set the effect and time of its execution in MS
VOID SetEFFECT (Segeffect EFF, UINT16_T DURATION = 300);

// Launch
VOID Start ();

// Stop animation
VOID Stop ();

// True - the effect is reproduced
Bool Running ();

// forcibly update the display from the buffer effect
VOID Refresh ();

// play the effect on all numbers (silent)
VOID Forceall (Bool Force);

// wait for the end of the effect of the effect
VOID WAITEND ();

// ticker.Will return 0 in idle, 1 at a new step, 2 at the end of the animation
uint8_t tick ();

// manual ticker.Will return 0 in idle, 1 at a new step, 2 at the end of the animation
uint8_t Tickmanual ();

// Return statuses Tick:
GS_animation_idle
Gs_animation_step
GS_animation_end
`` `
</details>

<a Id="UTIL"> </a>

### utilities

`` `CPP
// Get a code for a symbol
Uint8_t Sseg :: Getcharcode (Char Symb);

// Get the length integro
uint8_t sseg :: intlen (int32_t val);

// get the length of the FLOAT number with the indicated accuracy
Uint8_t Sseg :: Floatlen (Double Val, Uint8_t Dec = 2);
`` `

<a ID="DD"> </a>

### How to add a display
The library allows you to very easily add its capabilities (conclusion, running line, effects) to any other 7-segment displays, even homemade.For this, get itCranberries exactly create a class from `segbuffer` and implement in it:
- buffer for the right amount of familiar
- method `update ()`, which will display this buffer on the display
- method `tick ()` (for dynamic display)
- The buffer has the following bits order: `0BPGFEDCBA`

! [7seg] (img/7seg.png)

`` `CPP
// Number is familiar and the presence of points
// Names Constant are given for example
#define my_disp_size 4
#define my_disp_decimals True

Class MyDisp: Public Segbuffer {
   Public:
    MyDisp (): Segbuffer (Buffer, my_disp_size, my_disp_decimals) {}

    // update the display
    VOID update () {
        // Your code
        Return 0;
    }

    // Ticker of dynamic indication, call in LOOP constantly or by timer
    uint8_t tick () {
        // Your code
        Return 0;
    }

    uint8_t buffer [my_disp_size] = {0};

   Private:
};
`` `

Now your display supports all the possibilities of the library!

<a id="EXAMPLASX> </A>

## Examples
### Demka
`` `CPP
#include <arduino.h>
#include <gyversegment.h>

#define dio_pin 2
#define clk_pin 3
#define lat_pin 4

// announcement of display.Choose any
Disp595_4 Disp (DIO_PIN, CLK_PIN, LAT_PIN);
// Disp595_8 Disp (DIO_PIN, CLK_PIN, LAT_PIN);
// Disp595_8V2 Disp (DIO_PIN, CLK_PIN, LAT_PIN);
// Disp595static <4> Disp (Dio_pin, Clk_pin, Lat_pin);
// Disp1637_4 Disp (dio_pin, clk_pin);
// Disp1637_6 Disp (dio_pin, clk_pin);
// Disp1637colon Disp (Dio_pin, Clk_pin);
// Disp7219 <1> Disp (dio_pin, clk_pin, lat_pin);// 1 chip - 8 digits

uint8_t digs [] = {2, 3, 4, 5, a2, a3, a4, a5};// Pins of numbers
uint8_t segs [] = {6, 7, 8, 9, 10, 11, 12, 13};// Pins of the segments
// 8 digits, there are disper points, the general cathode
// Dispbare <8, true, false> disp (digs, segs);

VOID setup () {
  // Used Disp.Delay () for display with Dean.indication

  // text
  Disp.Setcursor (0);
  Disp.print ("Hello");
  Disp.update ();
  Disp.Delay (1000);

  // The whole number
  Disp.Setcursor (0);
  Disp.print (1234);
  Disp.update ();
  Disp.Delay (1000);

  // Float
  Disp.Setcursor (0);
  Disp.print (3.14, 3);// accuracy 3 signs
  Disp.update ();
  Disp.Delay (1000);

  // The output of the number on the right with the filling of zeros
  Disp.Setcursorend ();
  Disp.Printright (True);
  Disp.Fillchar ('0');
  Disp.print (3.14, 3);
  Disp.update ();
  Disp.Delay (1000);

  DISP.PRINTRIGHT (FALSE);
}
VOID loop () {
  // is needed for displays with dynamic indication, but
  // For compatibility, all displays have, even if nothing does nothing
  Disp.Tick ();
}
`` `

### meter with alignment on the right
`` `CPP
#include <gyversegment.h>

Disp1637_4 Disp (2, 3);

VOID setup () {
  Disp.Printright (True);// Print on the right
  Disp.Setcursorend ();// cursor to the end
}

int n = 0;

VOID loop () {
  Disp.clear ();
  Disp.print (n);
  Disp.update ();
  n ++;
  Disp.Delay (100);// for Dean.Display
}
`` `

<a id="versions"> </a>
## versions
- V1.0
- V1.1 - Added by Forceall () and length for Printright ()
- V1.1.1 - Bag is fixed
- v1.1.2 - Fixed bug with Printright
- V1.2
  - The brightness driver for dynamic displays is rewritten.Reduced load on the processor, increased stability
  - added a hand ticker for dynamic displays
- v1.3 - Added support for the display Disp595_8V2
- V1.4 - Added Disp595static display support

<a id="install"> </a>
## Installation
- The library can be found by the name ** gyversegment ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download library] (https://github.com/gyverlibs/gyversegment/archive/refs/heads/main.zip). Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!

<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!

When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code