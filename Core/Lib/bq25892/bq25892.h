#ifndef __BQ25892__
#define __BQ25892__

#include "i2c.h"
#include "stm32h7xx_hal_i2c.h"
#include <stdint.h>
#include <string.h>

#define BQ25892_I2C_ADDR (0x6B << 1) // device I2C address
#define BQ25892_I2C_Handle &hi2c1 // i2c handle

#define REG00_ADDR 0x00
#define REG01_ADDR 0x01
#define REG02_ADDR 0x02
#define REG03_ADDR 0x03
#define REG04_ADDR 0x04
#define REG05_ADDR 0x05
#define REG06_ADDR 0x06
#define REG07_ADDR 0x07
#define REG08_ADDR 0x08
#define REG09_ADDR 0x09
#define REG0A_ADDR 0x0A
#define REG0B_ADDR 0x0B
#define REG0C_ADDR 0x0C
#define REG0D_ADDR 0x0D
#define REG0E_ADDR 0x0E
#define REG0F_ADDR 0x0F
#define REG10_ADDR 0x10
#define REG11_ADDR 0x11
#define REG12_ADDR 0x12
#define REG13_ADDR 0x13
#define REG14_ADDR 0x14

// REG0B STATUS

typedef enum {
    NO_INPUT_OR_USB_HOST_SDP,
    USB_HOST_SDP,
    USB_CDP_OR_ADAPTER, // (1,5A or 3,25A)
    USB_DCP, // (3,25A)
    DCP_MAX_CHARGE, // (1,5A)
    UNKNOWN_ADAPTER, // (500mA)
    NON_STANDARD_ADAPTER, // (1A/2A/2.1A/2.4A)
    USB_OTG 
} VBUS_STAT;

typedef enum {
    NOT_CHARGING,
    PRE_CHARGE, // (< V_BATLOWV)
    FAST_CHARGING,
    CHARGE_TERMINATION_DONE
} CHRG_STAT;

typedef enum {
    NOT_POWER_GOOD,
    POWER_GOOD
} PG_STAT;

typedef enum {
    NOT_IN_VSYSMIN_REG, // (BAT > VSYSMIN)
    IN_VSYSMIN_REG // (BAT < VSYSMIN)
} VSYS_STAT;

// REG0C STATUS

typedef enum {
    WATCHDOG_GOOD,
    TIMER_EXPIRATION
} WATCHDOG_FAULT;

typedef enum {
    BOOST_GOOD,
    BOOST_NOT_GOOD // VBUS overloaded in OTG, or VBUS OVP, or battery is too low in boost mode
} BOOST_FAULT;

typedef enum {
    CHARGE_GOOD,
    INPUT_FAULT, // (VBUS > VACOV or VBAT < VBUS < VVBUSMIN(typical 3.8V) )
    THERMAL_SHUTDOWN,
    CHARGE_SAFETY_TIMER_EXPIRATION
} CHRG_FAULT;

typedef enum {
    BAT_GOOD,
    BATOVP // (VBAT > VBATOVP)
} BAT_FAULT;

typedef enum {
    NTC_GOOD = 0x00,
    TS_WARM = 0x02,
    TS_COOL = 0x03,
    TS_COLD = 0x05,
    TS_HOT = 0x06
} NTC_FAULT;

// REG0E Status

typedef enum {
    THERM_REG_OFF, // Normal
    THERM_REG_ON // In Thermal Regulation
} THERM_STAT;

uint16_t BATV_MAP[7] = 
{   
     1280, 
     640,
     320,
     160,
     80,
     40,
     20
}; // [mV]

typedef struct  {
    VBUS_STAT vbus_status;
    CHRG_STAT charging_status;
    PG_STAT power_good_status;
    VSYS_STAT vsys_regulation_status;

    WATCHDOG_FAULT watchdog_fault;
    BOOST_FAULT boost_fault;
    CHRG_FAULT chrg_fault;
    BAT_FAULT bat_fault;
    NTC_FAULT ntc_fault;

    THERM_STAT therm_stat;
    uint16_t batv;
} BQ25892;

static BQ25892 bq25892;

void bq25892_init(void);

void bq25892_int(void);

#endif