#include "DisplayController.hpp"
#include "Screens.hpp"

#include "utility"

void DisplayController::init()
{
    u8g2_Setup_uc1608_240x128_f(
        &u8g2,
        U8G2_R0,
        u8x8_dummy_cb,
        u8x8_dummy_cb
    );

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_SetContrast(&u8g2, 50);
    u8g2_ClearBuffer(&u8g2);

    static MainScreen mainScreen;
    static Settings settings;

    screens[0] = &mainScreen;
    screens[1] = &settings;

    currScreen = &settings;
}

void DisplayController::render()
{
    if (currScreen != nullptr)
    {
        currScreen->draw();
    }
}

uint8_t* DisplayController::GetScreenBuffer()
{
    return u8g2_GetBufferPtr(&u8g2);
}

void DisplayController::setCurrScreen(IScreen* screen)
{
    prevScreen = currScreen;
    currScreen = screen;
}

void DisplayController::backFromJump()
{
    std::swap(currScreen, prevScreen);
}

void DisplayController::buttonView()
{
    if (screenIndex <= 256 && screenIndex < (screensSize - 1))
    {
        screenIndex++;
    }
    else
    {
        screenIndex = 0;
    }
    currScreen = screens[screenIndex];
}

void DisplayController::buttonUp() { if (currScreen) currScreen->buttonUp(); }
void DisplayController::buttonDown() { if (currScreen) currScreen->buttonDown(); }
void DisplayController::buttonOk() { if (currScreen) currScreen->buttonOk(); }
void DisplayController::buttonBack() { if (currScreen) currScreen->buttonBack(); }
void DisplayController::buttonFn() { if (currScreen) currScreen->buttonFn(); }
