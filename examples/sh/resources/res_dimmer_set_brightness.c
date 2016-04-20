/**
 * \file
 *      COAP command from server: Set Brightness Level
 */

#include "rest-engine.h"
#include "sh_main.h"


static void res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

extern sh_dimmer_t dim_chan0;

RESOURCE(res_dimmer_set_brightness,
        "title=\"Set Brightness: brightness=0..100&time=0..60\";rt=\"Control\"",
         NULL,
         NULL,
         res_put_handler,
         NULL);

static void
res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  const char *brightness = NULL;
  const char *time = NULL;

  if (REST.get_post_variable(request, "brightness", &brightness) && atoi(brightness) <=100 && atoi(brightness) >=0) {
    PRINTF("Set Brightness value: %u\r\n", atoi(brightness));
    dim_chan0.brightness_to_set = atoi(brightness);
    if (REST.get_post_variable(request, "time", &time)) {
      PRINTF("Set Brightness time value: %u\r\n", atoi(time));
      dim_chan0.Tconst = atoi(time);
    }
    else  dim_chan0.Tconst = dim_chan0.config_fram.Tconst;
    dim_chan0.command = DIMMER_SET_BRIGHTNESS;
    process_post(&dimmer_process, PROCESS_EVENT_CONTINUE, &dim_chan0);
  }
  else {
    REST.set_response_status(response, REST.status.BAD_REQUEST);
  }


}
