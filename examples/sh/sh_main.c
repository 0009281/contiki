
/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "contiki-net.h"
#include "sys/clock.h"
#include "sys/rtimer.h"
#include "dev/leds.h"
#include "dev/ioc.h"
#include "dev/als-sensor.h"
#include "dev/cc2538-sensors.h"
#include <math.h>

#include <stdio.h> /* For printf() */

#include "cpu.h"
#include "button-sensor.h"

#include "dev/rom-util.h"

#include "rest-engine.h"

#include "sh_main.h"

static struct etimer  dimmer_timer;
static uint32_t current_sensor_iteration,  current_sensor_value_max, pwr;
static uint64_t pwr_sum;
static double current_sensor_value, temp_current_sensor_value;
//extern int dimming_time;


unsigned char dimmer_command, dimmer_Lmax=30;
uint32_t dimmer_current_state=0;


extern resource_t  res_toggle, res_dimmer_toggle, res_dimmer_cyclic_dimming, res_dimmer_cyclic_dimming_stop, res_dimmer_on,  res_dimmer_off,
                   res_dimmer_set_brightness, res_dimmer_get_status;

sh_dimmer_t dim_chan0;


rtimer_clock_t rt_now, rt_for;
//static clock_time_t ct;

//#define FILENAME "test"

//static struct etimer et;

//static struct rtimer my_rt2;

//static struct rtimer rt;
//rtimer_clock_t rt_now, rt_for;
//static clock_time_t ct;

/*
void my_callback2(void)
{
 
GPIO_SET_PIN(GPIO_C_BASE, 0x80);
}

*/

/*
void
rt_callback(struct rtimer *t, void *ptr)
{
  rt_now = RTIMER_NOW();
8  ct = clock_time();
  printf("Task called at %lu (clock = %lu)\n", rt_now, ct);
GPIO_SET_PIN(GPIO_C_BASE, 0x80);
}
*/

/*---------------------------------------------------------------------------*/
PROCESS(dimmer_process, "Dimming process");
PROCESS(btn_process, "Button/Gpio process");
PROCESS(current_sensor_process, "Current sensor process");
PROCESS(er_example_server, "Erbium Example Server");
AUTOSTART_PROCESSES(&dimmer_process, &btn_process, &er_example_server);

void fram_write(void *data, uint16_t size, uint32_t address)
{
//  rom_util_page_erase(address, 2048);
//  rom_util_program_flash(data, address, size);
}

void fram_read(void *data, uint16_t size, uint32_t address)
{
//  rom_util_memcpy(data, address, size);
}

