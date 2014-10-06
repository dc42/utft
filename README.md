## UTFT - Library
This code is based on Henning Karlsen's work
http://henningkarlsen.com/electronics/library.php?id=51

My main purpose in forking this library is to support my Ormerod 3D printer control board, which is designed to be compatible with Arduino Leonardo for programming. The changes I have made are:

1. Supports a new 9 bit interface mode. This used an external 74HC373 chip to latch the high byte of data, so that the same 8 pins can be used for both the low and high bytes when driving a display that has a 16 bit interface. One pin is needed to drive the latch. So this saves 7 pins compared to using 16-bit mode.
2. The transfer mode is now set via another parameter separate from the display type. This avoids having to use separate names for the same 16-bit display driven with/without the external latch.
3. I have rewritten the text print functions. The UTFT class now inherits from Print, so all the usual printing functions can be used. A separate call setTextPos(x, y) sets the starting position. Proportionally-spaced fonts and auto kerning are now used. Optionally, a right margin can be defined. This is used to prevent text from spiling beyond the margin, also there is a clear-to-marging command. Both of these features are useful when writing text using propertionally-spaced fonts, as the width of the text is not easy to predict.
4. I have rewritten the straight line drawing function to use Bresenham's algorithm, to avoid using floating point maths.
5. I have rewritten the fillCircle function to be much faster.
6. I have made other speed improvements. In particular, filling or clearing large blocks of pixels is much faster, when using 9- or 16-bit interface mode.
7. I have added other options in memorysaver.h to allow further memory savings to be made, e.g. by disabling transfer modes that are not required.

Fonts must now be ordered in columns, with a column count at the start of each character. Use GLCD Font Creator to create them, then add the 4 header bytes at the start. I provide 10x10 and 16x16 fonts.

The constructor is now called like this:

UTFT myLcd(displayType, transferMode, RS, WR, CS, RST, SER_LATCH);

The SER_LATCH parameter is only needed when using 5-wire serial or 9-bit parallel mode. It specifies the Arduino pin number used for then 5th serial wire or the 74HC373 latch signal. Example:

  UTFT lcd(HX8352A, TMode9bit, 11, 10, 9, 8, 4);
  
or:

  UTFT lcd(ITDB32S, ITDB32SMode, 11, 10, 9, 8);

Limitations:

1. PIC is no longer supported, because I do not have a PIC to test on.
2. Writing text currently works in landscape mode only.
3. Character rotation is not supported.
4. I have only tested a small subset of the display types and transfer mode.

