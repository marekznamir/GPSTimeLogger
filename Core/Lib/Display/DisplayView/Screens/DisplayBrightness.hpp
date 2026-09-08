#ifndef __DISPLAYBRIGHTNESS_HPP__
#define __DISPLAYBRIGHTNESS_HPP__

#include "Screens.hpp"
#include "DisplayController.hpp"

class DisplayBrightness : public IScreen {
private:
    u8g2_uint_t level = 0;
public:
    DisplayBrightness();
    u8g2_uint_t getLevel();

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