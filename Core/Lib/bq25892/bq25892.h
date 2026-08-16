#ifndef __BQ25892__
#define __BQ25892__

#define BQ25892_I2C_ADDR 0x6B // device I2C address

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

extern enum VBUS_STATUS {
    NO_INPUT_OR_USB_HOST_SDP,
    USB_HOST_SDP,
    USB_CDP_OR_ADAPTER,
    USB_DCP,
    DCP_MAX_CHARGE,
    UNKNOWN_ADAPTER,
    NON_STANDARD_ADAPTER, 
    USB_OTG 
};

extern enum CHARGING_STATUS {
    NOT_CHARGING,
    PRE_CHARGE,
    FAST_CHARGING,
    CHARGE_TERMINATION_DONE
};

extern enum POWER_GOOD_STATUS {
    NOT_POWER_GOOD,
    POWER_GOOD
}

extern enum VSYS_REGULATION_STATUS {
    NOT_IN_VSYSMIN_REGULATION,
    IN_VSYSMIN_REGULATION
}

extern struct BQ25892 {
    uint16_t i2c_addr;
    VBUS_STATUS vbus_status;
    CHARGING_STATUS charging_status;
    POWER_GOOD_STATUS power_good_status;
    VSYS_REGULATION_STATUS vsys_regulation_status;
};

extern static BQ25892 bq25892;

void bq25892_init(void);

void bq25892_int(void);

#endif __BQ25892__