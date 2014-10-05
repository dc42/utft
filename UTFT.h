/*
  UTFT.h - Arduino/chipKit library support for Color TFT LCD Boards
  Copyright (C)2010-2012 Henning Karlsen. All right reserved
  
  This library is the continuation of my ITDB02_Graph, ITDB02_Graph16
  and RGB_GLCD libraries for Arduino and chipKit. As the number of 
  supported display modules and controllers started to increase I felt 
  it was time to make a single, universal library as it will be much 
  easier to maintain in the future.

  Basic functionality of this library was origianlly based on the 
  demo-code provided by ITead studio (for the ITDB02 modules) and 
  NKC Electronics (for the RGB GLCD module/shield).

  This library supports a number of 8bit, 16bit and serial graphic 
  displays, and will work with both Arduino and chipKit boards. For a 
  full list of tested display modules and controllers, see the 
  document UTFT_Supported_display_modules_&_controllers.pdf.

  When using 8bit and 16bit display modules there are some 
  requirements you must adhere to. These requirements can be found 
  in the document UTFT_Requirements.pdf.
  There are no special requirements when using serial displays.

  You can always find the latest version of the library at 
  http://electronics.henningkarlsen.com/

  If you make any modifications or improvements to the code, I would 
  appreciate that you share the code with me so that I might include 
  it in the next release. I can be contacted through 
  http://electronics.henningkarlsen.com/contact.php.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef UTFT_h
#define UTFT_h

#define LEFT 0
#define RIGHT 9999
#define CENTER 9998

#define PORTRAIT 0
#define LANDSCAPE 1

enum DisplayType {
	HX8347A,
	ILI9327,
	SSD1289,
	ILI9325C,
	ILI9325D,
	HX8340B,
	HX8340B_S,
	HX8352A,
	ST7735,
	PCF8833,
	S1D19122,
	SSD1963_480,
	SSD1963_800,
	S6D1121,
	
	// Aliases for particular display models
	ITDB32 = HX8347A,								// HX8347-A (16bit)
	ITDB32WC = ILI9327,								// ILI9327  (16bit)
	ITDB32S	= SSD1289, TFT01_32 = SSD1289,			// SSD1289  (16bit)
	ITDB24 = ILI9325C,								// ILI9325C (8bit)
	ITDB24D = ILI9325D, ITDB24DWOT = ILI9325D,		// ILI9325C (8bit)
	ITDB28 = ILI9325D, TFT01_24_8 = ILI9325D,		// ILI9325D (8bit)
	TFT01_24_16 = ILI9325D,							// ILI9325D (16bit)
	ITDB22 = HX8340B,								// HX8340-B (8bit)
	ITDB22SP = HX8340B_S,							// HX8340-B (Serial)
	ITDB32WD = HX8352A,								// HX8352-A (16bit)
	TFT01_32WD = HX8352A,							// HX8352A	(16bit)
	ITDB18SP = ST7735,								// ST7735   (Serial)
	LPH9135 = PCF8833,								// PCF8833	(Serial)
	ITDB25H = S1D19122,								// S1D19122	(16bit)
	ITDB43 = SSD1963_480,							// SSD1963	(16bit) 480x272
	ITDB50 = SSD1963_800,							// SSD1963	(16bit) 800x480
	ITDB24E = S6D1121,								// S6D1121
};

enum TransferMode
{
	// The serial ones must come first
	TModeSerial4pin,
	TModeSerial5pin,
	
	// All the rest are parallel
	TModeLowestParallel,
	TMode8bit = TModeLowestParallel,
	TMode9bit,
	TMode16bit,
	
	// Aliased for modes of particular display models
	ITDB32Mode = TMode16bit,							// HX8347-A (16bit)
	ITDB32WCMode = TMode16bit,							// ILI9327  (16bit)
	ITDB32SMode	= TMode16bit,							// SSD1289  (16bit)
	TFT01_32Mode = TMode16bit,							// SSD1289  (16bit)
	ITDB24Mode = TMode8bit,								// ILI9325C (8bit)
	ITDB24DMode = TMode8bit,							// ILI9325D (8bit)
	ITDB24DWOTMode = TMode8bit,							// ILI9325D (8bit)
	ITDB28Mode = TMode8bit,								// ILI9325D (8bit)
	TFT01_24_8Mode = TMode8bit,							// ILI9325D (8bit)
	TFT01_24_16Mode = TMode16bit,						// ILI9325D (16bit)
	ITDB22Mode = TMode8bit,								// HX8340-B (8bit)
	ITDB22SPMode = TModeSerial4pin,						// HX8340-B (Serial)
	ITDB32WDMode = TMode16bit,							// HX8352-A (16bit)
	TFT01_32WDMode = TMode16bit,						// HX8352A	(16bit)
	ITDB18SPMode = TModeSerial5pin,						// ST7735   (Serial)
	LPH9135Mode = TModeSerial5pin,						// PCF8833	(Serial)
	ITDB25HMode = TMode16bit,							// S1D19122	(16bit)
	ITDB43Mode = TMode16bit,							// SSD1963	(16bit) 480x272
	ITDB50Mode = TMode16bit,							// SSD1963	(16bit) 800x480
	ITDB24E_8Mode = TMode8bit,							// S6D1121	(8bit)
	ITDB24E_16Mode = TMode16bit							// S6D1121	(16bit)
};

#if defined(__AVR__)
	#if defined(ARDUINO) && ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h"
	#endif
	#include "HW_AVR_defines.h"
#else
	#include "WProgram.h"
	#include "HW_PIC32_defines.h"
#endif

struct _current_font
{
	uint8_t* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
};

class UTFT
{
	public:
		UTFT(DisplayType model, TransferMode pMode, int RS, int WR, int CS, int RST, int SER_LATCH = 0);
		void InitLCD(uint8_t orientation = LANDSCAPE);
		void clrScr();
		void drawPixel(int x, int y);
		void drawLine(int x1, int y1, int x2, int y2);
		void fillScr(uint8_t r, uint8_t g, uint8_t b);
		void drawRect(int x1, int y1, int x2, int y2);
		void drawRoundRect(int x1, int y1, int x2, int y2);
		void fillRect(int x1, int y1, int x2, int y2);
		void fillRoundRect(int x1, int y1, int x2, int y2);
		void drawCircle(int x, int y, int radius);
		void fillCircle(int x, int y, int radius);
		void setColor(uint8_t r, uint8_t g, uint8_t b);
		void setBackColor(uint8_t r, uint8_t g, uint8_t b);
		void print(char *st, int x, int y, int deg = 0);
#ifndef DISABLE_STRINGS
		void print(String st, int x, int y, int deg = 0);
#endif
		void printNumI(long num, int x, int y, int length = 0, char filler=' ');
		void printNumF(double num, byte dec, int x, int y, char divider = '.', int length = 0, char filler = ' ');
		void setFont(uint8_t* font);
		void drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale = 1);
		void drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy);
		void lcdOff();
		void lcdOn();
		void setContrast(uint8_t c);
		int  getDisplayXSize() const;
		int	 getDisplayYSize() const;

	protected:
		uint8_t fcolorr, fcolorg, fcolorb;
		uint8_t bcolorr, bcolorg, bcolorb;
		uint8_t orient;
		long disp_x_size, disp_y_size;
		DisplayType displayModel;
		TransferMode displayTransferMode;
		regtype *P_RS, *P_WR, *P_CS, *P_RST, *P_SDA, *P_SCL;
		regsize B_RS, B_WR, B_CS, B_RST, B_SDA, B_SCL;
		_current_font cfont;

		void LCD_Writ_Bus(uint8_t VH, uint8_t VL);
		void LCD_Write_COM(uint8_t VL);
		void LCD_Write_DATA(uint8_t VH, uint8_t VL);
		void LCD_Write_DATA(uint8_t VL);
		void LCD_Write_COM_DATA(uint8_t com1, uint16_t dat1);
		void LCD_Write_COM_DATA8(uint8_t com1, uint8_t dat1);
		void setPixel(uint8_t r, uint8_t g, uint8_t b);
		void drawHLine(int x, int y, int l);
		void drawVLine(int x, int y, int l);
		void printChar(byte c, int x, int y);
		void setXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		void clrXY();
		void rotateChar(byte c, int x, int y, int pos, int deg);
		void _set_direction_registers();
		
	private:
		bool isParallel() const;
};

#endif