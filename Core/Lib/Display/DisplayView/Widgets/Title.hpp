#ifndef __TITLE_HPP__
#define __TITLE_HPP__

#include "Widgets.hpp"

constexpr uint8_t titleTextSize = 15;

class Title : public Widget {
private:
    char title[16];
public:
    Title(const char text[], u8g2_uint_t y_offset_);
    void draw() override;
    uint16_t offset() override;

    void setSelected() override;
    void setUnselected() override;
};

#endif


