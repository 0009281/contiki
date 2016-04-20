/**
 * \file
 *      COAP command from server: Dimmer On
 */

#include "rest-engine.h"
#include "sh_main.h"

extern sh_dimmer_t dim_chan0;

static void res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_dimmer_on,
         "title=\"On: time=0..60\";rt=\"Control\"",
         NULL,
         NULL,
         res_put_handler,
         NULL);

static void
res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  const char *time = NULL;

  PRINTF("Dimmer On\r\n");
  if (REST.get_post_variable(request, "time", &time)) {
    PRINTF("Dimmer On time value: %u\r\n", atoi(time));
    dim_chan0.Tconst = atoi(time);
  }
  else  dim_chan0.Tconst = dim_chan0.config_fram.Tconst;

  dim_chan0.command = DIMMER_ON;
  process_post(&dimmer_process, PROCESS_EVENT_CONTINUE, &dim_chan0);
}
