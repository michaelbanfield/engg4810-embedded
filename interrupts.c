/*
 * interrupts.c
 *
 *  Created on: 07/04/2013
 *      Author: Michael
 */

#include "interrupts.h"




//void keyPressHandler(void) {
	//UARTprintf("herro\n");

//	int intVal;
	//intVal = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4);
//	if(GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4)) {
//		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);
//		intVal = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4);
//		if(!intVal) {
//			UARTprintf("One\n");
//		}
		//printf("The value is :%d\n", intVal);
		//if(!intVal) {
			//UARTprintf("is highhh: %d\n", intVal);
//			GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 1);
		//}
		//UARTprintf("is highhh\n");
//	}
//}


void keyPressHandler1(void) {
	GPIOPinIntClear(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	//if(GPIOPinRead(GPIO_PORTD_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3))) {
	//	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);


	//}
	int number = 0;
	number += 10 * (GPIOPinRead(GPIO_PORTD_BASE, (GPIO_PIN_0)) > 0);
	number += 7 * (GPIOPinRead(GPIO_PORTD_BASE, (GPIO_PIN_1)) > 0);
	number += 4 * (GPIOPinRead(GPIO_PORTD_BASE, (GPIO_PIN_2)) > 0);
	number += (GPIOPinRead(GPIO_PORTD_BASE, (GPIO_PIN_3)) > 0);
	if(number) {

		UARTprintf("The key is %d\n", number);
	}

}




void initButtons1(void) {

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPortIntRegister(GPIO_PORTD_BASE, keyPressHandler1);




	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4));
	GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_BOTH_EDGES);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4, 0xFF);
	//GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_, 1);

	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
	UARTprintf("One: %d\n", GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0));
	UARTprintf("One: %d\n", GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1));


	GPIOPinIntEnable(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	UARTprintf("yeahhmateee\n");


	//GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_4, GPIO_RISING_EDGE);


	//iVal = GPIOPinRead(GPIO_PORTD_BASE,
	 //       (GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 |
	  //      GPIO_PIN_4 | GPIO_PIN_5));







}






