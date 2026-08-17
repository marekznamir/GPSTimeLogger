#include "bq25892.h"
#include <stdint.h>

inline uint8_t mem_read(uint16_t reg_mem_address)
{
    uint8_t data;
    HAL_I2C_Mem_Read(BQ25892_I2C_Handle, BQ25892_I2C_ADDR, reg_mem_address, I2C_MEMADD_SIZE_8BIT, &data, 1, 0);

    return data;
}

inline void mem_write(uint16_t reg_mem_address, uint8_t data)
{
    HAL_I2C_Mem_Write(BQ25892_I2C_Handle, BQ25892_I2C_ADDR, reg_mem_address, I2C_MEMADD_SIZE_8BIT, &data, 1, 0);
}

void read_reg0b(void)
{
    uint8_t data = mem_read(REG0B_ADDR);

    uint8_t vbus_status_bits = (data & 0x07);
    uint8_t charging_status_bits = (data & 0x18);
    uint8_t power_good_status_bits = (data & 0x20);
    uint8_t vsys_regulation_status_bits = (data & 0x80);

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
            bq25892.vsys_regulation_status = NOT_IN_VSYSMIN_REG;
            break;
        case 0x01:
            bq25892.vsys_regulation_status = IN_VSYSMIN_REG;
    }
}

void read_reg0c(void)
{
    uint8_t data = mem_read(REG0C_ADDR);

    uint8_t watchdog_fault_bits = (data & 0x01);
    uint8_t boost_fault_bits = (data & 0x02);
    uint8_t chrg_fault_bits = (data & 0x0C);
    uint8_t bat_fault_bits = (data & 0x10);
    uint8_t ntc_fault_bits = (data & 0xE0);

    switch (watchdog_fault_bits)
    {
        case 0x00:
            bq25892.watchdog_fault = WATCHDOG_GOOD;
            break;
        case 0x01:
            bq25892.watchdog_fault = TIMER_EXPIRATION;
    }

    switch (boost_fault_bits)
    {
        case 0x00:
            bq25892.boost_fault = BOOST_GOOD;
            break;
        case 0x01:
            bq25892.boost_fault = BOOST_NOT_GOOD;
    }

    switch (chrg_fault_bits)
    {
        case 0x00:
            bq25892.chrg_fault = CHARGE_GOOD;
            break;
        case 0x01:
            bq25892.chrg_fault = INPUT_FAULT;
            break;
        case 0x02:
            bq25892.chrg_fault = THERMAL_SHUTDOWN;
            break;
        case 0x03:
            bq25892.chrg_fault = CHARGE_SAFETY_TIMER_EXPIRATION;
    }

    switch (bat_fault_bits) 
    {
        case 0x00:
            bq25892.bat_fault = BAT_GOOD;
            break;
        case 0x01:
            bq25892.bat_fault = BATOVP;
    }

    switch (ntc_fault_bits)
    {
        case 0x00:
            bq25892.ntc_fault = NTC_GOOD;
            break;
        case 0x02:
            bq25892.ntc_fault = TS_WARM;
            break;
        case 0x03:
            bq25892.ntc_fault = TS_COOL;
            break;
        case 0x05:
            bq25892.ntc_fault = TS_COLD;
            break;
        case 0x06:
            bq25892.ntc_fault = TS_HOT;
    }
}

void read_reg0e(void)
{
    uint8_t data = mem_read(REG0E_ADDR);

    uint8_t therm_stat_bits = (data & 0x01);

    switch (therm_stat_bits) {
        case 0x00:
            bq25892.therm_stat = THERM_REG_OFF;
            break;
        case 0x01:
            bq25892.therm_stat = THERM_REG_ON;
    }

    //bq25892.batv = 
    uint8_t bit_pos = 0x01;
    uint16_t batv = 0;
    for(uint8_t i = 0; i < 7; i++)
    {
        if(data & bit_pos)
        {
            batv += BATV_MAP[i];
        }
        bit_pos >>= 1;
    }
    bq25892.batv = batv;
}



void bq25892_int(void)
{ 
}

void bq25892_init(void)
{
    const uint8_t REG02_CONFIG_DATA = (0x1D && 0x01); // Start ADC conversion (CONV_START bit to 1)
    mem_write(REG02_ADDR, REG02_CONFIG_DATA);
}
