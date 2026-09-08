#pragma once
#include "u8g2.h"
#include "Screens.hpp"

constexpr uint8_t screensSize = 2;

class DisplayController {
private:
	IScreen* currScreen = nullptr;
	IScreen* prevScreen = nullptr;
	IScreen* screens[screensSize];

	uint8_t screenIndex = 0;
public:
	void init();
	void render();
	uint8_t* GetScreenBuffer();
	void setCurrScreen(IScreen* screen);
	void backFromJump();

	void buttonView();
	void buttonUp();
	void buttonDown();
	void buttonOk();
	void buttonBack();
	void buttonFn();
};

