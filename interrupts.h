/*
 * interrupts.h
 *
 *  Created on: 07/04/2013
 *      Author: Michael
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include <math.h>
#include <stdint.h>
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "ff.h"
#include "diskio.h"
#include "buttons.h"
#include "driverlib/rom.h"

#define foreach(item, array) \
    for(int keep = 1, \
            count = 0,\
            size = sizeof (array) / sizeof *(array); \
        keep && count != size; \
        keep = !keep, count++) \
      for(item = (array) + count; keep; keep = !keep)

void keyPressHandler1(void);
void initButtons1(void);




#endif /* INTERRUPTS_H_ */
