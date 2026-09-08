#ifndef __SCREENS_H__
#define __SCREENS_H__

#include "u8g2.h"
#include "Widgets.hpp"

#include <stdint.h>

class IScreen {
protected:
    uint16_t y_offset = 0;
public:
    virtual ~IScreen() = default;

    virtual void draw() = 0;
    virtual void init() = 0;

    virtual void buttonView() = 0;
    virtual void buttonUp() = 0;
    virtual void buttonDown() = 0;
    virtual void buttonOk() = 0;
    virtual void buttonBack() = 0;
    virtual void buttonFn() = 0;
};

extern u8g2_t u8g2;

#include "MainScreen.hpp"
#include "Settings.hpp"
#include "DisplayBrightness.hpp"

#endif