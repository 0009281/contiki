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

#include "sh_main.h"


#define AC_ZERO_DETECTOR_PORT_BASE    GPIO_PORT_TO_BASE(AC_ZERO_DETECTOR_PORT)
#define AC_ZERO_DETECTOR_PIN_MASK     GPIO_PIN_MASK(AC_ZERO_DETECTOR_PIN)


#define BUTTON_ONBOARD_PORT_BASE    GPIO_PORT_TO_BASE(BUTTON_ONBOARD_PORT)
#define BUTTON_ONBOARD_PIN_MASK     GPIO_PIN_MASK(BUTTON_ONBOARD_PIN)

#define BUTTON_GPIO0_PORT_BASE    GPIO_PORT_TO_BASE(BUTTON_GPIO0_PORT)
#define BUTTON_GPIO0_PIN_MASK     GPIO_PIN_MASK(BUTTON_GPIO0_PIN)

#define BUTTON_GPIO1_PORT_BASE    GPIO_PORT_TO_BASE(BUTTON_GPIO1_PORT)
#define BUTTON_GPIO1_PIN_MASK     GPIO_PIN_MASK(BUTTON_GPIO1_PIN)

#define BUTTON_GPIO2_PORT_BASE    GPIO_PORT_TO_BASE(BUTTON_GPIO2_PORT)
#define BUTTON_GPIO2_PIN_MASK     GPIO_PIN_MASK(BUTTON_GPIO2_PIN)

/*---------------------------------------------------------------------------*/
static struct timer debouncetimer;
static struct ctimer GPIO_callback_timer;
static struct rtimer simistor_strob_rtimer, zero_detector_rtimer;
static int count50=0;


struct btn_timer {
  uint8_t status;
  struct timer debounce;
  clock_time_t start;
  clock_time_t duration;
};

static struct btn_timer onboard_timer, GPIO0_timer, GPIO1_timer, GPIO2_timer;

extern sh_dimmer_t dim_chan0;


/*---------------------------------------------------------------------------*/
/**
 * \brief Common initialiser for all buttons
 * \param port_base GPIO port's register offset
 * \param pin_mask Pin mask corresponding to the button's pin
 */
static void
config(uint32_t port_base, uint32_t pin_mask)
{
  GPIO_SOFTWARE_CONTROL(port_base, pin_mask);
  GPIO_SET_INPUT(port_base, pin_mask);
  GPIO_DETECT_EDGE(port_base, pin_mask);
  GPIO_TRIGGER_BOTH_EDGES(port_base, pin_mask);
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

// if(!timer_expired(&debouncetimer)) {
//  return;
// }
 if(!timer_expired(&GPIO_callback_timer)) {
  return;
 }


// timer_set(&debouncetimer, CLOCK_SECOND / 16);


 if((port == BUTTON_ONBOARD_PORT) && (pin == BUTTON_ONBOARD_PIN)) 
  ctimer_set(&GPIO_callback_timer, CLOCK_SECOND/8, GPIO_timer_callback, BUTTON_ONBOARD_PIN_MASK);
 else if((port == BUTTON_GPIO0_PORT) && (pin == BUTTON_GPIO0_PIN)) 
  ctimer_set(&GPIO_callback_timer, CLOCK_SECOND/8, GPIO_timer_callback, BUTTON_GPIO0_PIN_MASK);
 else if((port == BUTTON_GPIO1_PORT) && (pin == BUTTON_GPIO1_PIN))
  ctimer_set(&GPIO_callback_timer, CLOCK_SECOND/8, GPIO_timer_callback, BUTTON_GPIO1_PIN_MASK);
 else if((port == BUTTON_GPIO2_PORT) && (pin == BUTTON_GPIO2_PIN)) {
  ctimer_set(&GPIO_callback_timer, CLOCK_SECOND/8, GPIO_timer_callback, BUTTON_GPIO2_PIN_MASK);

/*
  if (GPIO_READ_PIN(GPIO_D_BASE, 0x4 ) == 0) {
   leds_on(LEDS_RED);
   clock_delay_usec(1000);
   leds_off(LEDS_RED);
   GPIO2_timer.start = clock_time();
   GPIO2_timer.duration = 0;
   GPIO2_timer.status = 0xAA;
  }
  else if (GPIO2_timer.status == 0xAA)  {
   leds_on(LEDS_GREEN);
   clock_delay_usec(1000);
   leds_off(LEDS_GREEN);
   GPIO2_timer.duration = clock_time() - GPIO2_timer.start;
   sensors_changed(&button_GPIO2_sensor);
   GPIO2_timer.status = 0x0;
  }
*/







 }

}


