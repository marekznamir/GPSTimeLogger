#include "bq25892.h"
#include "i2c.h"
#include "stm32h7xx_hal_i2c.h"
#include <stdint.h>
#include <string.h>

void bq25892_init(void)
{

}

uint8_t* mem_read(uint16_t reg_mem_adress)
{
    uint8_t* data;
    HAL_I2C_Mem_Read(hi2c1, BQ25892_I2C_ADDR, reg_mem_adress, I2C_MEMADD_SIZE_8BIT, data, 1, 0)

    return data;
}

void handle_reg0b(void)
{
    uint8_t* data;
    data = mem_read(REG0B_ADDR);

    uint8_t vbus_status_bits = (*data & 0x07);
    uint8_t charging_status_bits = (*data & 0x18);
    uint8_t power_good_status_bits = (*data & 0x20);
    uint8_t vsys_regulation_status_bits = (*data & 0x80);

    switch (vbus_status_bits)
    {
        case 0x00:
            bq25892.vbus_status = NO_INPUT_OR_USB_HOST_SDP;
            break;
        case 0x01:
            bq25892.vbus_status = USB_HOST_SDP;
            break;
        case 0x02:
            bq25892.vbus_status = USB_CDP_OR_ADAPTER;
            break;
        case 0x03:
            bq25892.vbus_status = USB_DCP;
            break;
        case 0x04:
            bq25892.vbus_status = DCP_MAX_CHARGE;
            break;
        case 0x05:
            bq25892.vbus_status = UNKNOWN_ADAPTER;
            break;
        case 0x06:
            bq25892.vbus_status = NON_STANDARD_ADAPTER;
            break;
        case 0x07:
            bq25892.vbus_status = USB_OTG;
    }

    switch (charging_status_bits) 
    {
        case 0x00:
            bq25892.charging_status = NOT_CHARGING;
            break;
        case 0x01:
            bq25892.charging_status = PRE_CHARGE;
            break;
        case 0x02:
            bq25892.charging_status = FAST_CHARGING;
            break;
        case 0x03:
            bq25892.charging_status = CHARGE_TERMINATION_DONE;
    }

    switch (power_good_status_bits)
    {
        case 0x00:
            bq25892.power_good_status = NOT_CHARGING;
            break;
        case 0x01:
            bq25892.charging_status = POWER_GOOD;
    }

    switch (vsys_regulation_status_bits)
    {
        case 0x00:
            bq25892.vsys_regulation_status = NOT_IN_VSYSMIN_REGULATION;
            break;
        case 0x01:
            bq25892.vsys_regulation_status = IN_VSYSMIN_REGULATION;
    }
}

void handle_reg0c(void)
{
    uint8_t* data;
    data = mem_read(REG0C_ADDR);



}

void bq25892_int(void)
{
    




    
}