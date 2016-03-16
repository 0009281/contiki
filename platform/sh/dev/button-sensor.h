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
 * \addtogroup cc2538-smartrf-sensors
 * @{
 *
 * \defgroup cc2538dk-button-sensor cc2538dk Button Driver
 *
 * Driver for the SmartRF06EB buttons
 * @{
 *
 * \file
 * Header file for the cc2538dk Button Driver
 */
#ifndef BUTTON_SENSOR_H_
#define BUTTON_SENSOR_H_

#include "lib/sensors.h"
#include "dev/gpio.h"

#define BUTTON_SENSOR "Button"

#define BUTTON_SENSOR_VALUE_STATE    0
#define BUTTON_SENSOR_VALUE_DURATION 1

#define BUTTON_SENSOR_VALUE_RELEASED 0
#define BUTTON_SENSOR_VALUE_PRESSED  1


#define button_sensor button_select_sensor
extern const struct sensors_sensor AC_ZERO_DETECTOR_sensor;
extern const struct sensors_sensor button_onboard_sensor;
extern const struct sensors_sensor button_GPIO0_sensor;
extern const struct sensors_sensor button_GPIO1_sensor;
extern const struct sensors_sensor button_GPIO2_sensor;
/*---------------------------------------------------------------------------*/
#endif /* BUTTON_SENSOR_H_ */


int dimming_time;

/** \brief Common initialiser for all SmartRF Buttons */
void button_sensor_init();
static void GPIO_timer_callback(void *);
/**
 * @}
 * @}
 */
