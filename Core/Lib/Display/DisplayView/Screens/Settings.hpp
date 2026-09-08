#ifndef __SETTINGS_HPP__
#define __SETTINGS_HPP__

#include "Screens.hpp"

constexpr uint8_t widgetsSize = 2;

class Settings : public IScreen {
private:
    uint16_t moveIndex;
    Widget* widgets[widgetsSize];
    Widget* currentWidget;
public:
    Settings();

    void draw() override;
    void init() override;
    void moveScreen(bool dir, bool toMove);

    void buttonView() override;
    void buttonUp() override;
    void buttonDown() override;
    void buttonOk() override;
    void buttonBack() override;
    void buttonFn() override;
};

#endif