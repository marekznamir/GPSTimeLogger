#ifndef __MAINSCREEN_HPP__
#define __MAINSCREEN_HPP__

#include "Screens.hpp"

class MainScreen : public IScreen {
public:
    MainScreen();

    void draw() override;
    void init() override;

    void buttonView() override;
    void buttonUp() override;
    void buttonDown() override;
    void buttonOk() override;
    void buttonBack() override;
    void buttonFn() override;
};

#endif