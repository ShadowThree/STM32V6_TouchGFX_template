#include <gui/screen1_screen/Screen1View.hpp>

#ifndef SIMULATOR
extern "C" {
#include "main.h"
#include <string.h>
#include "atk_ncr.h"
#include "dbger.h"
extern volatile uint16_t point_num;
}
#endif

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
#ifndef SIMULATOR
	Bitmap::cacheAll();
#endif
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::handWriteClear()
{
#ifndef SIMULATOR
	memset((void*)LTDC_L1_ADDR, 0xFF, LTDC_L1_WIDTH * LTDC_L1_HEIGHT);
	alientek_ncr(draw_coor, point_num, CHAR_NUM, RECOGNIZE_Aa1, result);
	LOG_DBG("result: %s\n", result);
	point_num = 0;
#endif
}
