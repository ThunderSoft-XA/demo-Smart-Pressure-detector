#include "gpio.h"
#include "quectel_utils.h"			
#include "../../sdk/include/qapi/qapi_timer.h"		//QAPI_TIMER_UNIT_SEC
#include "../../main/log.h"		// gizLog

/*  !!! This Pin Enumeration Only Applicable BG96-OPEN Project !!!
 */
 
 GPIO_MAP_TBL gpio_map_tbl[PIN_E_GPIO_MAX] = {
/* PIN NUM,     PIN NAME,    GPIO ID  GPIO FUNC */
	{  4, 		"GPIO01",  		23, 	 0},
	{  5, 		"GPIO02",  		20, 	 0},
	{  6, 		"GPIO03",  		21, 	 0},
	{  7, 		"GPIO04",  		22, 	 0},
	{ 18, 		"GPIO05",  		11, 	 0},
	{ 19, 		"GPIO06",  		10, 	 0},
	{ 22, 		"GPIO07",  		 9, 	 0},
	{ 23, 		"GPIO08",  	 	 8, 	 0},
	{ 26, 		"GPIO09",  		15, 	 0},
	{ 27, 		"GPIO10",  		12, 	 0},
	{ 28, 		"GPIO11",  		13, 	 0},
	{ 40, 		"GPIO19",  		19, 	 0},
	{ 41, 		"GPIO20",  		18, 	 0},
	{ 64, 		"GPIO21",  		07, 	 0},
};

/* gpio id table */
qapi_GPIO_ID_t gpio_id_tbl[PIN_E_GPIO_MAX];

/* gpio tlmm config table */
qapi_TLMM_Config_t tlmm_config[PIN_E_GPIO_MAX];
	
/* Modify this pin num which you want to test */
//MODULE_PIN_ENUM  led_pin_num_motion;
//MODULE_PIN_ENUM  led_pin_num_Pulsesensor;
//MODULE_PIN_ENUM  led_pin_num_pressure;
//MODULE_PIN_ENUM  motor_pin_num ;

MODULE_PIN_ENUM led_blue;		//data status
MODULE_PIN_ENUM led_red;		//abnormal status
MODULE_PIN_ENUM led_green;		//normal status

/**************************************************************************
*                                 FUNCTION
***************************************************************************/
/*
 @func
 gpio_config_init
 @brief
 */


 void led_gpio_config()
 {
	    gizLog(LOG_INFO,"in led_gpio_config...\n"); 
	   
	    led_blue = GPIO_BLUE;	//pulsensor led	D9
	    gpio_config(led_blue, QAPI_GPIO_OUTPUT_E, QAPI_GPIO_NO_PULL_E, QAPI_GPIO_2MA_E);	
		
		led_red = GPIO_RED;		//pulsensor led	D2
	    gpio_config(led_red, QAPI_GPIO_OUTPUT_E, QAPI_GPIO_NO_PULL_E, QAPI_GPIO_2MA_E);	
		
		led_green = GPIO_GREEN;	//pulsensor led	D3
	    gpio_config(led_green, QAPI_GPIO_OUTPUT_E, QAPI_GPIO_NO_PULL_E, QAPI_GPIO_2MA_E);	
		
 }

 void led_on_off(bool on, uint8_t pin_gpio)
 {
	
	if (on)
	{
		qapi_TLMM_Drive_Gpio(gpio_id_tbl[pin_gpio], gpio_map_tbl[pin_gpio].gpio_id, QAPI_GPIO_LOW_VALUE_E);
	}
	else
	{
		qapi_TLMM_Drive_Gpio(gpio_id_tbl[pin_gpio], gpio_map_tbl[pin_gpio].gpio_id, QAPI_GPIO_HIGH_VALUE_E);
	}
 }

 #if 0
 int int_gpio_read(void)
 {
	 qapi_GPIO_Value_t level = QAPI_GPIO_HIGH_VALUE_E;

	 qapi_TLMM_Read_Gpio(gpio_id_tbl[PIN_E_GPIO], gpio_map_tbl[PIN_E_GPIO].gpio_id, &level);

	 if (level == QAPI_GPIO_HIGH_VALUE_E)
	 	return 1;
	 else if (level == QAPI_GPIO_LOW_VALUE_E)
	 	return 0;
	 else
	 	return -1;
 }
 #endif

/*
 @func
  motor_gpio_config
 @brief
 */

 #if 0
void motor_gpio_config()
{
		 gizLog(LOG_INFO,"in motor_gpio_config ...\n"); 
		 
		 motor_pin_num = PIN_E_GPIO_09;						// motor pin;
	     gpio_config(motor_pin_num, QAPI_GPIO_OUTPUT_E, QAPI_GPIO_NO_PULL_E, QAPI_GPIO_2MA_E);	
}
#endif

