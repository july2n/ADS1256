#ifndef __SSD1306_TEST_H__
#define __SSD1306_TEST_H__

#include "ssd1306.h"
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"

void ssd1306_TestBorder(void);
void ssd1306_TestFonts(void);
void ssd1306_TestFPS(void);
void ssd1306_TestAll(void);
void ssd1306_TestLine(void);
void ssd1306_TestRectangle(void);
void ssd1306_TestCircle(void);
void ssd1306_TestArc(void);
void ssd1306_TestPolyline(void);
void ssd1306_TestDrawBitmap(void);

#endif // __SSD1306_TEST_H__