static void
GPIO_timer_callback(void *n)
{
static uint8_t GPIO_handle;

 GPIO_handle = n;
 switch (GPIO_handle) {

 case 0x10:
  if (GPIO_READ_PIN(GPIO_C_BASE, GPIO_handle ) == 0) {
   onboard_timer.start = clock_time();
   onboard_timer.duration = 0;
   onboard_timer.status = 0xAA;
  }
  else if (onboard_timer.status==0xAA) {
   onboard_timer.duration = clock_time() - onboard_timer.start;
   sensors_changed(&button_onboard_sensor);
   onboard_timer.status = 0;
  }
  break;
 case 0x1:
  if (GPIO_READ_PIN(GPIO_D_BASE, GPIO_handle ) == 0) {
   GPIO1_timer.start = clock_time();
   GPIO1_timer.duration = 0;
   GPIO1_timer.status = 0xAA;
  }
  else {
   GPIO1_timer.duration = clock_time() - GPIO1_timer.start;
   sensors_changed(&button_GPIO1_sensor);
   GPIO1_timer.status = 0x0;
  }
  break;
 case 0x2:
  if (GPIO_READ_PIN(GPIO_D_BASE, GPIO_handle ) == 0) {
   GPIO0_timer.start = clock_time();
   GPIO0_timer.duration = 0;
   GPIO0_timer.status = 0xAA;
  }
  else {
   GPIO0_timer.duration = clock_time() - GPIO0_timer.start;
   sensors_changed(&button_GPIO0_sensor);
   GPIO0_timer.status = 0x0;
  }
  break;

 case 0x4:
  if (GPIO_READ_PIN(GPIO_D_BASE, GPIO_handle ) == 0) {
   leds_on(LEDS_RED);
   clock_delay_usec(1000);
   leds_off(LEDS_RED);
   GPIO2_timer.start = clock_time();
   GPIO2_timer.duration = 0;
   GPIO2_timer.status = 0xAA;
  }
  else if (GPIO2_timer.status==0xAA){
   leds_on(LEDS_GREEN);
   clock_delay_usec(1000);
   leds_off(LEDS_GREEN);
   GPIO2_timer.duration = clock_time() - GPIO2_timer.start;
   sensors_changed(&button_GPIO2_sensor);
   GPIO2_timer.status = 0x0;
  }
  break;


  }
}


void
simistor_strob_callback(void)
{
//  rt_now = RTIMER_NOW();
//  ct = clock_time();
//  printf("Task called at %lu (clock = %lu)\n", rt_now, ct);


  GPIO_SET_PIN(GPIO_C_BASE, LAMP_CHAN0);
  clock_delay_usec(50);
//disable simistor
  GPIO_CLR_PIN(GPIO_C_BASE, LAMP_CHAN0);


//GPIO_SET_PIN(GPIO_C_BASE, 0x80);
//GPIO_CLR_PIN(GPIO_C_BASE, 0x80);

// GPIO_ENABLE_INTERRUPT(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN);
// nvic_interrupt_enable(BUTTON_LEFT_VECTOR);
}