/*
PRCOESS_THREAD(dimmer_thyristor_control, ev, daya(sh_dimmer_t *dim)
{

 do {
//  leds_on(LEDS_GREEN);

//  etimer_set(&dimmer_timer, CLOCK_SECOND*dim->Tconst/(dim->Lmax-dim->Lmin));
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&dimmer_timer));
  etimer_reset(&dimmer_timer);
//  leds_off(LEDS_GREEN);
  dimming_time++;

 } while (dimming_time<=dim->Lmax);


 return 0;
}

*/


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(dimmer_process, ev, data)
{
static sh_dimmer_t *dim;

  PROCESS_BEGIN();




//  PROCESS_YIELD();

  printf("Beginning of Dimming process\n");

//do {

/*
  GPIO_SOFTWARE_CONTROL(GPIO_B_NUM,0x08 );
*/
  GPIO_CLR_PIN(GPIO_C_BASE, 0x4);
  GPIO_SET_OUTPUT(GPIO_C_BASE, 0x4);

  GPIO_SET_INPUT(GPIO_A_BASE, 0x40);
  ioc_set_over(GPIO_A_BASE, 6, IOC_OVERRIDE_PUE);

//  GPIO_SET_INPUT(GPIO_C_BASE, 0x10); //button



/*  GPIO_SET_INPUT(GPIO_B_BASE, 0x08);

*/


//  GPIO_SET_OUTPUT(GPIO_B_BASE, 0x20);
//  GPIO_CLR_PIN(GPIO_B_BASE, 0x20);
//rtimer_init();
//rtimer_set(&my_rt2, RTIMER_NOW() + RTIMER_SECOND*5, 1, (void *)my_callback2, NULL);
//rtimer_set(&rt, RTIMER_NOW()+RTIMER_SECOND*10, 1, rt_callback, NULL);

//    leds_on(LEDS_GREEN);
//    for (i=0; i<40;i++)  clock_delay_usec(50000);
//    leds_off(LEDS_GREEN);

//INTERRUPTS_DISABLE();8

//do {

//  PROCESS_YIELD();

//if    ((GPIO_READ_PIN(GPIO_C_BASE, 0x10)&0x10)==0x0)  {btn_pressed=1;}
/*
if (btn_pressed==1) {
    GPIO_SET_PIN(GPIO_C_BASE, 0x40);

    etimer_set(&et, CLOCK_SECOND/8);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    etimer_reset(&et);

    GPIO_CLR_PIN(GPIO_C_BASE, 0x40);

    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    etimer_reset(&et);

*/

/*
if   ( (GPIO_READ_PIN(GPIO_C_BASE, 0x10)&0x10)==0x0   ) 
{

 GPIO_SET_PIN(GPIO_C_BASE, 0x84);

//for (i=0; i<100;i++)  
clock_delay_usec(10000);
 GPIO_CLR_PIN(GPIO_C_BASE, 0x84);
//for (i=0; i<100;i++)  
clock_delay_usec(10000);
// clock_delay_usec(10000);
}
else   GPIO_SET_PIN(GPIO_C_BASE, 0x84);
*/




/*
//if (btn_pressed==1) {
//clock_delay_usec(65535);
  GPIO_SET_PIN(GPIO_C_BASE, 0x84);
//  GPIO_CLR_PIN(GPIO_C_BASE, 0x40);
//  GPIO_SET_PIN(GPIO_B_BASE, 0x20);
//for (i=0; i<20;i++) 
clock_delay_usec(2000);

//  GPIO_SET_PIN(GPIO_C_BASE, 0x40);
  GPIO_CLR_PIN(GPIO_C_BASE, 0x84);
//  GPIO_CLR_PIN(GPIO_B_BASE, 0x20);
//for (i=0; i<20;i++) 
clock_delay_usec(18000);
//}
*/

/*
//if button pressed
if   ( (GPIO_READ_PIN(GPIO_C_BASE, 0x10)&0x10)==0x0   )
{
//if 1 on the zero detector
if   ( (GPIO_READ_PIN(GPIO_B_BASE, 0x08)&0x08)==0x08   )
{
//  clock_delay_usec(1500);
//red led ON
//  GPIO_SET_PIN(GPIO_C_BASE, 0x80);
  clock_delay_usec(3000);

//wait 1-0 on the zero detector
do{
} while ((GPIO_READ_PIN(GPIO_B_BASE, 0x08)&0x08)==0x08);

//red led OFF
//  GPIO_CLR_PIN(GPIO_C_BASE, 0x80);


//green led OFF
//GPIO_CLR_PIN(GPIO_C_BASE, 0x40);

 clock_delay_usec(4500);
//do
//{
//enable simistor
  GPIO_SET_PIN(GPIO_C_BASE, 0x04);
  clock_delay_usec(50);
//disable simistor
  GPIO_CLR_PIN(GPIO_C_BASE, 0x04);
//} while ((GPIO_READ_PIN(GPIO_B_BASE, 0x08)&0x08)==0x0);

//green led ON
//GPIO_SET_PIN(GPIO_C_BASE, 0x40);
}

}
else
{

//disable simistor
// GPIO_CLR_PIN(GPIO_C_BASE, 0x04);
//green led OFF
//GPIO_CLR_PIN(GPIO_C_BASE, 0x40);
//red led OFF
//  GPIO_CLR_PIN(GPIO_C_BASE, 0x80);

//}


}

*/

//rom_util_memcpy(&dim_chan0, 0x27F000, sizeof(dim_chan0));
fram_read(&dim_chan0, sizeof(dim_chan0), 0x27F000);
//if (dim_chan0.current_state) {dim_chan0.thyristor_open_time = 30;}

   PRINTF("Dimmer current state=%x\r\n", dim_chan0.current_state);
dim_chan0.command = 0;
dim_chan0.thyristor_open_time = 30;
dim_chan0.Lmin = 0;
dim_chan0.Lmax = 30;
dim_chan0.Tconst = 0;
dim_chan0.current_light=30;
dim_chan0.current_state = DIMMER_ENABLED;

//    dimming_time=0;
//    dimmer_current_state=0;
//dimmer_Lmax=30;
//direction=1;
  while(1) {

   PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);
   dim = data;
   PRINTF("Dimmer process has awoken, DIMMER_COMMAND=%x\r\n", dim->command);
   PRINTF("Dimmer current state=%x\r\n", dim->current_state);
   if (dim->command==DIMMER_TOGGLE) {
    if (!dim->current_state) {
     dim->command=DIMMER_ON;
    }
    else if (dim->current_state) {
     dim->command=DIMMER_OFF;
    }
   } //DIMMER_TOGGLE


   if (dim->command==DIMMER_CYCLIC_DIMMING)
	{

//	    leds_on(LEDS_ALL);	    
            etimer_set(&dimmer_timer, CLOCK_SECOND/10);
	    do {
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&dimmer_timer));
//	     PRINTF("Dimmer direction: %u\n\r", dim->direction);
//	     PRINTF("Dimmer current state: %u\n\r", dim->current_state);
//	     PRINTF("Dimmer thyristor open timr: %u\n\r", dim->thyristor_open_time);
    	    if (dim->current_state)	dim->direction=DIMMER_DOWN; else dim->direction=DIMMER_UP;
	    if (dim->direction==DIMMER_UP) dim->thyristor_open_time++; else dim->thyristor_open_time--;

	    if (dim->thyristor_open_time==0) dim->current_state=DIMMER_DISABLED;
	    else if (dim->thyristor_open_time==100) dim->current_state=DIMMER_ENABLED;

            etimer_reset(&dimmer_timer);
	    } while (dim->command!=DIMMER_CYCLIC_DIMMING_STOP);

	    dim->Lmax = dim->thyristor_open_time;
	    if (dim->thyristor_open_time) dim->current_state=DIMMER_ENABLED;
             else dim->current_state=DIMMER_DISABLED;
	    printf("Dimmer value after cyclic dimming: %u\n\r",dim->Lmax);
	    dim->command = 0;
	}

    else if ((dim->command==DIMMER_ON) && (dim->current_state != DIMMER_ENABLED)) {
     printf("Toggle Command On!\n\r");
     PRINTF("Switch on dimmer\n\r");
     PRINTF("Dimmer open time: %u\n\r", dim->thyristor_open_time);
     PRINTF("Dimmer Lmax: %u\n\r", dim->Lmax);
       etimer_set(&dimmer_timer, CLOCK_SECOND*dim->Tconst/(dim->Lmax-dim->Lmin));
     do {
       PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&dimmer_timer));
       dim->thyristor_open_time++;
       etimer_reset(&dimmer_timer);
     } while (dim->thyristor_open_time < dim->Lmax);

     dim->current_light=dim->Lmax;
     dim->current_state = DIMMER_ENABLED;
     dim->command = 0;
     fram_write(dim, sizeof(*dim), 0x27F000);
    }
    else if ((dim->command==DIMMER_OFF) && (dim->current_state != DIMMER_DISABLED)) {
     PRINTF("Switch off dimmer\n\r");
     PRINTF("Dimmer open time: %u\n\r", dim->thyristor_open_time);
     PRINTF("Dimmer Lmin: %u\n\r", dim->Lmin);
     
      etimer_set(&dimmer_timer, CLOCK_SECOND*dim->Tconst/(dim->Lmax-dim->Lmin));
     do {
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&dimmer_timer));
      dim->thyristor_open_time--;
      etimer_reset(&dimmer_timer);
     } while (dim->thyristor_open_time > dim->Lmin);

     dim->current_light=dim->Lmin;
     dim->current_state = DIMMER_DISABLED;
     dim->command = 0;     
     fram_write(dim, sizeof(*dim), 0x27F000);
    }
    else if ((dim->command==DIMMER_SET_BRIGHTNESS) && (dim->brightness_to_set != dim->current_light)) {
     PRINTF("Dimmer Set Light!\n\r");
     PRINTF("Dimmer open time: %u\n\r", dim->thyristor_open_time);
     PRINTF("Dimmer Light To Set: %u\n\r", dim->brightness_to_set);
     
      etimer_set(&dimmer_timer, CLOCK_SECOND*dim->Tconst/(dim->Lmax-dim->Lmin));
     do {
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&dimmer_timer));
      if (dim->brightness_to_set > dim->current_light) dim->thyristor_open_time++;
       else dim->thyristor_open_time--;

      etimer_reset(&dimmer_timer);
     } while (dim->thyristor_open_time != dim->brightness_to_set);

     dim->current_light=dim->brightness_to_set;
     dim->Lmax=dim->brightness_to_set;
     dim->current_state = DIMMER_ENABLED;
     dim->command = 0;
    }
    else      dim->command = 0;

         printf("!!!!!!!!!!!!!!!!!\n\r");




