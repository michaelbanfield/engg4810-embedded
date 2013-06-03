//*****************************************************************************
//
// usb_dev_msc.c - Main routines for the device mass storage class example.
//
// Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 9453 of the EK-LM4F232 Firmware Package.
//
//*****************************************************************************

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/systick.h"
#include "driverlib/usb.h"
#include "driverlib/gpio.h"
#include "driverlib/udma.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"
#include "grlib/grlib.h"
#include "usblib/usblib.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdmsc.h"
#include "drivers/cfal96x64x16.h"
#include "usb_msc_structs.h"
#include "third_party/fatfs/src/diskio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include <math.h>
#include <stdint.h>
#include "driverlib/fpu.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "ff.h"
#include "diskio.h"
#include "globals.h"
#include "driverlib/rom.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/systick.h"
#include "driverlib/usb.h"
#include "driverlib/gpio.h"
#include "driverlib/udma.h"
#include "driverlib/pin_map.h"
#include "usblib/usblib.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdmsc.h"
#include "usb_msc_structs.h"
#include "third_party/fatfs/src/diskio.h"
#include "arm_math.h"
#include "math_helper.h"
#include <stdbool.h>

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>USB MSC Device (usb_dev_msc)</h1>
//!
//! This example application turns the evaluation board into a USB mass storage
//! class device.  The application will use the microSD card for the storage
//! media for the mass storage device.  The screen will display the current
//! action occurring on the device ranging from disconnected, no media, reading,
//! writing and idle.
//
//*****************************************************************************

//*****************************************************************************
//
// These defines are used to define the screen constraints to the application.
//
//*****************************************************************************
#define DISPLAY_BANNER_HEIGHT   11
#define DISPLAY_BANNER_BG       ClrDarkBlue
#define DISPLAY_BANNER_FG       ClrWhite

//*****************************************************************************
//
// The number of ticks to wait before falling back to the idle state.  Since
// the tick rate is 100Hz this is approximately 3 seconds.
//
//*****************************************************************************
#define USBMSC_ACTIVITY_TIMEOUT 300
#define TEST_LENGTH_SAMPLES  128
#define SNR_THRESHOLD_F32    140.0f
#define BLOCK_SIZE            32
#define NUM_TAPS              29
#define NUM_SAMPLES			128
int pointer = 128;
int index2 = 128;
float32_t pCoeffs[5];
extern float32_t testInput_f32_1kHz_15kHz[TEST_LENGTH_SAMPLES];
bool tempoLit = false;

char playingBlock = 0;
char loadingBlock = 1;
bool sampleWaiting = false;
int blockPointer = 0;
float tempo = 0;
char playingBlock_2 = 0;
char loadingBlock_2 = 1;
bool sampleWaiting_2 = false;
int blockPointer_2 = 0;

int playingTracks[2] = {-1, -1};
int loadTrack = 0;
int pot1Value = 0;
int pot2Value = 0;
int pot3Value = 0;
int pot4Value = 0;
bool isDecimator = false;

#define NUM_SSI_DATA 3
int oldTrack = -1;

int highPins[4] = {GPIO_PIN_5, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_4};
int lowPins[4] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3};
wav_player wavPlayer;
config mpcConfig;
static float32_t state1[4];
static float32_t state2[4];
static float32_t state3[4];
static float32_t state4[4];
bool isBitwise = false;

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES / BLOCK_SIZE;
float32_t snr;

void lowPassFunction(int loop, int potValue1, int potValue2);
void highPassFunction(int i, int potValue1, int potValue2);
void emptyFunction(int i, int a, int b);
void bandPassFunction(int i, int potValue1, int potValue2);
void notchPassFunction(int i, int potValue1, int potValue2);
void checkADC();

void (*func_ptr[5]) = {emptyFunction, lowPassFunction, highPassFunction, bandPassFunction, notchPassFunction};

volatile enum {
    //
    // Unconfigured.
    //
    MSC_DEV_DISCONNECTED,

    //
    // Connected but not yet fully enumerated.
    //
    MSC_DEV_CONNECTED,

    //
    // Connected and fully enumerated but not currently handling a command.
    //
    MSC_DEV_IDLE,

    //
    // Currently reading the SD card.
    //
    MSC_DEV_READ,

    //
    // Currently writing the SD card.
    //
    MSC_DEV_WRITE,
} g_eMSCState;


