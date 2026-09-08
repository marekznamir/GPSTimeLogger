#include "Title.hpp"

#include <cstring>

Title::Title(const char text[], u8g2_uint_t y_offset_)
{
	std::memcpy(title, text, titleTextSize); // constexpr dla 16 wypadaloby
	title[titleTextSize] = '\0';
	y_offset = y_offset_;

	offsetToReturn = 25;
}

void Title::draw()
{
	u8g2_SetFont(&u8g2, u8g2_font_8x13B_mf);
	u8g2_DrawStr(&u8g2, 5, y_offset + 15, title);

	u8g2_DrawBox(&u8g2, 5, y_offset + 20, u8g2_GetDisplayWidth(&u8g2) - 10, 1);
}

uint16_t Title::offset()
{
	return offsetToReturn;
}

void Title::setSelected()
{
}

void Title::setUnselected()
{
}
