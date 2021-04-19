#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>

#include "leds.h"
#include "spi_comm.h"
#include "motors.h"
#include "selector.h"
#include "sensors/proximity.h"

#include "sensors/VL53L0X/VL53L0X.h"
#include "epuck1x/uart/e_uart_char.h"
#include "serial_comm.h"

#include "chprintf.h"
#include "usbcfg.h"

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

//moving forward
void fw(int s){
	left_motor_set_speed(s);
	right_motor_set_speed(s);
}

//moving backward
void bw(int s){
	left_motor_set_speed(-s);
	right_motor_set_speed(-s);
}

//turning
void trn(int a, int b){
	left_motor_set_speed(a);
	right_motor_set_speed(b);
}

// stop
void st(void){
	left_motor_set_speed(0);
	right_motor_set_speed(0);
}



int main(void)
{
	messagebus_init(&bus, &bus_lock, &bus_condvar);
    halInit();
    chSysInit();

    serial_start();
    usb_start();

    mpu_init();
    clear_leds();

    spi_comm_start();
    motors_init();

    proximity_start();
    calibrate_ir();

    /* Infinite loop. */
    while (1) {

    	// Proximity sensor reading
    	if (SDU1.config->usbp->state == USB_ACTIVE){
    	   chprintf((BaseSequentialStream *)&SDU1, "-----------------------\n");
    	}
    	if (SDU1.config->usbp->state == USB_ACTIVE){
    		chprintf((BaseSequentialStream *)&SDU1, "sensor 0 : %4d\n",get_prox(0));
    	}
    	if (SDU1.config->usbp->state == USB_ACTIVE){
    		chprintf((BaseSequentialStream *)&SDU1, "sensor 1 : %4d\n",get_prox(1));
    	}
    	if (SDU1.config->usbp->state == USB_ACTIVE){
    	    chprintf((BaseSequentialStream *)&SDU1, "sensor 2 : %4d\n",get_prox(2));
    	}
    	if (SDU1.config->usbp->state == USB_ACTIVE){
    	    chprintf((BaseSequentialStream *)&SDU1, "sensor 3 : %4d\n",get_prox(3));
    	}
    	if (SDU1.config->usbp->state == USB_ACTIVE){
    	    chprintf((BaseSequentialStream *)&SDU1, "sensor 4 : %4d\n",get_prox(4));
    	}
    	if (SDU1.config->usbp->state == USB_ACTIVE){
    	    chprintf((BaseSequentialStream *)&SDU1, "sensor 5 : %4d\n",get_prox(5));
    	}
    	if (SDU1.config->usbp->state == USB_ACTIVE){
    	    chprintf((BaseSequentialStream *)&SDU1, "sensor 6 : %4d\n",get_prox(6));
      	}
    	if (SDU1.config->usbp->state == USB_ACTIVE){
    	    chprintf((BaseSequentialStream *)&SDU1, "sensor 7 : %4d\n",get_prox(7));
    	}
    	chThdSleepMilliseconds(50);

    	//movement according to sensor

    	fw(0); // move forward

    	if (get_prox(0) >= 250 && get_prox(7) >= 250){
    		st();
    		chThdSleepMilliseconds(100);
    		bw(500);
    		if (get_prox(0) == 50 && get_prox(7) == 50){ // out of range of the prox
    			st();
    		}
    	}
    }
}



#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