#define FLAG_UPDATE_STATUS      1
static unsigned long g_ulFlags;
static unsigned long g_ulIdleTimeout;


tContext g_sContext;


#ifdef ewarm
#pragma data_alignment=1024
tDMAControlTable sDMAControlTable[64];
#elif defined(ccs)
#pragma DATA_ALIGN(sDMAControlTable, 1024)
tDMAControlTable sDMAControlTable[64];
#else
tDMAControlTable sDMAControlTable[64] __attribute__((aligned(1024)));
#endif



unsigned long RxHandler(void *pvCBData, unsigned long ulEvent,
        unsigned long ulMsgValue, void *pvMsgData) {
    return (0);
}


unsigned long TxHandler(void *pvCBData, unsigned long ulEvent,
        unsigned long ulMsgValue, void *pvMsgData) {
    return (0);
}



unsigned long USBDMSCEventCallback(void *pvCBData, unsigned long ulEvent,
        unsigned long ulMsgParam, void *pvMsgData) {
    g_ulIdleTimeout = USBMSC_ACTIVITY_TIMEOUT;

    switch (ulEvent) {
        case USBD_MSC_EVENT_WRITING:
        {
            if (g_eMSCState != MSC_DEV_WRITE) {
                g_eMSCState = MSC_DEV_WRITE;
                g_ulFlags |= FLAG_UPDATE_STATUS;
            }

            break;
        }
        case USBD_MSC_EVENT_READING:
        {
            if (g_eMSCState != MSC_DEV_READ) {
                g_eMSCState = MSC_DEV_READ;
                g_ulFlags |= FLAG_UPDATE_STATUS;
            }

            break;
        }
        case USB_EVENT_DISCONNECTED:
        {
            g_eMSCState = MSC_DEV_DISCONNECTED;
            g_ulFlags |= FLAG_UPDATE_STATUS;

            break;
        }
        case USB_EVENT_CONNECTED:
        {
            g_eMSCState = MSC_DEV_IDLE;

            break;
        }
        case USBD_MSC_EVENT_IDLE:
        default:
        {
            break;
        }
    }

    return (0);
}

void SysTickHandler(void) {
    disk_timerproc();

    if (g_ulIdleTimeout != 0) {
        g_ulIdleTimeout--;
    }
}

int mainUSB(void) {

    unsigned long ulRetcode;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    SysCtlDelay(10);
    uDMAControlBaseSet(&sDMAControlTable[0]);
    uDMAEnable();
    g_ulIdleTimeout = 0;
    g_ulFlags = 0;
    g_eMSCState = MSC_DEV_DISCONNECTED;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    USBStackModeSet(0, USB_MODE_DEVICE, 0);
    USBDMSCInit(0, (tUSBDMSCDevice *) & g_sMSCDevice);
    ulRetcode = disk_initialize(0);
    return 1;

}


void InitConsole(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioInit(0);
}

void periphSetup(void) {
    unsigned long ulDataRx[NUM_SSI_DATA];
    //setup dac
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);

    //setup SSI 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PF2_SSI1CLK);
    GPIOPinConfigure(GPIO_PF3_SSI1FSS);
    GPIOPinConfigure(GPIO_PF0_SSI1RX);
    GPIOPinConfigure(GPIO_PF1_SSI1TX);
    GPIOPinTypeSSI(GPIO_PORTF_BASE,
            GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_0 | GPIO_PIN_1);
    //set SSI 1 mode and clock
    SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
            SSI_MODE_MASTER, 20000000, 16);
    SSIEnable(SSI1_BASE);
    //clear FIFO
    while (SSIDataGetNonBlocking(SSI1_BASE, &ulDataRx[0])) {
    }

}

void openAndReadHeader(int loopNumber) {
    UINT br = 0;
    f_open(&wavPlayer.files[loopNumber], filenames[loopNumber],
            FA_OPEN_EXISTING | FA_READ);
    f_read(&wavPlayer.files[0], &wavPlayer.headers[loopNumber],
            sizeof wavPlayer.headers[0], &br);
}

