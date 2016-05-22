/**
 * \file
 *      COAP command from server: Dimmer Up
 */

#include "rest-engine.h"
#include "sh_main.h"
#include "er-coap.h"
#include "dev/rom-util.h"
#include "cpu.h"
#include "cc2538-dev.h"


#define FIRMWARE_SIZE  65535 + 512
#define CHUNKS_TOTAL    1024*512
#define VTOR    *( uint32_t *)0xE000ED08

extern sh_dimmer_t dim_chan02;


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
  uint32_t firmware_new_base, crc_received_packet, start_firmware_vector, firmware_new_ver, firmware_new_crc;


//check if payload lenght is not zero
  if((len = REST.get_request_payload(request, (const uint8_t **)&incoming))) {

      if (VTOR == 0x200000) firmware_new_base = 0x23e000; else firmware_new_base=0x200000;
      PRINTF("block number: %d\n", coap_req->block1_num);
      PRINTF("block size: %d\n", coap_req->block1_size);
      PRINTF("preferred size: %d\n", preferred_size);
      PRINTF("len: %d\n", len);
      PRINTF("offset: %d\n", *offset);
      PRINTF("current firmware location(VTOR): %lx\n", VTOR);
      PRINTF("new firmware location: %lx\n", firmware_new_base);

//check start-up vector in the new firmware if fits in other flash part, not in the current
    if (!coap_req->block1_num) {
      start_firmware_vector = *( uint32_t *)&incoming[4];
      if ( !((start_firmware_vector > firmware_new_base) && (start_firmware_vector < (firmware_new_base + (CC2538_DEV_FLASH_SIZE>>1) - 8192)))) {
        printf("firmware_start_vetor: %lx\n", *( uint32_t *)&incoming[4]);
        PRINTF("Wrong firmware\n");
        REST.set_response_payload(response, buffer, snprintf((char *)buffer, preferred_size, "Wrong firmware"));
        REST.set_response_status(response, REST.status.NOT_ACCEPTABLE);
        return;
      }
    }

    //check if amount of inboud data is fit in the flash
    if(coap_req->block1_num * coap_req->block1_size + len <= (CC2538_DEV_FLASH_SIZE>>1)-8192) {
      //for (i=0; i< coap_req->block1_size;i++ ) PRINTF("%02x ", incoming[i]); //print block that has receied

      if (!coap_req->block1_more) {
        //do we need to clear page for last data in the coap receiving data?
        if (!((firmware_new_base + coap_req->block1_num * coap_req->block1_size) % 2048)) {
         PRINTF("Erase last packet page(2048 bytes) at address: %lx\n\r", firmware_new_base + coap_req->block1_num * coap_req->block1_size);
         INTERRUPTS_DISABLE();
         rom_util_page_erase(firmware_new_base + coap_req->block1_num * coap_req->block1_size, 2048);
         INTERRUPTS_ENABLE();
        }

        //write last block minus CRC
        INTERRUPTS_DISABLE();
        rom_util_program_flash(incoming, firmware_new_base + coap_req->block1_num * coap_req->block1_size,  len - 4);
        INTERRUPTS_ENABLE();
        //calculate CRC of the new firmware that was ceceived
        crc_received_packet = 0 - calc4summ(firmware_new_base, ((coap_req->block1_num * coap_req->block1_size + len) >> 2)-1);
        PRINTF("CRC of all data that have received: %lx\n\r", crc_received_packet);
        PRINTF("CRC that received: %lx\n\r", *( uint32_t *)&incoming[len-4]);
        //if received data is correct, then calculate new CRC, new version number and write it to the end of firmware flash
        if (crc_received_packet == *( uint32_t *)&incoming[len-4]) {
          firmware_new_ver = *( uint32_t *)(VTOR + (CC2538_DEV_FLASH_SIZE >> 1) - 8192 - 8);
          firmware_new_ver += 1;

          PRINTF("Erase flash page where version&crc is located at address: %lx\n\r", firmware_new_base + (CC2538_DEV_FLASH_SIZE>>1) - 8192 - 2048);
          INTERRUPTS_DISABLE();
          rom_util_page_erase(firmware_new_base + (CC2538_DEV_FLASH_SIZE>>1) - 8192 - 2048, 2048);
          INTERRUPTS_ENABLE();

          //write version
          INTERRUPTS_DISABLE();
          rom_util_program_flash(&firmware_new_ver, firmware_new_base + (CC2538_DEV_FLASH_SIZE>>1) - 8192 - 8,  4);
          INTERRUPTS_ENABLE();
          //calculate CRC (received through COAP firmware and other 0xFF until end of firmware flash
          firmware_new_crc = 0 - calc4summ(firmware_new_base, (((CC2538_DEV_FLASH_SIZE>>1)-8192) >> 2)-1); 
          PRINTF("ver new: %lx\n\r", firmware_new_ver);
          PRINTF("crc new: %lx\n\r", firmware_new_crc);

          INTERRUPTS_DISABLE();
          rom_util_program_flash(&firmware_new_crc, firmware_new_base + (CC2538_DEV_FLASH_SIZE>>1) -8192 -4,  4);
          INTERRUPTS_ENABLE();
          REST.set_response_payload(response, buffer, snprintf((char *)buffer, preferred_size, "Firmware updating succeed"));
          REST.set_response_status(response, REST.status.CHANGED);

        }
        else {
          REST.set_response_payload(response, buffer, snprintf((char *)buffer, preferred_size, "CRC error"));
          REST.set_response_status(response, REST.status.NOT_MODIFIED);
        }
      }
      else {
        //clear flash page
        if (!((firmware_new_base + coap_req->block1_num * coap_req->block1_size) % 2048)) {
         PRINTF("Erase page(2048 bytes) at address: %lx\n\r", firmware_new_base + coap_req->block1_num * coap_req->block1_size);
         INTERRUPTS_DISABLE();
         rom_util_page_erase(firmware_new_base + coap_req->block1_num * coap_req->block1_size, 2048);
         INTERRUPTS_ENABLE();
        }
        //program next part of the incoming data to flash memory
        INTERRUPTS_DISABLE();
        rom_util_program_flash(incoming, firmware_new_base + coap_req->block1_num * coap_req->block1_size,  coap_req->block1_size);
        INTERRUPTS_ENABLE();
        coap_set_status_code(response, CONTINUE_2_31);
      }

      coap_set_header_block1(response, coap_req->block1_num, 0, coap_req->block1_size);
    } 
    else {
      REST.set_response_status(response, REST.status.REQUEST_ENTITY_TOO_LARGE);
      REST.set_response_payload(response, buffer, snprintf((char *)buffer, preferred_size, "Size is %lx bytes max.", (CC2538_DEV_FLASH_SIZE>>1) - 8192));
      return;
    }
  } else {
    REST.set_response_payload(response, buffer, snprintf((char *)buffer, preferred_size, "NoPayload"));
    REST.set_response_status(response, REST.status.BAD_REQUEST);
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

   printf("preferred size: %d, offset: %ld", preferred_size, *offset);
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
