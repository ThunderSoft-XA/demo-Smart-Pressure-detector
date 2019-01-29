/**
************************************************************
* @file         main.c
* @brief        main
* @author       Gizwits
* @date         2018-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#include "gagent_soc.h"
#include "../Gizwits/gizwits_product.h"
#include "log.h"
#include "../driver/gpio/gpio.h"
#include "../driver/adc/fsr402_pressure_adc.h"


#define QUERY_INTERVAL 2 //S

extern MODULE_PIN_ENUM led_green;
extern MODULE_PIN_ENUM led_red;

TX_TIMER *userTimer;

//led port
void led_init()
{
	   gizLog(LOG_INFO,"in led init...\n"); 
	   led_gpio_config();
	   led_on_off(false,led_green);		//init status is off
}

void ICACHE_FLASH_ATTR userTimerCB(void)
{
	gizLog(LOG_INFO,"in userTimerCB.....\n"); 
	int16_t quality;

	quality = fsr402_pressure_adc_read();	
	if(quality >= 1000/*g*/)
	{
		led_on_off(true,led_green);
		qapi_Timer_Sleep(100, QAPI_TIMER_UNIT_MSEC, true);
		led_on_off(false, led_green);
	}else if(quality == -1){
		led_on_off(true,led_red);
		qapi_Timer_Sleep(100, QAPI_TIMER_UNIT_MSEC, true);
		led_on_off(false, led_red);
	}
 
}

void user_init(void)
{
    int32 ret = -1;
    
    gizLog(LOG_INFO,"Sensor initialization ...\n"); 

	led_init();							//led init
    txm_module_object_allocate(&userTimer, sizeof(TX_TIMER));
    ret = tx_timer_create(userTimer, "userTimer", userTimerCB, NULL, 1,
                         20, TX_AUTO_ACTIVATE);			
    if(ret != TX_SUCCESS)
    {
        gizLog(LOG_WARNING,"Failed to create UserTimer.\n");
    }
}


void gagentMain( void )
{
    getFreeHeap();
    user_init();
}