void loadSample(int loopNumber) {
    UINT br = 0;
    f_read(&wavPlayer.files[loopNumber],
            &wavPlayer.samples[loopNumber].sample[loadingBlock],
            sizeof wavPlayer.samples[0].sample[0], &br);

    if (br < (sizeof wavPlayer.samples[0].sample[0])) {

        if (wavPlayer.loop[loopNumber]) {
            f_lseek(&wavPlayer.files[loopNumber], sizeof wavPlayer.samples[0].sample);
        } else {
            if (playingTracks[0] == loopNumber) {
                playingTracks[0] = -1;
            } else {
                playingTracks[1] = -1;
            }
        }
    }
}

void loadSamples(int loop1, int loop2) {
    UINT br = 0, br2 = 0;
    f_read(&wavPlayer.files[loop1],
            &wavPlayer.samples[loop1].sample[loadingBlock],
            sizeof wavPlayer.samples[0].sample[0], &br);
    f_read(&wavPlayer.files[loop2],
            &wavPlayer.samples[loop2].sample[loadingBlock],
            sizeof wavPlayer.samples[0].sample[0], &br2);

    if (br < (sizeof wavPlayer.samples[0].sample[0])) {
        if (wavPlayer.loop[loop1]) {
            f_lseek(&wavPlayer.files[loop1], sizeof wavPlayer.samples[0].sample);
        } else {
            playingTracks[0] = -1;
        }
    }
    if (br2 < (sizeof wavPlayer.samples[0].sample[0])) {
        if (wavPlayer.loop[loop2]) {
            f_lseek(&wavPlayer.files[loop2], sizeof wavPlayer.samples[0].sample);
        } else {
            playingTracks[1] = -1;
        }


    }

}

void loadSample2(int loopNumber) {
    UINT br = 0;
    f_read(&wavPlayer.files[loopNumber],
            &wavPlayer.samples[loopNumber].sample[loadingBlock_2],
            sizeof wavPlayer.samples[0].sample[0], &br);

    if (br < (sizeof wavPlayer.samples[0].sample[0])) {
        if (wavPlayer.loop[loopNumber]) {
            f_lseek(&wavPlayer.files[loopNumber], sizeof wavPlayer.samples[0]);
        } else {
            wavPlayer.samples[loopNumber].isPlaying = 0;
        }
    }
}

void setRowPinHigh(int index) {
    GPIOPinWrite(GPIO_PORTB_BASE, highPins[index], highPins[index]);

}

void setRowPinLow(int index) {
    GPIOPinWrite(GPIO_PORTB_BASE, highPins[index], 0);
}

void scanButtons() {
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5, 0);
    int i, j = 0;
    int track = -1;
    for (i = 0; i < 4; i++) {
        setRowPinHigh(i);
        for (j = 0; j < 4; j++) {
            if (GPIOPinRead(GPIO_PORTD_BASE, lowPins[j])) {
                if (!GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6)) {
                    wavPlayer.loop[4 * j + i] = 1;
                    UARTprintf("Winning\n");
                }
                UARTprintf(" %d\n", 4 * j + i);
                track = 4 * j + i;
                if ((playingTracks[0] != track)
                        && (playingTracks[1] != track)) {
                    wavPlayer.loop[track] = 0;
                    playingTracks[loadTrack] = track;
                    f_lseek(&wavPlayer.files[4 * j + i],
                            sizeof wavPlayer.samples[0].sample);
                    loadTrack = !loadTrack;

                }
            } else if (playingTracks[!loadTrack] == 4 * j + i) {
                if (mpcConfig.hold[4 * j + i]) {
                    playingTracks[!loadTrack] = -1;
                }

            }

        }

        setRowPinLow(i);
    }

    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);
}

void keyPressHandler(void) {
    GPIOPinIntClear(GPIO_PORTD_BASE,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

void initButtons(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPortIntRegister(GPIO_PORTD_BASE, keyPressHandler);
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPD);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);
    GPIOIntTypeSet(GPIO_PORTD_BASE,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
            GPIO_RISING_EDGE);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinWrite(GPIO_PORTD_BASE,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
}

void Timer2IntHandler(void) {
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    checkADC();
    scanButtons();
}

void Timer3IntHandler(void) {
    TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    if (tempoLit == false) {
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5);
        tempoLit = true;
    } else {
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);
        tempoLit = false;

    }

}

