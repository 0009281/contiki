/**
 * \file
 *      COAP command from server: Dimmer Down
 */

#include "rest-engine.h"
#include "sh_main.h"

extern sh_dimmer_t dim_chan0;

static void res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_dimmer_down_start,
         "title=\"Down\";rt=\"Control\"",
         NULL,
         NULL,
         res_put_handler,
         NULL);

static void
res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  const char *time = NULL;

  PRINTF("Dimmer Down\r\n");
  dim_chan0.Tconst = 5;
  dim_chan0.command = DIMMER_DOWN;
  process_post(&dimmer_process, PROCESS_EVENT_CONTINUE, &dim_chan0);
}
