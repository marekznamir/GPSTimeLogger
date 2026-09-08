#ifndef __WIDGETS_HPP__
#define __WIDGETS_HPP__

#include "u8g2.h"

class Widget {
protected:
    bool isSelected = false;
    uint16_t offsetToReturn;
public:
    u8g2_uint_t y_offset;
    virtual ~Widget() = default;
    virtual void draw() = 0;
    virtual uint16_t offset() = 0;

    virtual void setSelected() = 0;
    virtual void setUnselected() = 0;
};

extern u8g2_t u8g2;

#include "Title.hpp"
#include "OptionBox.hpp"

#endif