void Timer0IntHandler(void) {
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    float value;
    if ((playingTracks[0] < 0) && (playingTracks[1] < 0)) {
        return;
    } else if (playingTracks[0] < 0) {
        value = wavPlayer.samples[playingTracks[1]].sample[playingBlock][blockPointer];
    } else {
        value = wavPlayer.samples[playingTracks[0]].sample[playingBlock][blockPointer];

    }
    blockPointer++;

    if (blockPointer >= NUM_SAMPLES) {
        if (playingBlock == 1) {
            playingBlock = 0;
            loadingBlock = 1;
        } else {
            playingBlock = 1;
            loadingBlock = 0;
        }
        blockPointer = 0;
        sampleWaiting = true;
    }
    if(isDecimator) {
    	value = value * (0xFFF - pot4Value);
    } else {
    	value = value * 0xFFF;
    }

    value = value / 2;
    value += (0xFFF / 2);
    uint16_t write = 0;
    if(isBitwise) {
    	//UARTprintf("yeahkarnn\n");
    	uint16_t looseValue =  4096/pot3Value;
    	uint16_t looseValue2 =  4096/pot4Value;
    	uint16_t intValue = (uint16_t) value;
    	if(pot3Value > 1 ) {
    		intValue = intValue & looseValue;
    	}
    	if(pot4Value > 1) {
    		intValue = intValue ^ looseValue2;
    	}
    	write = 0x0FFF & (uint16_t) intValue;

    } else {
    	write = 0x0FFF & (uint16_t) value;

    }
    write = 0x3000 | write;

    if (isDecimator) {
    	//UARTprintf("yeahkarnn\n");
        if (pot3Value > 10) {
            if (blockPointer % pot3Value == 0) {
                SSIDataPut(SSI1_BASE, write);
            }
        } else {
            SSIDataPut(SSI1_BASE, write);
        }
    } else {
        SSIDataPut(SSI1_BASE, write);
    }






    while (SSIBusy(SSI1_BASE)) {
    }
}

void timerSetup(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    IntMasterEnable();
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / ((44100)));
    //TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER2_BASE, TIMER_A, SysCtlClockGet() / 10);
    //TimerLoadSet(TIMER2_BASE, TIMER_A, SysCtlClockGet());
    IntEnable(INT_TIMER2A);
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

    TimerEnable(TIMER0_BASE, TIMER_A);

    TimerEnable(TIMER2_BASE, TIMER_A);
}

initTempoLed() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER3_BASE, TIMER_A, SysCtlClockGet() / tempo);
    //TimerLoadSet(TIMER2_BASE, TIMER_A, SysCtlClockGet());
    IntEnable(INT_TIMER3A);
    TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER3_BASE, TIMER_A);
}

void initSD() {
    IntMasterDisable();
    FIL configFile;
    UINT br;
    UARTprintf("Init SD\n");
    while (disk_initialize(0))
        ;
    UARTprintf("Mounting FS\n");
    f_mount(0, &wavPlayer.fso);
    UARTprintf("\n");
    int i;
    for (i = 0; i < 16; i++) {
        openAndReadHeader(i);
        wavPlayer.loop[i] = 0;
        wavPlayer.samples[i].isPlaying = 0;
    }
    f_open(&configFile, "config.txt",
            FA_OPEN_EXISTING | FA_READ);
    f_read(&configFile, &mpcConfig.hold,
            sizeof mpcConfig.hold, &br);
    f_read(&configFile, &mpcConfig.effectIndex,
            sizeof mpcConfig.effectIndex, &br);

    UARTprintf("The char is : %d\n", mpcConfig.hold[0]);
    UARTprintf("The char is : %d\n", mpcConfig.hold[1]);
    UARTprintf("The char is : %d\n", mpcConfig.hold[15]);
    UARTprintf("The effect is : %d\n", mpcConfig.effectIndex[0]);
    UARTprintf("The effect is : %d\n", mpcConfig.effectIndex[1]);
    mpcConfig.effects.fxOne = func_ptr[mpcConfig.effectIndex[0]];
    if (mpcConfig.effectIndex[1] < 5) {
        mpcConfig.effects.fxTwo = func_ptr[mpcConfig.effectIndex[1]];
    } else {
    	if(mpcConfig.effectIndex[1] == 5) {
    		isDecimator = true;
    	} else {
    		isBitwise = true;
    	}

    	mpcConfig.effects.fxTwo = emptyFunction;
        //mpcConfig.effects.fxOne = emptyFunction;
    }

    f_read(&configFile, &tempo,
            sizeof tempo, &br);

    tempo = (tempo / 60.0f);



    IntMasterEnable();
    UARTprintf("sd finished\n");

}

