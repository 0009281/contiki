/**
 * \file
 *      COAP command from server: Start Cyclic Dimming
 */

#include "rest-engine.h"
#include "sh_main.h"

extern sh_dimmer_t dim_chan0;
static void res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_dimmer_cyclic_dimming,
         "title=\"Start Cyclic Dimming\";rt=\"Control\"",
         NULL,
         NULL,
         res_put_handler,
         NULL);

static void
res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  PRINTF("Start Dimmer Cyclic\r\n");
  dim_chan0.command = DIMMER_CYCLIC_DIMMING;
  process_post(&dimmer_process, PROCESS_EVENT_CONTINUE, &dim_chan0);
}
