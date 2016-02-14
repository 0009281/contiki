/*
 * Copyright (c) 2012, Texas Instruments Incorporated - http://www.ti.com/
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * \addtogroup cc2538dk-button-sensor
 * @{
 *
 * \file
 *  Driver for the SmartRF06EB buttons
 */
#include "contiki.h"
#include "dev/nvic.h"
#include "dev/ioc.h"
#include "dev/gpio.h"
#include "dev/button-sensor.h"
#include "sys/timer.h"

#include <stdint.h>
#include <string.h>

#define BUTTON_SELECT_PORT_BASE  GPIO_PORT_TO_BASE(BUTTON_SELECT_PORT)
#define BUTTON_SELECT_PIN_MASK   GPIO_PIN_MASK(BUTTON_SELECT_PIN)

#define BUTTON_LEFT_PORT_BASE    GPIO_PORT_TO_BASE(BUTTON_LEFT_PORT)
#define BUTTON_LEFT_PIN_MASK     GPIO_PIN_MASK(BUTTON_LEFT_PIN)

#define BUTTON_ONBOARD_PORT_BASE    GPIO_PORT_TO_BASE(BUTTON_ONBOARD_PORT)
#define BUTTON_ONBOARD_PIN_MASK     GPIO_PIN_MASK(BUTTON_ONBOARD_PIN)

#define BUTTON_RIGHT_PORT_BASE   GPIO_PORT_TO_BASE(BUTTON_RIGHT_PORT)
#define BUTTON_RIGHT_PIN_MASK    GPIO_PIN_MASK(BUTTON_RIGHT_PIN)

#define BUTTON_UP_PORT_BASE      GPIO_PORT_TO_BASE(BUTTON_UP_PORT)
#define BUTTON_UP_PIN_MASK       GPIO_PIN_MASK(BUTTON_UP_PIN)

#define BUTTON_DOWN_PORT_BASE    GPIO_PORT_TO_BASE(BUTTON_DOWN_PORT)
#define BUTTON_DOWN_PIN_MASK     GPIO_PIN_MASK(BUTTON_DOWN_PIN)
/*---------------------------------------------------------------------------*/
static struct timer debouncetimer;
static struct ctimer my_ct;
static struct rtimer simistor_strob_rtimer, zero_detector_rtimer;
static int count50=0;


struct btn_timer {
  struct timer debounce;
  clock_time_t start;
  clock_time_t duration;
};

static struct btn_timer onboard_timer;

/*---------------------------------------------------------------------------*/
/**
 * \brief Common initialiser for all buttons
 * \param port_base GPIO port's register offset
 * \param pin_mask Pin mask corresponding to the button's pin
 */
static void
config(uint32_t port_base, uint32_t pin_mask)
{
  /* Software controlled */
  GPIO_SOFTWARE_CONTROL(port_base, pin_mask);

  /* Set pin to input */
  GPIO_SET_INPUT(port_base, pin_mask);

  /* Enable edge detection */
  GPIO_DETECT_EDGE(port_base, pin_mask);

  /* Single edge */
  GPIO_TRIGGER_BOTH_EDGES(port_base, pin_mask);

  /* Trigger interrupt on Falling edge */
//  GPIO_DETECT_FALLING(port_base, pin_mask);

  GPIO_ENABLE_INTERRUPT(port_base, pin_mask);
}


/*---------------------------------------------------------------------------*/
/**
 * \brief Callback registered with the GPIO module. Gets fired with a button
 * port/pin generates an interrupt
 * \param port The port number that generated the interrupt
 * \param pin The pin number that generated the interrupt. This is the pin
 * absolute number (i.e. 0, 1, ..., 7), not a mask
 */
static void
btn_callback(uint8_t port, uint8_t pin)
{
  if(!timer_expired(&debouncetimer)) {
    return;
  }

  timer_set(&debouncetimer, CLOCK_SECOND / 8);

  if((port == BUTTON_LEFT_PORT) && (pin == BUTTON_LEFT_PIN)) {
    sensors_changed(&button_left_sensor);
  } else if((port == BUTTON_ONBOARD_PORT) && (pin == BUTTON_ONBOARD_PIN)) {
    if (GPIO_READ_PIN(GPIO_C_BASE, 0x10 ) == 0) {
	onboard_timer.start = clock_time();
	onboard_timer.duration = 0;
    }
    else {
      onboard_timer.duration = clock_time() - onboard_timer.start;
      sensors_changed(&button_onboard_sensor);
    }

  }
}

void
simistor_strob_callback(void)
{
//  rt_now = RTIMER_NOW();
//  ct = clock_time();
//  printf("Task called at %lu (clock = %lu)\n", rt_now, ct);


  GPIO_SET_PIN(GPIO_C_BASE, 0x04);
  clock_delay_usec(50);
//disable simistor
  GPIO_CLR_PIN(GPIO_C_BASE, 0x04);


//GPIO_SET_PIN(GPIO_C_BASE, 0x80);
//GPIO_CLR_PIN(GPIO_C_BASE, 0x80);

// GPIO_ENABLE_INTERRUPT(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN);
// nvic_interrupt_enable(BUTTON_LEFT_VECTOR);
}



void
 zero_detector_enable_callback(void)
{
 GPIO_ENABLE_INTERRUPT(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN);
 nvic_interrupt_enable(BUTTON_LEFT_VECTOR);

}

