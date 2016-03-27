/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
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
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      Example resource
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 */

#include "contiki.h"

#if PLATFORM_HAS_LEDS

#include <string.h>
#include "contiki.h"
#include "rest-engine.h"
#include "dev/leds.h"
#include "sh_main.h"

#include "er-coap.h"
#include "er-plugtest.h"

//extern unsigned char dimmer_command;
static uint8_t validate_etag[8] = { 0 };
static uint8_t validate_etag_len = 1;
static uint8_t validate_change = 1;
    
static uint8_t bytes[8] = {0};
static size_t len = 0;

static void res_post_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);


extern sh_dimmer_t dim_chan0;

//static void res_post_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
//static void res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_dimmer_set_light,
        "title=\"LEDs: ?color=r|g|b, POST/PUT mode=on|off\";rt=\"Control\"",
         NULL,
         NULL,
         res_post_put_handler,
         NULL);

static void
res_post_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  size_t len = 0;
  const char *light = NULL;
  const char *time = NULL;
  uint8_t led = 0;
  int success = 1;


  if (REST.get_post_variable(request, "light", &light)) {
   PRINTF("light value: %u\r\n", atoi(light));
   dim_chan0.light_to_set = atoi(light);
   if (REST.get_post_variable(request, "time", &time)) {
    PRINTF("time value: %u\r\n", atoi(time));
    dim_chan0.Tconst = atoi(time);
   }
  }
  else {
    REST.set_response_status(response, REST.status.BAD_REQUEST);
  }

  dim_chan0.command = DIMMER_SET_LIGHT;
  process_post(&dimmer_process, PROCESS_EVENT_CONTINUE, &dim_chan0);
}



#endif /* PLATFORM_HAS_LEDS */
