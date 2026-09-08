#include "OptionBox.hpp"

#include <cstring>

SimpleOptionBox::SimpleOptionBox(const char text_[], u8g2_uint_t y_offset_, IScreen* screenToSwitch)
{
	std::memcpy(text, text_, optionBoxTextSize);
	text[optionBoxTextSize] = '\0';

	y_offset = y_offset_;

	screen = screenToSwitch;

	offsetToReturn = 25;
}

IScreen* SimpleOptionBox::screenJump()
{
	return screen;
}

void SimpleOptionBox::draw()
{
	u8g2_SetFont(&u8g2, u8g2_font_8x13B_mf);

	if (isSelected == false)
	{
		u8g2_SetFont(&u8g2, u8g2_font_8x13B_mf);
		u8g2_DrawStr(&u8g2, 8, y_offset + 15, text);

		u8g2_DrawFrame(&u8g2, 5, y_offset, u8g2_GetDisplayWidth(&u8g2) - 10, 20);
	}
	else
	{
		u8g2_DrawBox(&u8g2, 5, y_offset, u8g2_GetDisplayWidth(&u8g2) - 10, 20);
		u8g2_SetDrawColor(&u8g2, 0);

		u8g2_DrawStr(&u8g2, 8, y_offset + 15, text);

		u8g2_SetDrawColor(&u8g2, 4);
	}
}

uint16_t SimpleOptionBox::offset()
{
	return offsetToReturn;
}

void SimpleOptionBox::setSelected()
{
	isSelected = true;
}

void SimpleOptionBox::setUnselected()
{
	isSelected = false;
}