static void
zero_cross_callback(uint8_t port, uint8_t pin)
{
//INTERRUPTS_DISABLE();
//if (pin == BUTTON_LEFT_PIN)
//{
//  GPIO_DISABLE_INTERRUPT(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN);


//do{
//} while ((GPIO_READ_PIN(GPIO_B_BASE, 0x08)&0x08)==0x08);



//  clock_delay_usec(8000);
//  GPIO_SET_PIN(GPIO_C_BASE, 0x04);
//  clock_delay_usec(50);
//disable simistor
//  GPIO_CLR_PIN(GPIO_C_BASE, 0x04);
//  GPIO_ENABLE_INTERRUPT(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN);

//INTERRUPTS_ENABLE();
//  count50++;
//  if (count50==100) {
//    GPIO_SET_PIN(GPIO_C_BASE, 0x80);
//    count50=0;
//    ctimer_set(&my_ct, CLOCK_SECOND/128, my_rt_callback, NULL);
//    GPIO_DISABLE_INTERRUPT(BUTTON_LEFT_PORT_BASE, BUTTON_LEFT_PIN_MASK);
//    nvic_interrupt_disable(BUTTON_LEFT_VECTOR);
//    clock_delay_usec(4000);

// GPIO_ENABLE_INTERRUPT(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN);
// nvic_interrupt_enable(BUTTON_LEFT_VECTOR);


//timer if RISING detect
//    rtimer_set(&zero_detector_rtimer, RTIMER_NOW()+(RTIMER_SECOND/1000)*9, 1, zero_detector_enable_callback, NULL);


//    if (GPIO_READ_PIN(GPIO_B_BASE, 0x08)&0x08)
//        rtimer_set(&my_rt, RTIMER_NOW()+(RTIMER_SECOND/10000)*abs(95-dimming_time), 1, my_rt_callback, NULL);


        rtimer_set(&simistor_strob_rtimer, RTIMER_NOW()+(RTIMER_SECOND/10000)*abs(95-dimming_time), 1, simistor_strob_callback, NULL);
//    else 
//	 nvic_interrupt_enable(BUTTON_LEFT_VECTOR);	
//  }
//}

}



/*---------------------------------------------------------------------------*/
/**
 * \brief Init function for the left button.
 *
 * Parameters are ignored. They have been included because the prototype is
 * dictated by the core sensor api. The return value is also not required by
 * the API but otherwise ignored.
 *
 * \param type ignored
 * \param value ignored
 * \return ignored
 */
static int
config_left(int type, int value)
{
//  config(BUTTON_LEFT_PORT_BASE, BUTTON_LEFT_PIN_MASK);
      /* Software controlled */
  GPIO_SOFTWARE_CONTROL(BUTTON_LEFT_PORT_BASE, BUTTON_LEFT_PIN_MASK);

  /* Se2t pin to input */
  GPIO_SET_INPUT(BUTTON_LEFT_PORT_BASE, BUTTON_LEFT_PIN_MASK);

  /* Enable edge detection */
  GPIO_DETECT_EDGE(BUTTON_LEFT_PORT_BASE, BUTTON_LEFT_PIN_MASK);

  /* Single edge */
  GPIO_TRIGGER_SINGLE_EDGE(BUTTON_LEFT_PORT_BASE, BUTTON_LEFT_PIN_MASK);

  /* Trigger interrupt on Falling edge */
  GPIO_DETECT_FALLING(BUTTON_LEFT_PORT_BASE, BUTTON_LEFT_PIN_MASK);
//  GPIO_DETECT_RISING(BUTTON_LEFT_PORT_BASE, BUTTON_LEFT_PIN_MASK);

  GPIO_ENABLE_INTERRUPT(BUTTON_LEFT_PORT_BASE, BUTTON_LEFT_PIN_MASK);

  ioc_set_over(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN, IOC_OVERRIDE_PUE);

  nvic_interrupt_enable(BUTTON_LEFT_VECTOR);

  gpio_register_callback(zero_cross_callback, BUTTON_LEFT_PORT, BUTTON_LEFT_PIN);
  return 1;
}


static int
config_onboard(int type, int value)
{
  config(BUTTON_ONBOARD_PORT_BASE, BUTTON_ONBOARD_PIN_MASK);

  ioc_set_over(BUTTON_ONBOARD_PORT, BUTTON_ONBOARD_PIN, IOC_OVERRIDE_PUE);

  nvic_interrupt_enable(BUTTON_ONBOARD_VECTOR);

  gpio_register_callback(btn_callback, BUTTON_ONBOARD_PORT, BUTTON_ONBOARD_PIN);
  return 1;
}


static int
value_onboard(int type)
{
  if (type == BUTTON_SENSOR_VALUE_STATE) {
    return GPIO_READ_PIN(GPIO_C_BASE, 0x10 ) == 0 ?
           BUTTON_SENSOR_VALUE_PRESSED : BUTTON_SENSOR_VALUE_RELEASED;
  } else if(type == BUTTON_SENSOR_VALUE_DURATION) return (int)onboard_timer.duration;

  return 0;
}



/*---------------------------------------------------------------------------*/
void
button_sensor_init()
{
  timer_set(&debouncetimer, 0);
}
/*---------------------------------------------------------------------------*/
//SENSORS_SENSOR(button_select_sensor, BUTTON_SENSOR, NULL, config_select, NULL);
SENSORS_SENSOR(button_left_sensor, BUTTON_SENSOR, NULL, config_left, NULL);
SENSORS_SENSOR(button_onboard_sensor, BUTTON_SENSOR, value_onboard, config_onboard, NULL);
//SENSORS_SENSOR(button_right_sensor, BUTTON_SENSOR, NULL, config_right, NULL);
//SENSORS_SENSOR(button_up_sensor, BUTTON_SENSOR, NULL, config_up, NULL);
//SENSORS_SENSOR(button_down_sensor, BUTTON_SENSOR, NULL, config_down, NULL);

/** @} */
