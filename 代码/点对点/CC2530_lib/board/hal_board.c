#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_mcu.h"
#include "hal_digio.h"
#include "hal_int.h"
#include "hal_board.h"
#include "hal_uart.h"

void halBoardInit(void)
{
    halMcuInit();
    
    // LEDs
    MCU_IO_OUTPUT(HAL_BOARD_IO_LED_1_PORT, HAL_BOARD_IO_LED_1_PIN, 0);
    MCU_IO_OUTPUT(HAL_BOARD_IO_LED_2_PORT, HAL_BOARD_IO_LED_2_PIN, 0);
    MCU_IO_OUTPUT(HAL_BOARD_IO_LED_3_PORT, HAL_BOARD_IO_LED_3_PIN, 0);
    MCU_IO_OUTPUT(HAL_BOARD_IO_LED_4_PORT, HAL_BOARD_IO_LED_4_PIN, 0);
    
    halUartInit(115200); //初始化串口0的波特率为38400（接Router）
    
    halIntOn();
}



