#include "Screens.hpp"
#include "Settings.hpp"
#include "Widgets.hpp"
#include "DisplayController.hpp"

#include <cstring>
#include <vector>
#include <iostream>

extern DisplayController displayController;

Settings::Settings()
{
	init();
}

void Settings::draw()
{
	u8g2_ClearBuffer(&u8g2);

	for (uint8_t i = 0; i < widgetsSize; i++)
	{
		widgets[i]->draw();
	}

	u8g2_DrawFrame(&u8g2, 0, 0, u8g2_GetDisplayWidth(&u8g2), u8g2_GetDisplayHeight(&u8g2));

	u8g2_SendBuffer(&u8g2);
}

void Settings::init()
{

	static Title titleSettings("Settings", y_offset);
	this->y_offset += titleSettings.offset();
	widgets[0] = &titleSettings;

	static DisplayBrightness displayBrightness;

	static SimpleOptionBox displayBrightnessBox("Brightness", y_offset, &displayBrightness);
	this->y_offset += displayBrightnessBox.offset();
	widgets[1] = &displayBrightnessBox;

	/*static SimpleOptionBox option2("option2", y_offset);
	this->y_offset += option2.offset();
	widgets.push_back(&option2);

	static SimpleOptionBox option3("option3", y_offset);
	this->y_offset += option3.offset();
	widgets.push_back(&option3);

	static SimpleOptionBox option6("option6", y_offset);
	this->y_offset += option6.offset();
	widgets.push_back(&option6);

	static SimpleOptionBox option7("option7", y_offset);
	this->y_offset += option7.offset();
	widgets.push_back(&option7);

	static Title titleTestowe("Testowe", y_offset);
	this->y_offset += titleTestowe.offset();
	widgets.push_back(&titleTestowe);

	static SimpleOptionBox option8("option8", y_offset);
	this->y_offset += option8.offset();
	widgets.push_back(&option8);

	static SimpleOptionBox option4("option4", y_offset);
	this->y_offset += option4.offset();
	widgets.push_back(&option4);

	static SimpleOptionBox option5("option5", y_offset);
	this->y_offset += option5.offset();
	widgets.push_back(&option5);*/
}

void Settings::moveScreen(bool dir, bool shouldScroll) // true = down, false = up, zamienic na enum potem
{
	if (dynamic_cast<OptionBox*>(widgets[moveIndex]) != nullptr)
	{
		widgets[moveIndex]->setSelected();
		currentWidget = widgets[moveIndex];

		if (shouldScroll == true)
		{
			if (dir == true)
			{
				for (uint16_t i = 0; i < widgetsSize; i++)
				{
					widgets[i]->y_offset -= widgets[i]->offset();
				}
			}
			else
			{
				for (uint16_t i = 0; i < widgetsSize; i++)
				{
					widgets[i]->y_offset += widgets[i]->offset();
				}
			}
		}
	}
	else if (dynamic_cast<Title*>(widgets[moveIndex]) != nullptr)
	{
		dir == true ? buttonDown() : buttonUp();
	}
}

void Settings::buttonView()
{
}
  
void Settings::buttonUp()
{
	if (moveIndex == 0) { return; }

	std::cout << moveIndex << std::endl;
	widgets[moveIndex]->setUnselected();
	moveIndex--;

	bool shouldScroll = (widgets[moveIndex]->y_offset + widgets[moveIndex]->offset()) < 70;
	moveScreen(false, shouldScroll);
}

void Settings::buttonDown()
{
	if (moveIndex == widgetsSize - 1) { return; }

	std::cout << moveIndex << std::endl;

	widgets[moveIndex]->setUnselected();
	moveIndex++;

	bool shouldScroll = (widgets[moveIndex]->y_offset + widgets[moveIndex]->offset()) > 70;
	moveScreen(true, shouldScroll);
}

void Settings::buttonOk()
{
	auto temp = dynamic_cast<SimpleOptionBox*>(currentWidget);
	if (temp != nullptr)
	{
		displayController.setCurrScreen(temp->screenJump());
	}
}

void Settings::buttonBack()
{
}

void Settings::buttonFn()
{
}
