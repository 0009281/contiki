/**
 * \file
 *      COAP command from server: Dimmer On
 */

#include "rest-engine.h"
#include "sh_main.h"
#include "dev/rom-util.h"


static void res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_sh_reboot,
         "title=\"Reboot\";rt=\"Control\"",
         NULL,
         NULL,
         res_put_handler,
         NULL);


static void
res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{

        rom_util_reset_device();
}