void biquad_gen(int type, float Fc, float Q, float Fs) {
    float32_t w0 = 2 * PI * (Fc / Fs);
    float32_t c1 = cos(w0);
    float32_t alpha = sin(w0) / (2 * Q);
    float32_t b0 = 0, b1 = 0, b2 = 0, a0 = 0, a1 = 0, a2 = 0;
    //LPF
    switch (type) {
        case 0:
            b0 = (1 - c1) / 2;
            b1 = 1 - c1;
            b2 = (1 - c1) / 2;
            a0 = 1 + alpha;
            a1 = -2 * c1;
            a2 = 1 - alpha;
            break;
        case 1:
            b0 = (1 + c1) / 2;
            b1 = -1 - c1;
            b2 = (1 + c1) / 2;
            a0 = 1 + alpha;
            a1 = -2 * c1;
            a2 = 1 - alpha;
            break;
        case 2:
            b0 = alpha;
            b1 = 0;
            b2 = -alpha;
            a0 = 1 + alpha;
            a1 = -2 * c1;
            a2 = 1 - alpha;
            break;
        case 3:
            b0 = 1;
            b1 = -2 * c1;
            b2 = 1;
            a0 = 1 + alpha;
            a1 = -2 * c1;
            a2 = 1 - alpha;
            break;

    }

    pCoeffs[0] = (b0 / a0);
    pCoeffs[1] = b1 / a0;
    pCoeffs[2] = b2 / a0;
    pCoeffs[3] = (a1 / a0) * -1;
    pCoeffs[4] = (a2 / a0) * -1;

}

void lowPassFunction(int i, int paramOne, int potValue2) {
    if (paramOne < 10) {
        return;
    }
    int biquadParam = 7936 - paramOne * 62;
    float biquadParam2 = 0.77f;
    biquad_gen(0, biquadParam, biquadParam2, 44100);
    arm_biquad_casd_df1_inst_f32 S = {1, state1, pCoeffs};
    arm_biquad_cascade_df1_f32(&S,
            &wavPlayer.samples[i].sample[loadingBlock][0],
            &wavPlayer.samples[i].sample[loadingBlock][0], 128);
}

void highPassFunction(int i, int potValue1, int potValue2) {
    if (potValue1 < 10) {
        return;
    }
    int biquadParam = potValue1 * 62;
    float biquadParam2 = 0.77;
    biquad_gen(1, biquadParam, biquadParam2, 44100);
    arm_biquad_casd_df1_inst_f32 S = {1, state2, pCoeffs};
    arm_biquad_cascade_df1_f32(&S,
            &wavPlayer.samples[i].sample[loadingBlock][0],
            &wavPlayer.samples[i].sample[loadingBlock][0], 128);


}

void bandPassFunction(int i, int potValue1, int potValue2) {
    if (potValue1 < 10) {
        return;
    }
    int biquadParam = potValue1 * 78;
    float biquadParam2 = 1.0f + potValue2 * 0.5f;
    biquad_gen(2, biquadParam, biquadParam2, 44100);
    arm_biquad_casd_df1_inst_f32 S = {1, state3, pCoeffs};
    arm_biquad_cascade_df1_f32(&S,
            &wavPlayer.samples[i].sample[loadingBlock][0],
            &wavPlayer.samples[i].sample[loadingBlock][0], 128);


}

