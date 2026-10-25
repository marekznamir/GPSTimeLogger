#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void NEO_M9N_RxEventCallback(uint16_t size);

void CLIENT_RxEventCallback(uint16_t size);

uint8_t UBXFrameDecode(uint8_t* frame, uint8_t* payload);

union UBX_NAV_PVT {
    struct PVT {
        uint32_t iTOW;          // [ms] GPS time of week of the navigation epoch
        uint16_t year;          // [y] Year (UTC)
        uint8_t  month;         // [month] Month, range 1..12 (UTC)
        uint8_t  day;           // [d] Day of month, range 1..31 (UTC)
        uint8_t  hour;          // [h] Hour of day, range 0..23 (UTC)
        uint8_t  min;           // [min] Minute of hour, range 0..59 (UTC)
        uint8_t  sec;           // [s] Seconds of minute, range 0..60 (UTC)
        // Validity flags
        uint8_t  validDate     : 1; // 1 = valid UTC Date
        uint8_t  validTime     : 1; // 1 = valid UTC time of day
        uint8_t  fullyResolved : 1; // 1 = UTC time of day has been fully resolved (no seconds uncertainty)
        uint8_t  validMag      : 1; // 1 = valid magnetic declination
        uint8_t                : 4;
        uint32_t tAcc;          // [ns] Time accuracy estimate (UTC)
        int32_t  nano;          // [ns] Fraction of second, range -1e9 .. 1e9 (UTC)
        uint8_t  fixType;       // GNSSfix Type
        // Fix status flags
        uint8_t  gnssFixOK     : 1; // 1 = valid fix (i.e within DOP & accuracy masks)
        uint8_t  diffSoln      : 1; // 1 = differential corrections were applied
        uint8_t  psmState      : 3; // Power save mode state
        uint8_t  headVehValid  : 1; // 1 = heading of vehicle is valid, only set if the receiver is in sensor fusion mode
        uint8_t  carrSoln      : 2; // Carrier phase range solution status
        // Additional flags
        uint8_t                : 5;
        uint8_t  confirmedAvai : 1; // 1 = information about UTC Date and Time of Day validity confirmation is available
        uint8_t  confirmedDate : 1; // 1 = UTC Date validity could be confirmed
        uint8_t  confirmedTime : 1; // 1 = UTC Time of Day could be confirmed
        uint8_t  numSV;         // Number of satellites used in Nav Solution
        int32_t  lon;           // [1e-7 deg] Longitude
        int32_t  lat;           // [1e-7 deg] Latitude
        int32_t  height;        // [mm] Height above ellipsoid
        int32_t  hMSL;          // [mm] Height above mean sea level
        uint32_t hAcc;          // [mm] Horizontal accuracy estimate
        uint32_t vAcc;          // [mm] Vertical accuracy estimate
        int32_t  velN;          // [mm/s] NED north velocity
        int32_t  velE;          // [mm/s] NED east velocity
        int32_t  velD;          // [mm/s] NED down velocity
        int32_t  gSpeed;        // [mm/s] Ground Speed (2-D)
        int32_t  headMot;       // [1e-5 deg] Heading of motion (2-D)
        uint32_t sAcc;          // [mm/s] Speed accuracy estimate
        uint32_t headAcc;       // [1e-5 deg] Heading accuracy estimate (both motion and vehicle)
        uint16_t pDOP;          // [0.01] Position DOP
        // Additional flags
        uint8_t  invalidLlh    : 1; // 1 = Invalid lon, lat, height and hMSL
        uint8_t                : 7;
        uint8_t  reserved0[5];  // Reserved
        int32_t  headVeh;       // [1e-5 deg] Heading of vehicle (2-D)
        int16_t  magDec;        // [1e-2 deg] Magnetic declination
        uint16_t magAcc;        // [1e-2 deg] Magnetic declination accuracy
    } pvt;

    uint8_t data[92];
};

#ifdef __cplusplus
}
#endif