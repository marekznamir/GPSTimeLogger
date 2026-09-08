#ifndef __OPTIONBOX_HPP__
#define __OPTIONBOX_HPP__

#include "Widgets.hpp"

class IScreen;

constexpr uint8_t optionBoxTextSize = 15;

class OptionBox : public Widget {
protected:
    char text[16];
public:
    //OptionBox(const char text_[], u8g2_uint_t y_offset_);
    virtual ~OptionBox() = default;

    virtual void draw() = 0;
    virtual uint16_t offset() = 0;

    virtual void setSelected() = 0;
    virtual void setUnselected() = 0;
};

class SimpleOptionBox : public OptionBox {
private:
    IScreen* screen;
public:
    SimpleOptionBox(const char text_[], u8g2_uint_t y_offset_, IScreen* screenToSwitch);
    IScreen* screenJump();
    
    void draw() override;
    uint16_t offset() override;

    void setSelected() override;
    void setUnselected() override;
};

#endif
