#include "DisplayBrightness.hpp"

extern DisplayController displayController;

DisplayBrightness::DisplayBrightness()
{
}

u8g2_uint_t DisplayBrightness::getLevel()
{
	return level;
}

void DisplayBrightness::draw()
{
	u8g2_ClearBuffer(&u8g2);

	u8g2_DrawFrame(&u8g2, (u8g2_GetDisplayWidth(&u8g2) / 2) - 60, (u8g2_GetDisplayHeight(&u8g2) / 2) - 8, 124, 16);

	u8g2_SetFont(&u8g2, u8g2_font_8x13B_mf);
	u8g2_DrawStr(&u8g2, (u8g2_GetDisplayWidth(&u8g2) / 2) - 60, (u8g2_GetDisplayHeight(&u8g2) / 2) - 8 - 2, "0");
	u8g2_DrawStr(&u8g2, (u8g2_GetDisplayWidth(&u8g2) / 2) + 60 - 11, (u8g2_GetDisplayHeight(&u8g2) / 2) - 8 - 2, "10");

	u8g2_DrawBox(&u8g2, (u8g2_GetDisplayWidth(&u8g2) / 2) - 60 + 2, (u8g2_GetDisplayHeight(&u8g2) / 2) - 8 + 2, 12 * level, 12);

	u8g2_DrawFrame(&u8g2, 0, 0, u8g2_GetDisplayWidth(&u8g2), u8g2_GetDisplayHeight(&u8g2));

	u8g2_SendBuffer(&u8g2);
}

void DisplayBrightness::init()
{
}

void DisplayBrightness::buttonView()
{
}

void DisplayBrightness::buttonUp()
{
	if (level < 10)
	{
		level++;
	}
}

void DisplayBrightness::buttonDown()
{
	if (level > 0)
	{
		level--;
	}
}

void DisplayBrightness::buttonOk()
{

}

void DisplayBrightness::buttonBack()
{
	displayController.backFromJump();
}

void DisplayBrightness::buttonFn()
{
}