/*
    		leds_on(LEDS_GREEN);
	for (i=0;i<20;i++) 
	{
                etimer_set(&dimmer_timer, CLOCK_SECOND/10);
                PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&dimmer_timer));
                etimer_reset(&dimmer_timer);

	    if (direction==1) dimming_time++;
            if (direction==2) dimming_time--;
	}
///	    if (dimming_time>=20) direction=2;
//	    if (dimming_time==0) direction=1;
	    if (direction==1)	direction=2; else direction=1;
    		leds_off(LEDS_GREEN);
*/
	} //while(1)
//  }



//PROCESS_WAIT_EVENT();

//} while(1);
//  REG((GPIO_C_BASE | GPIO_DATA) + (0x80<<2) ) = 0xFF;
  
  PROCESS_END();
}



PROCESS_THREAD(current_sensor_process, ev, data)
{
//  uint32_t pwr;

  PROCESS_BEGIN();

  while(1) {

    PROCESS_YIELD();


    if(ev == PROCESS_EVENT_TIMER) {

    pwr=(unsigned)als_sensor.value(0);
//    pwr += 6;
//    pwr = pwr & 0x0FFFFFFF;
    if (pwr<1018) pwr = 1018 + (1018-pwr);
//    pwr_sum += (pwr-1022)*(pwr-1022);
    pwr_sum += pwr;

    if (pwr>current_sensor_value_max)  current_sensor_value_max=pwr;
	//current_sensor_value =  ((3.3/2048)*pwr-1.65)/0.275; // 2048=11bit ADC, 1.65= Allegro zero, 0.275=sensivity V/A=3.3/12A
        //current_sensor_value = current_sensor_value + ((3.3/2048)*pwr-1.65)*((3.3/2048)*pwr-1.65); // 2048=11bit ADC, 1.65= Allegro zero, 0.275=sensivity V/A=3.3/12A
//	current_sensor_value = current_sensor_value + ((pwr-1020)*(1/170.7))*((pwr-1020)*(1/170.7)); // 2048=11bit ADC, 1.65= Allegro zero, 0.275=sensivity V/A=3.3/12A
        current_sensor_iteration++;

	if (current_sensor_iteration==4096) {
	    temp_current_sensor_value=sqrt(abs((pwr_sum/4096) - 1020)*0.005859375);

          printf("Current sensor v2alue = %ld.%03u A \n\r",  (long)(temp_current_sensor_value), (unsigned)((temp_current_sensor_value-floor(temp_current_sensor_value))*1000));
//          printf("Current sensor value = %ld.%03u A \n\r", /*current_sensor_value/4096*/ (long)sqrt(current_sensor_value/4096), (unsigned)sqrt(((current_sensor_value/4096-floor(current_sensor_value/4096))*1000)));
//          printf("Current sensor value = %ld.%03u A \n\r", /*current_sensor_value/4096*/ (long)current_sensor_value/1000, (unsigned)((current_sensor_value/1000-floor(current_sensor_value/1000))*1000));
          printf("Max Iout = %d V\n\r, Average V = %d\n\r", current_sensor_value_max, pwr_sum/4096);
//          printf("Temperature = %d mC\n",  cc2538_temp_sensor.value(CC2538_SENSORS_VALUE_TYPE_CONVERTED));
	    current_sensor_iteration=0;
	    current_sensor_value=0;
	    current_sensor_value_max=0;
	    pwr_sum=0;
	}
    


    }



  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(btn_process, ev, data)
{

 static uint32_t pwr;

 PROCESS_BEGIN();

 printf("Beginning of Button process\n");

//pwr=0;

 while(1) {
  PROCESS_YIELD();

  if(ev == sensors_event) {

   if(data == &button_onboard_sensor) {
    PRINTF("Onboard button: Pin %d, press duration %d clock ticks\n\r", (&button_onboard_sensor)->value(BUTTON_SENSOR_VALUE_STATE), (&button_onboard_sensor)->value(BUTTON_SENSOR_VALUE_DURATION));
    if((&button_onboard_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) < CLOCK_SECOND*0.7) {
     PRINTF("Onboard button short press\n\r");
     PRINTF("Dimmer command=%x\r\n", dim_chan0.command);
     if (!dim_chan0.command) {
      dim_chan0.command = DIMMER_TOGGLE;
      process_post(&dimmer_process, PROCESS_EVENT_CONTINUE, &dim_chan0);
     }
     else if (dim_chan0.command==DIMMER_CYCLIC_DIMMING) {
      dim_chan0.command=DIMMER_CYCLIC_DIMMING_STOP;
     }
    }
    else if(((&button_onboard_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) > CLOCK_SECOND*0.7) && ((&button_onboard_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) <CLOCK_SECOND*3)) {
     printf("Onboard button middle button press!\n\r");
//     dim_chan0.thyristor_open_time--;
     dim_chan0.Tconst++;
//     PRINTF("Dimmer open time: %u\n\r", dim_chan0.thyristor_open_time);
     PRINTF("Dimmer Tconst: %u\n\r", dim_chan0.Tconst);

    }
    else if((&button_onboard_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) > CLOCK_SECOND*3)  {
     printf("Onboard button long button press!\n\r");
     dim_chan0.command = DIMMER_CYCLIC_DIMMING;
     process_post(&dimmer_process, PROCESS_EVENT_CONTINUE, &dim_chan0);
    }
   } //button_onboard_sensor


   else if(data == &button_GPIO0_sensor) {
    printf("GPIO0 button: Pin = %d, press duration %d clock ticks\n\r", (&button_GPIO0_sensor)->value(BUTTON_SENSOR_VALUE_STATE), (&button_GPIO0_sensor)->value(BUTTON_SENSOR_VALUE_DURATION));

    if((&button_GPIO0_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) < CLOCK_SECOND*0.7) {
     printf("GPIO0 button short press\n\r");
    }

    else if(((&button_GPIO0_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) > CLOCK_SECOND*0.7) && ((&button_GPIO0_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) <CLOCK_SECOND*3)) {
     printf("GPIO0 button middle button press!\n\r");
    }

    else if((&button_GPIO0_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) > CLOCK_SECOND*3) {
     printf("GPIO0 button long button press!\n\r");
    }
   } //button_GPIO0_sensor


   else if(data == &button_GPIO1_sensor) {
    printf("GPIO1 button: Pin = %d, press duration %d clock ticks\n\r", (&button_GPIO1_sensor)->value(BUTTON_SENSOR_VALUE_STATE), (&button_GPIO1_sensor)->value(BUTTON_SENSOR_VALUE_DURATION));

    if((&button_GPIO1_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) < CLOCK_SECOND*0.7) {
     printf("GPIO1 button short press\n\r");
    }

    else if(((&button_GPIO1_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) > CLOCK_SECOND*0.7) && ((&button_GPIO1_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) <CLOCK_SECOND*3)) {
     printf("GPIO1 button middle button press!\n\r");
    }

    else if((&button_GPIO1_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) > CLOCK_SECOND*3) {
     printf("GPIO1 button long button press!\n\r");
    }
   } //button_GPIO1_sensor


   else if(data == &button_GPIO2_sensor) {
    printf("GPIO2 button: Pin = %d, press duration %d clock ticks\n\r", (&button_GPIO2_sensor)->value(BUTTON_SENSOR_VALUE_STATE), (&button_GPIO2_sensor)->value(BUTTON_SENSOR_VALUE_DURATION));

    if((&button_GPIO2_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) < CLOCK_SECOND*0.7) {
     printf("GPIO2 button short press\n\r");
    }

    else if(((&button_GPIO2_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) > CLOCK_SECOND*0.7) && ((&button_GPIO2_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) <CLOCK_SECOND*3)) {
     printf("GPIO2 button middle button press!\n\r");
    }

    else if((&button_GPIO2_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) > CLOCK_SECOND*3) {
     printf("GPIO2 button long button press!\n\r");
    }
   } //button_GPIO2_sensor




    } //sensor_event
  } //while

  PROCESS_END();

}

/*---------------------------------------------------------------------------*/


PROCESS_THREAD(er_example_server, ev, data)
{
  PROCESS_BEGIN();

  PROCESS_PAUSE();

  PRINTF("Starting Erbium Example Server\n");

#ifdef RF_CHANNEL
  PRINTF("RF channel: %u\n", RF_CHANNEL);
#endif
#ifdef IEEE802154_PANID
  PRINTF("PAN ID: 0x%04X\n", IEEE802154_PANID);
#endif

  PRINTF("uIP buffer: %u\n", UIP_BUFSIZE);
  PRINTF("LL header: %u\n", UIP_LLH_LEN);
  PRINTF("IP+UDP header: %u\n", UIP_IPUDPH_LEN);
  PRINTF("REST max chunk: %u\n", REST_MAX_CHUNK_SIZE);

  /* Initialize the REST engine. */
  rest_init_engine();

  /*
   * Bind the resources to their Uri-Path.
   * WARNING: Activating twice only means alternate path, not two instances!
   * All static variables are the same for each URI path.
   */
  rest_activate_resource(&res_dimmer_toggle, "dimmer/toggle");
  rest_activate_resource(&res_dimmer_cyclic_dimming, "dimmer/cyclic_dimming");
  rest_activate_resource(&res_dimmer_cyclic_dimming_stop, "dimmer/cyclic_dimming_stop");
  rest_activate_resource(&res_dimmer_on, "dimmer/on");
  rest_activate_resource(&res_dimmer_off, "dimmer/off");
  rest_activate_resource(&res_dimmer_set_brightness, "dimmer/set_brightness");
  rest_activate_resource(&res_dimmer_get_status, "dimmer/status");

  /* Define application-specific events here. */
  while(1) {
    PROCESS_WAIT_EVENT();
  }                             /* while (1) */

  PROCESS_END();
}
