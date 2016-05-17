/**
 * \file
 *      COAP command from server: Dimmer Up
 */

#include "rest-engine.h"
#include "sh_main.h"
#include "er-coap.h"
#include "dev/rom-util.h"
#include "cpu.h"


#define MAX_PLUGFEST_PAYLOAD 64 + 1       /* +1 for the terminating zero, which is not transmitted */
#define FIRMWARE_SIZE  65535 + 512
#define CHUNKS_TOTAL    1024*512

extern sh_dimmer_t dim_chan02;
static int32_t large_update_size = 0;
static uint8_t large_update_store[4096] = { 0 };
static struct etimer  firmware_erase_timer;

//static unsigned int large_update_ct = APPLICATION_OCTET_STREAM;


uint32_t calc4summ(uint32_t *ptr, uint16_t count) {
  uint32_t res = 0;

  while(count > 0) {
    res = res + ptr[count - 1];
    count = count -1;
  }

  return res;
}



static void res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
static void res_post_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_dimmer_firmware,
         "title=\"Update firmware\";rt=\"Control\"",
         res_get_handler,
         res_post_handler,
         res_put_handler,
         NULL);


static void
res_post_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{


PRINTF("calc4summ: %x\n\r", calc4summ(0x23e000, (FIRMWARE_SIZE >> 2)-1));


}
static void
res_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  coap_packet_t *const coap_req = (coap_packet_t *)request;
  uint8_t *incoming = NULL;
  size_t len = 0;
  uint16_t i;
  unsigned int ct = -1;
//preferred_size, *offset only for GEt method!!!


/*
  if(!REST.get_header_content_type(request, &ct)) {
    REST.set_response_status(response, REST.status.BAD_REQUEST);
    const char *error_msg = "NoContentType";
    REST.set_response_payload(response, error_msg, strlen(error_msg));
    return;
  }
*/

  if((len = REST.get_request_payload(request, (const uint8_t **)&incoming))) {

      printf("block number: %d\n", coap_req->block1_num);
      printf("block size: %d\n", coap_req->block1_size);
      printf("preferred size: %d\n", preferred_size);
      printf("len: %d\n", len);
      printf("offset: %d\n", *offset);
/*    if((coap_req->block1_num >= 10)) {
      printf("Start flash erasing!\n");
//      etimer_set(&firmware_erase_timer, CLOCK_SECOND/100);
//      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&firmware_erase_timer));
//      while (!etimer_expired(&firmware_erase_timer));
      coap_set_status_code(response, CONTINUE_2_31);
      coap_set_header_block1(response, coap_req->block1_num, 1, coap_req->block1_size);
//      REST.set_response_payload(response, buffer, snprintf((char *)buffer, MAX_PLUGFEST_PAYLOAD, "Wait 0 %d\n",0));
      return;
    }
*/
    if(coap_req->block1_num * coap_req->block1_size + len <= FIRMWARE_SIZE) {
//      memcpy(large_update_store + coap_req->block1_num * coap_req->block1_size,        incoming, len);
//      large_update_size = coap_req->block1_num * coap_req->block1_size + len;
//      large_update_ct = ct;
      INTERRUPTS_DISABLE();
      rom_util_program_flash(incoming, 0x23e000 + coap_req->block1_num * coap_req->block1_size,  coap_req->block1_size);
      INTERRUPTS_ENABLE();
//      for (i=0; i< coap_req->block1_size;i++ ) PRINTF("%02x ", incoming[i]);

      if (!coap_req->block1_more) REST.set_response_status(response, REST.status.CHANGED);
        else        coap_set_status_code(response, CONTINUE_2_31);

      coap_set_header_block1(response, coap_req->block1_num, 0, coap_req->block1_size);
    } else {
      REST.set_response_status(response,
                               REST.status.REQUEST_ENTITY_TOO_LARGE);
      REST.set_response_payload(
        response,
        buffer,
        snprintf((char *)buffer, MAX_PLUGFEST_PAYLOAD, "%uB max.",
                 sizeof(large_update_store)));
      return;
    }
  } else {
    REST.set_response_status(response, REST.status.BAD_REQUEST);
    const char *error_msg = "NoPayload";
    REST.set_response_payload(response, error_msg, strlen(error_msg));
    return;
  }
}

static void
res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  int32_t strpos = 0;

  /* Check the offset for boundaries of the resource data. */
  if(*offset >= CHUNKS_TOTAL) {
    REST.set_response_status(response, REST.status.BAD_OPTION);
    /* A block error message should not exceed the minimum block size (16). */

    const char *error_msg = "BlockOutOfScope";
    REST.set_response_payload(response, error_msg, strlen(error_msg));
    return;
  }

  /* Generate data until reaching CHUNKS_TOTAL. */
//  while(strpos < preferred_size) {
//    strpos += snprintf((char *)buffer + strpos, preferred_size - strpos + 1, "|%ld|", *offset);
//  }

   printf("preferred size: %d, offset: %d", preferred_size, *offset);
   memcpy(buffer, 0x200000 + (*offset), preferred_size);
   strpos = preferred_size;

  /* snprintf() does not adjust return value if truncated by size. */
  if(strpos > preferred_size) {
    strpos = preferred_size;
    /* Truncate if above CHUNKS_TOTAL bytes. */
  }
  if(*offset + (int32_t)strpos > CHUNKS_TOTAL) {
    strpos = CHUNKS_TOTAL - *offset;
  }
  REST.set_response_payload(response, buffer, strpos);

  /* IMPORTANT for chunk-wise resources: Signal chunk awareness to REST engine. */
  *offset += strpos;

  /* Signal end of resource representation. */
  if(*offset >= CHUNKS_TOTAL) {
    *offset = -1;
  }
}