/*
void
 zero_detector_enable_callback(void)
{
 GPIO_ENABLE_INTERRUPT(AC_ZERO_DETECTOR_PORT, AC_ZERO_DETECTOR_PIN);
 nvic_interrupt_enable(AC_ZERO_DETECTOR_VECTOR);
}
*/

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


//    rtimer_set(&simistor_strob_rtimer, RTIMER_NOW()+(RTIMER_SECOND/10000)*abs(95-dimming_time), 1, simistor_strob_callback, NULL);
 if (dim_chan0.thyristor_open_time)
//  rtimer_set(&dim_chan0.thyristor_rtimer, RTIMER_NOW()+(RTIMER_SECOND/10000)*(255 - dim_chan0.thyristor_open_time)/2.68, 1, simistor_strob_callback, NULL);
  rtimer_set(&dim_chan0.thyristor_rtimer, RTIMER_NOW()+(RTIMER_SECOND/10000)*(100 - dim_chan0.thyristor_open_time)/1.02, 1, simistor_strob_callback, NULL);

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
config_ac_zero_detector(int type, int value)
{
//  config(BUTTON_LEFT_PORT_BASE, BUTTON_LEFT_PIN_MASK);
      /* Software controlled */
  GPIO_SOFTWARE_CONTROL(AC_ZERO_DETECTOR_PORT_BASE, AC_ZERO_DETECTOR_PIN_MASK);

  /* Se2t pin to input */
  GPIO_SET_INPUT(AC_ZERO_DETECTOR_PORT_BASE, AC_ZERO_DETECTOR_PIN_MASK);

  /* Enable edge detection */
  GPIO_DETECT_EDGE(AC_ZERO_DETECTOR_PORT_BASE, AC_ZERO_DETECTOR_PIN_MASK);

  /* Single edge */
  GPIO_TRIGGER_SINGLE_EDGE(AC_ZERO_DETECTOR_PORT_BASE, AC_ZERO_DETECTOR_PIN_MASK);

  /* Trigger interrupt on Falling edge */
  GPIO_DETECT_FALLING(AC_ZERO_DETECTOR_PORT_BASE, AC_ZERO_DETECTOR_PIN_MASK);
//We can't use RISING detect, becuse while symithtor is open detector will always return 1=like we have
//zero crossing, so we can use FALLING edge only.

//  GPIO_DETECT_RISING(AC_ZERO_DETECTOR_PORT_BASE, AC_ZERO_DETECTOR_PIN_MASK);

  GPIO_ENABLE_INTERRUPT(AC_ZERO_DETECTOR_PORT_BASE, AC_ZERO_DETECTOR_PIN_MASK);

  ioc_set_over(AC_ZERO_DETECTOR_PORT, AC_ZERO_DETECTOR_PIN, IOC_OVERRIDE_PUE);

  nvic_interrupt_enable(AC_ZERO_DETECTOR_VECTOR);
  gpio_register_callback(zero_cross_callback, AC_ZERO_DETECTOR_PORT, AC_ZERO_DETECTOR_PIN);
  return 1;
}

