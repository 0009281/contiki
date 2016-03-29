#include <stdlib.h>


#define DIMMER_OFF 0xC0
#define DIMMER_ON 0xC1
#define DIMMER_TOGGLE 0xC2
#define DIMMER_CYCLIC_DIMMING 0xC3
#define DIMMER_CYCLIC_DIMMING_STOP 0xC4
#define DIMMER_SET_BRIGHTNESS 0xC5
#define DIMMER_ENABLED 1
#define DIMMER_DISABLED 0
#define DIMMER_UP 1
#define DIMMER_DOWN 0


#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x]", ((u
#define PRINTLLADDR(lladdr) PRINTF("[%02x:%02x:%02x:%02x:%02x:%02x]", (lladdr)->addr[0], (lladdr)->addr[1], (lla
#else
#define PRINTF(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif



extern unsigned char dimmer_command, dimmer_Lmax;
extern uint32_t dimmer_current_state;


/* Global configuration */
typedef struct  {
//  uint8_t dimmer_command;
  uint8_t Lmin;
  uint8_t Lmax;
  uint8_t Tconst;
  uint8_t last_state;
  uint8_t controller_mode;
  uint8_t network_mode;
  uint8_t network_timeout;
  uint8_t button_type;
  uint8_t remote_controller_address[12]; //ipv6 address

  uint8_t dimmer_current_state;
  uint8_t dimmer_command;
} sh_dimmer_config_fram_t;


typedef struct {
//  uint8_t dimmer_command;
  sh_dimmer_config_fram_t config_fram;

  uint8_t Lmin;
  uint8_t Lmax;
  uint8_t Tconst;
  uint8_t last_state;
  uint8_t controller_mode;
  uint8_t network_mode;
  uint8_t network_timeout;
  uint8_t button_type;
  uint8_t remote_controller_address[12]; //ipv6 address

  uint8_t current_state;
  uint8_t direction;
  uint8_t command;
  uint8_t current_light;
  uint8_t brightness_to_set;
  uint8_t thyristor_open_time;
  struct rtimer thyristor_rtimer;

} sh_dimmer_t;



PROCESS_NAME(dimmer_process);