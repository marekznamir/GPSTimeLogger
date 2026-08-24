#ifndef __NEO_M9N__
#define __NEO_M9N__

#include "usart.h"
#include "gpio.h"
#include "stm32h7xx_hal_uart.h"

#define NEO_M9N_USARD_Handle &huart1

#define UBX_SYNC_1 0xB5
#define UBX_SYNC_2 0x62

#define UBX_CFG_VALSET_CLASS 0x06
#define UBX_CFG_VALSET_ID 0x8A

#define CFG_UART1OUTPROT_UBX 0x10740001
#define CFG_UART1OUTPROT_NMEA 0x10740002
#define CFG_MSGOUT_UBX_NAV_PVT_UART1 0x20910007


void neo_m9n_init(void);

void neo_m9n_pps_int(void);

void neo_m9n_eint(void);



#endif