/*
@func
  motorSetValue
@brief
  [in]  gpiovalue
  		gpiovalue: 0x00 
*/


void set_gpio_value(uint8_t gpiovalue, MODULE_PIN_ENUM m_pin)		//different pin (led or motor)
{
	int i = 5;
	gizLog(LOG_INFO,"in set_gpio_value.....\n"); 
	qapi_Status_t status = QAPI_OK;
	if(gpiovalue == 0x00)
	{
		while(i)
		{
			status = qapi_TLMM_Drive_Gpio(gpio_id_tbl[m_pin], gpio_map_tbl[m_pin].gpio_id, QAPI_GPIO_LOW_VALUE_E);
			gizLog(LOG_INFO,"set %d QAPI_GPIO_LOW_VALUE_E status = %d\n",m_pin, status);
			qapi_Timer_Sleep(50, QAPI_TIMER_UNIT_MSEC, true);
			
			status = qapi_TLMM_Drive_Gpio(gpio_id_tbl[m_pin], gpio_map_tbl[m_pin].gpio_id, QAPI_GPIO_HIGH_VALUE_E);
			gizLog(LOG_INFO,"set %d QAPI_GPIO_HIGH_VALUE_E status = %d\n",m_pin, status);
			qapi_Timer_Sleep(50, QAPI_TIMER_UNIT_MSEC, true);
			i--;
		}
	}
	else
	{
		gizLog(LOG_INFO,"Invalid value...\n");
	}
}



/*
@func
  gpio_config
@brief
  [in]  m_pin
  		MODULE_PIN_ENUM type; the GPIO pin which customer want used for operation;
  [in]  gpio_dir
  		qapi_GPIO_Direction_t type; GPIO pin direction.
  [in]  gpio_pull
  		qapi_GPIO_Pull_t type; GPIO pin pull type.
  [in]  gpio_drive
  		qapi_GPIO_Drive_t type; GPIO pin drive strength. 
*/

void gpio_config(MODULE_PIN_ENUM m_pin,qapi_GPIO_Direction_t gpio_dir,qapi_GPIO_Pull_t gpio_pull,qapi_GPIO_Drive_t gpio_drive)
{
	gizLog(LOG_INFO,"in gpio config.....\n"); 
	
	qapi_Status_t status = QAPI_OK;

	tlmm_config[m_pin].pin   = gpio_map_tbl[m_pin].gpio_id;
	tlmm_config[m_pin].func  = gpio_map_tbl[m_pin].gpio_func;
	tlmm_config[m_pin].dir   = gpio_dir;
	tlmm_config[m_pin].pull  = gpio_pull;
	tlmm_config[m_pin].drive = gpio_drive;

	// the default here
	status = qapi_TLMM_Get_Gpio_ID(&tlmm_config[m_pin], &gpio_id_tbl[m_pin]);
	//IOT_DEBUG(LOG_INFO, "gpio_id[%d], status = %d", gpio_map_tbl[m_pin].gpio_id, status);
	
	gizLog(LOG_INFO,"pin_num = %d, gpio_id[%d], status = %d ...\n",gpio_map_tbl[m_pin].pin_num, gpio_map_tbl[m_pin].gpio_id, status); 
	
	if (status == QAPI_OK)
	{
		status = qapi_TLMM_Config_Gpio(gpio_id_tbl[m_pin], &tlmm_config[m_pin]);
		//IOT_DEBUG("[GPIO] gpio_id[%d] status = %d", gpio_map_tbl[m_pin].gpio_id, status);
		gizLog(LOG_INFO,"after qapi_TLMM_Config_Gpio, status = %d ...\n", status); 
		
		if (status != QAPI_OK)
		{
			//IOT_DEBUG("[GPIO] gpio_config failed");
			gizLog(LOG_INFO,"gpio config failed.....\n"); 
		}
//		status = qapi_TLMM_Drive_Gpio(gpio_id_tbl[led_pin_num_pressure], gpio_map_tbl[led_pin_num_pressure].gpio_id, QAPI_GPIO_LOW_VALUE_E);	(low work for motor)
//		qapi_Timer_Sleep(5, QAPI_TIMER_UNIT_SEC, true);
		
		status = qapi_TLMM_Drive_Gpio(gpio_id_tbl[m_pin], gpio_map_tbl[m_pin].gpio_id, QAPI_GPIO_HIGH_VALUE_E);
//		qapi_Timer_Sleep(5, QAPI_TIMER_UNIT_SEC, true);
	}
}
