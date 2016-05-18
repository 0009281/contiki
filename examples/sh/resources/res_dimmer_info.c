/**
 * \file
 *      COAP command from server: Dimmer Return Status
 */

#include "rest-engine.h"
#include "sh_main.h"
#include "dev/rom-util.h"
#include "cpu.h"

extern const char device_name;
extern const char device_firmware;

static char EnableFirmwareUpdate = 0;



static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
static void res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_dimmer_get_info,
         "title=\"Return Information\";rt=\"Control\"",
         res_get_handler,
         NULL,
         res_put_handler,
         NULL);



static void
res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  unsigned int accept = -1;

  REST.get_header_accept(request, &accept);
  if(accept == -1 || accept == REST.type.TEXT_PLAIN) {
    REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
    snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "Device Type=%s&Firmware version=%s", device_name, device_firmware);

    REST.set_response_payload(response, (uint8_t *)buffer, strlen((char *)buffer));
  } else if(accept == REST.type.APPLICATION_JSON) {
    REST.set_header_content_type(response, REST.type.APPLICATION_JSON);
    snprintf((char *)buffer, REST_MAX_CHUNK_SIZE, "{'light':%d}", dimmer_Lmax);

    REST.set_response_payload(response, buffer, strlen((char *)buffer));
  } else {
    REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
    const char *msg = "Supporting content-types text/plain and application/json";
    REST.set_response_payload(response, msg, strlen(msg));
  }
}


static void
res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  size_t len = 0;
  const char *color = NULL;
  const char *mode = NULL;
  uint8_t led = 0;
  int success = 1;
  uint32_t firmware_base;
#define VTOR    *( uint32_t *)0xE000ED08

   if(len = REST.get_post_variable(request, "EnableFirmwareUpdate", &mode)) {
     PRINTF("mode %s\n", mode);
     if (atoi(mode) == 1) {
       PRINTF("Erase flash...\n");
       if (VTOR == 0x200000) firmware_base = 0x23e000;
         else firmware_base=0x200000;

       watchdog_periodic();
       INTERRUPTS_DISABLE();
       rom_util_page_erase(firmware_base, 124*2048);
       INTERRUPTS_ENABLE();
       EnableFirmwareUpdate = 0;
       REST.set_response_payload(response, buffer, snprintf((char *)buffer, preferred_size, "EnableFirmwareUpdate=1"));
     }

   }
}