/*---------------------------------------------------------------------------*/
static int
config_onboard(int type, int value)
{
  config(BUTTON_ONBOARD_PORT_BASE, BUTTON_ONBOARD_PIN_MASK);
  ioc_set_over(BUTTON_ONBOARD_PORT, BUTTON_ONBOARD_PIN, IOC_OVERRIDE_PUE);
  nvic_interrupt_enable(BUTTON_ONBOARD_VECTOR);
  gpio_register_callback(btn_callback, BUTTON_ONBOARD_PORT, BUTTON_ONBOARD_PIN);
  return 1;
}
/*---------------------------------------------------------------------------*/
static int
config_GPIO0(int type, int value)
{
  config(BUTTON_GPIO0_PORT_BASE, BUTTON_GPIO0_PIN_MASK);
  ioc_set_over(BUTTON_GPIO0_PORT, BUTTON_GPIO0_PIN, IOC_OVERRIDE_PUE);
  nvic_interrupt_enable(BUTTON_GPIO0_VECTOR);
  gpio_register_callback(btn_callback, BUTTON_GPIO0_PORT, BUTTON_GPIO0_PIN);
  return 1;
}
/*---------------------------------------------------------------------------*/
static int
config_GPIO1(int type, int value)
{
  config(BUTTON_GPIO1_PORT_BASE, BUTTON_GPIO1_PIN_MASK);
  ioc_set_over(BUTTON_GPIO1_PORT, BUTTON_GPIO1_PIN, IOC_OVERRIDE_PUE);
  nvic_interrupt_enable(BUTTON_GPIO1_VECTOR);
  gpio_register_callback(btn_callback, BUTTON_GPIO1_PORT, BUTTON_GPIO1_PIN);
  return 1;
}
/*---------------------------------------------------------------------------*/
static int
config_GPIO2(int type, int value)
{
  config(BUTTON_GPIO2_PORT_BASE, BUTTON_GPIO2_PIN_MASK);
  ioc_set_over(BUTTON_GPIO2_PORT, BUTTON_GPIO2_PIN, IOC_OVERRIDE_PUE);
  nvic_interrupt_enable(BUTTON_GPIO2_VECTOR);
  gpio_register_callback(btn_callback, BUTTON_GPIO2_PORT, BUTTON_GPIO2_PIN);
  return 1;
}

/*---------------------------------------------------------------------------*/
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
static int
value_GPIO0(int type)
{
  if (type == BUTTON_SENSOR_VALUE_STATE) {
    return GPIO_READ_PIN(GPIO_D_BASE, 0x2 ) == 0 ?
           BUTTON_SENSOR_VALUE_PRESSED : BUTTON_SENSOR_VALUE_RELEASED;
  } else if(type == BUTTON_SENSOR_VALUE_DURATION) return (int)GPIO0_timer.duration;

  return 0;
}
/*---------------------------------------------------------------------------*/
static int
value_GPIO1(int type)
{
  if (type == BUTTON_SENSOR_VALUE_STATE) {
    return GPIO_READ_PIN(GPIO_D_BASE, 0x1 ) == 0 ?
           BUTTON_SENSOR_VALUE_PRESSED : BUTTON_SENSOR_VALUE_RELEASED;
  } else if(type == BUTTON_SENSOR_VALUE_DURATION) return (int)GPIO1_timer.duration;

  return 0;
}
/*---------------------------------------------------------------------------*/
static int
value_GPIO2(int type)
{
  if (type == BUTTON_SENSOR_VALUE_STATE) {
    return GPIO_READ_PIN(GPIO_D_BASE, 0x4 ) == 0 ?
           BUTTON_SENSOR_VALUE_PRESSED : BUTTON_SENSOR_VALUE_RELEASED;
  } else if(type == BUTTON_SENSOR_VALUE_DURATION) return (int)GPIO2_timer.duration;

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
SENSORS_SENSOR(AC_ZERO_DETECTOR_sensor, BUTTON_SENSOR, NULL, config_ac_zero_detector, NULL);
SENSORS_SENSOR(button_onboard_sensor, BUTTON_SENSOR, value_onboard, config_onboard, NULL);
SENSORS_SENSOR(button_GPIO0_sensor, BUTTON_SENSOR, value_GPIO0, config_GPIO0, NULL);
SENSORS_SENSOR(button_GPIO1_sensor, BUTTON_SENSOR, value_GPIO1, config_GPIO1, NULL);
SENSORS_SENSOR(button_GPIO2_sensor, BUTTON_SENSOR, value_GPIO2, config_GPIO2, NULL);
//SENSORS_SENSOR(button_up_sensor, BUTTON_SENSOR, NULL, config_up, NULL);
//SENSORS_SENSOR(button_down_sensor, BUTTON_SENSOR, NULL, config_down, NULL);

/** @} */
