
#ifndef _HAL_PWM_H_
#define _HAL_PWM_H_

#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_mcu.h"

void TIM1_PwmInit(uint16 period, uint8 ration);

#endif /*_HAL_PWM_H_*/
