
/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
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

static struct etimer et, dimmer_timer, ett;
static unsigned char dimmer_command, dimmer_current_light, dimmer_Lmin=0, dimmer_Lmax=30, dimmer_Tconst=1;
static uint32_t dimmer_current_state=0, current_sensor_iteration,  current_sensor_value_max, pwr;
static uint64_t pwr_sum;
static double current_sensor_value, temp_current_sensor_value;
//extern int dimming_time;
#define DIMMER_TOGGLE 0xC2
#define DIMMER_CYCLE_DIMMING 0xC3
#define DIMMER_CYCLE_DIMMING_STOP 0xC4
#define DIMMER_ENABLED 1
#define DIMMER_DISABLED 0
#define DIMMER_UP 1
#define DIMMER_DOWN 0

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
  ct = clock_time();
  printf("Task called at %lu (clock = %lu)\n", rt_now, ct);
GPIO_SET_PIN(GPIO_C_BASE, 0x80);
}
*/

/*---------------------------------------------------------------------------*/
PROCESS(dimmer_process, "Dimming process");
PROCESS(btn_process, "Button/Gpio process");
PROCESS(current_sensor_process, "Current sensor process");
AUTOSTART_PROCESSES(&dimmer_process, &btn_process);



/*---------------------------------------------------------------------------*/
PROCESS_THREAD(dimmer_process, ev, data)
{
int i;
int btn_pressed=0;
static int direction;
uint32_t tt = 0x55AABBCC;

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

rom_util_memcpy(&dimmer_current_state, 0x27F000,4);
if (dimmer_current_state) { dimming_time=30; dimmer_current_state=1;}


//    dimming_time=0;
//    dimmer_current_state=0;
//dimmer_Lmax=30;
//direction=1;
  while(1) {

	PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE);
    
	if (dimmer_command==DIMMER_TOGGLE)
	{
	  printf("Toggle command received!\n\r");
	    if (dimmer_current_state==DIMMER_DISABLED)
	    {
		  GPIO_SET_OUTPUT(GPIO_C_BASE, 0x4);
		  printf("Enable dimmer!\n\r");
		do
		{
    		leds_on(LEDS_GREEN);
                etimer_set(&dimmer_timer, CLOCK_SECOND*dimmer_Tconst/(dimmer_Lmax-dimmer_Lmin));
                PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&dimmer_timer));
                etimer_reset(&dimmer_timer);
    		leds_off(LEDS_GREEN);
		dimming_time++;
		} while (dimming_time<=dimmer_Lmax);
		
//		dimming_time=80;

		dimmer_current_light=dimmer_Lmax;
		dimmer_current_state = DIMMER_ENABLED;
		rom_util_page_erase( 0x27F000, 2048);
		rom_util_program_flash(&dimmer_current_state, 0x27F000, 4);


	    }
	    else if (dimmer_current_state==DIMMER_ENABLED)
	    {
		  printf("Disable dimmer!\n\r");
		do
		{
                etimer_set(&dimmer_timer, CLOCK_SECOND*dimmer_Tconst/(dimmer_Lmax-dimmer_Lmin));
                PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&dimmer_timer));
                etimer_reset(&dimmer_timer);
		dimming_time--;
		} while (dimming_time>dimmer_Lmin);
//		  GPIO_SET_INPUT(GPIO_C_BASE, 0x4);    
		dimmer_current_light=dimmer_Lmin;
		dimmer_current_state = DIMMER_DISABLED;
		rom_util_page_erase( 0x27F000, 2048);
		rom_util_program_flash(&dimmer_current_state, 0x27F000, 4);

	    }
	}
	else if (dimmer_command==DIMMER_CYCLE_DIMMING)
	{

	    leds_on(LEDS_ALL);	    
	    do {
    	    if (dimmer_current_state==DIMMER_ENABLED)	direction=DIMMER_DOWN; else direction=DIMMER_UP;
	    if (direction==DIMMER_UP) dimming_time++; else dimming_time--;

	    if (dimming_time==0) dimmer_current_state=DIMMER_DISABLED;
	    else if (dimming_time==100) dimmer_current_state=DIMMER_ENABLED;

            etimer_set(&dimmer_timer, CLOCK_SECOND/10);
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&dimmer_timer));
            etimer_reset(&dimmer_timer);
	    } while (dimmer_command!=DIMMER_CYCLE_DIMMING_STOP);
	    dimmer_Lmax = dimming_time;
	    dimmer_current_state = DIMMER_ENABLED;
	    printf("Dimmer value after cycle dimming: %u\n\r",dimmer_Lmax);
	}

	    dimmer_command=0;





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

    etimer_set(&ett, CLOCK_SECOND/4096);
//    etimer_set(&ett, CLOCK_SECOND/100);
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
 uint16_t j;

 PROCESS_BEGIN();

 printf("Beginning of Button process\n");

//pwr=0;

 while(1) {
  PROCESS_YIELD();

  if(ev == sensors_event) {

   if(data == &button_onboard_sensor) {
    printf("Onboard button: Pin %d, press duration %d clock ticks\n\r", (&button_onboard_sensor)->value(BUTTON_SENSOR_VALUE_STATE), (&button_onboard_sensor)->value(BUTTON_SENSOR_VALUE_DURATION));
    if((&button_onboard_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) < CLOCK_SECOND*0.7) {
     printf("Onboard button short press\n\r");
     if (!dimmer_command) {
      dimmer_command = DIMMER_TOGGLE;
      process_post(&dimmer_process, PROCESS_EVENT_CONTINUE, NULL);
     }
     else if (dimmer_command==DIMMER_CYCLE_DIMMING) {
      dimmer_command=DIMMER_CYCLE_DIMMING_STOP;
     }
    }
    else if(((&button_onboard_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) > CLOCK_SECOND*0.7) && ((&button_onboard_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) <CLOCK_SECOND*3)) {
     printf("Onboard button middle button press!\n\r");
    }
    else if((&button_onboard_sensor)->value(BUTTON_SENSOR_VALUE_DURATION) > CLOCK_SECOND*3)  {
     printf("Onboard button long button press!\n\r");
     dimmer_command = DIMMER_CYCLE_DIMMING;
     process_post(&dimmer_process, PROCESS_EVENT_CONTINUE, NULL);
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