void notchPassFunction(int i, int potValue1, int potValue2) {
    if (potValue1 < 10) {
        return;
    }
    int biquadParam = potValue1 * 10;
    float biquadParam2 = 0.1f + potValue2 * 0.01f;
    biquad_gen(3, biquadParam, biquadParam2, 44100);
    arm_biquad_casd_df1_inst_f32 S = {1, state4, pCoeffs};
    arm_biquad_cascade_df1_f32(&S,
            &wavPlayer.samples[i].sample[loadingBlock][0],
            &wavPlayer.samples[i].sample[loadingBlock][0], 128);


}

void emptyFunction(int i, int a, int b) {
    return;
}

void checkADC() {
    uint32_t ui32Value[4];
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0,
            ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1,
            ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 2,
            ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 3,
            ADC_CTL_CH9 | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 0);
    ADCProcessorTrigger(ADC0_BASE, 0);
    while (!ADCIntStatus(ADC0_BASE, 0, false)) {
    }
    //
    // Read the value from the ADC.
    //
    ADCSequenceDataGet(ADC0_BASE, 0, &ui32Value);
    //UARTprintf("The analog 1 is : %d\n", (int) ui32Value[0]);
    //ADCSequenceDataGet(ADC0_BASE, 0, &ui32Value);
    //UARTprintf("The analog 2 is : %d\n", (int) ui32Value[1]);
    //ADCSequenceDataGet(ADC0_BASE, 0, &ui32Value);
    //UARTprintf("The analog 3 is : %d\n", (int) ui32Value[2]);
    //UARTprintf("The analog 4 is : %d\n", (int) ui32Value[3]);



    float value1 = ui32Value[0];
    float value2 = ui32Value[1];
    float value3 = ui32Value[2];
    float value4 = ui32Value[3];
    value1 = value1 / 4096;
    value1 = value1 * 128;
    value2 = value2 / 4096;
    value2 = value2 * 128;
    value3 = value3 / 4096;
    value3 = value3 * 128;
    value4 = value4 / 4096;
    value4 = value4 * 128;
    pot1Value = (int) value1;
    pot2Value = (int) value2;
    pot3Value = (int) value3;
    pot4Value = (int) value4;
    //UARTprintf("The analog is : %d\n", (int) value);
    //UARTprintf("gets here\n");
}

checkSamples() {
    if (sampleWaiting) {
        if ((playingTracks[0] > -1) && (playingTracks[1] > -1)) {

            loadSample(playingTracks[0]);
            loadSample(playingTracks[1]);
            arm_add_f32(
                    &wavPlayer.samples[playingTracks[0]].sample[loadingBlock][0],
                    &wavPlayer.samples[playingTracks[1]].sample[loadingBlock][0],
                    &wavPlayer.samples[playingTracks[0]].sample[loadingBlock][0],
                    128);
            arm_scale_f32(
                    &wavPlayer.samples[playingTracks[0]].sample[loadingBlock][0],
                    0.5f,
                    &wavPlayer.samples[playingTracks[0]].sample[loadingBlock][0],
                    128);
            mpcConfig.effects.fxOne(playingTracks[0], pot1Value, pot2Value);
            mpcConfig.effects.fxTwo(playingTracks[0], pot3Value, pot4Value);
            sampleWaiting = false;
            return;
        } else if (playingTracks[0] > -1) {
            loadSample(playingTracks[0]);
            mpcConfig.effects.fxOne(playingTracks[0], pot1Value, pot2Value);
            mpcConfig.effects.fxTwo(playingTracks[0], pot3Value, pot4Value);
            sampleWaiting = false;
            return;
        } else if (playingTracks[1] > -1) {
            loadSample(playingTracks[1]);
            mpcConfig.effects.fxOne(playingTracks[1], pot1Value, pot2Value);
            mpcConfig.effects.fxTwo(playingTracks[1], pot3Value, pot4Value);
            sampleWaiting = false;
            return;
        }
    }
}

int main(void) {
    SysCtlClockSet(
            SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN
            | SYSCTL_XTAL_16MHZ);
    FPUEnable();
    FPULazyStackingEnable();
    periphSetup();
    InitConsole();
    mainUSB();
    checkSamples();
    timerSetup();
    initSD();
    initButtons();
    initTempoLed();
    UARTprintf("Start\n");
    while (1) {
        checkSamples();
    }
}

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG

void
__error__(char *pcFilename, unsigned long ulLine) {
}
#endif


