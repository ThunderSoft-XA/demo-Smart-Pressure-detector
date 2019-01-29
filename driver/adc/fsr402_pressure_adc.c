/******************************************************************************
*@file    fsr402_pressure_adc.c
*@brief   IEFSR402 Pressure Sensor
*
*  ---------------------------------------------------------------------------
*******************************************************************************/

/*===========================================================================
						   Header file
===========================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include "qapi_adc.h"
#include "qapi_timer.h"
#include "../../main/log.h"
#include "fsr402_pressure_adc.h"

/**************************************************************************
*								  GLOBAL
***************************************************************************/
#define V_TO_Q_SIZE 34

V_TO_Q voltage_to_quality[V_TO_Q_SIZE] = {
{0, 12/*mv*/},
{200, 622/*mv*/},
{400, 746/*mv*/},
{600, 845/*mv*/},
{800, 856/*mv*/},
{1000, 883/*mv*/},
{1200, 893/*mv*/},
{1400, 918/*mv*/},
{1600, 958/*mv*/},
{1800, 961/*mv*/},
{2000, 963/*mv*/},
{2200, 971/*mv*/},
{2400, 975/*mv*/},
{2600, 976/*mv*/},
{2800, 977/*mv*/},
{3000, 978/*mv*/},
{3200, 979/*mv*/},
{3400, 980/*mv*/},
{3600, 982/*mv*/},
{3800, 983/*mv*/},
{4000, 984/*mv*/},
{4200, 985/*mv*/},
//{4400, 985/*mv*/},
//{4600, 985/*mv*/},
{4800, 986/*mv*/},
//{5000, 986/*mv*/},
{5400, 990/*mv*/},
{5600, 991/*mv*/},
{5800, 993/*mv*/},
{6000, 995/*mv*/},
{6200, 997/*mv*/},
{6400, 998/*mv*/},
{6600, 1000/*mv*/},
//{6800, 1000/*mv*/},
{7000, 1001/*mv*/},
//{7200, 1001/*mv*/},
//{7400, 1001/*mv*/},
//{7600, 1001/*mv*/},
//{7800, 1001/*mv*/},
//{8000, 1001/*mv*/},
{8200, 1002/*mv*/},
//{8400, 1002/*mv*/},
//{8600, 1002/*mv*/},
//{8800, 1002/*mv*/},
//{9000, 1002/*mv*/},
//{9200, 1002/*mv*/},
//{9400, 1002/*mv*/},
{9600, 1003/*mv*/},
//{9800, 1003/*mv*/},
{10000, 1004/*mv*/}
};

/*ADC handle*/
qapi_ADC_Handle_t adc_handle;

/**************************************************************************
*                                 FUNCTION
***************************************************************************/
qapi_Status_t adc_open_handle(void)
{
	qapi_Status_t status = QAPI_ERROR;
    status = qapi_ADC_Open(&adc_handle, 0);
    if (QAPI_OK != status) 
    {
        gizLog(LOG_ERROR, "open adc error!\n");
    }
    return status;
}

qapi_Status_t adc_get_properties(const char *Channel_Name_Ptr,
				        qapi_Adc_Input_Properties_Type_t *Properties_Ptr
				        )
{
    qapi_Status_t status = QAPI_ERROR;
    uint32_t Channel_Name_Size = strlen(Channel_Name_Ptr) + 1;

    status = qapi_ADC_Get_Input_Properties(adc_handle, Channel_Name_Ptr, Channel_Name_Size, Properties_Ptr);
    if (QAPI_OK != status)
    {   
		gizLog(LOG_ERROR, "ADC Get Channel-%s Properties ERROR!\n", Channel_Name_Ptr);
    }
    return status;
}

int16_t to_quality(int32 m_voltage)
{
    uint8_t i =0;
    int16_t quilty = 0;

    for(i = 0; i < V_TO_Q_SIZE; i++)
    {
        if(m_voltage <= voltage_to_quality[i].voltage)
        {
            quilty = voltage_to_quality[i].quality;
            return quilty;
        }  
        i++;
    }

    if(i >= V_TO_Q_SIZE)
        return -1;
}

int fsr402_pressure_adc_read(void)
{
    int ret = -1;
    int32 m_voltage = 0;
    int16_t quilty = 0;
    qapi_ADC_Read_Result_t result;
	qapi_Status_t status = QAPI_ERROR;

    const char *channel_name_adc0 = ADC_INPUT_ADC0;
    
    qapi_Adc_Input_Properties_Type_t Properties_ADC0;

    /*get an adc handle*/
    status = adc_open_handle();
    if(status != QAPI_OK)
    {
        gizLog(LOG_ERROR,"Get ADC Handle ERROR!\n");
        return status;
    }

    /*get the adc channel configuration*/
    status = adc_get_properties(channel_name_adc0, &Properties_ADC0);
    if(status != QAPI_OK)
    {
        gizLog(LOG_ERROR,"Get ADC channel-%s Configuration ERROR!\n", channel_name_adc0);
        return status;
    }   
        /*read channel ADC0*/
        memset(&result, 0, sizeof(result));
        status = qapi_ADC_Read_Channel(adc_handle,  &Properties_ADC0, &result);
        if(QAPI_OK == status)
        {
            if(ADC_RESULT_VALID == result.eStatus)
            {  
                m_voltage = result.nMicrovolts/1000;
                
                quilty = to_quality(m_voltage);
                if(quilty > 0)
                {
                    gizLog(LOG_INFO,"Input %s  Voltage: %d mV  about %d g\n", channel_name_adc0, m_voltage, quilty);
                }
                else if (quilty == 0)
                {
                    gizLog(LOG_INFO,"no press.\n");
                }
                else
                {
                    gizLog(LOG_INFO,"Out of range!\n");
                }  
            }
        }

    status = qapi_ADC_Close(adc_handle, false);
    if(QAPI_OK != status)
    {
        gizLog(LOG_ERROR,"Free ADC Handle ERROR!\n");
    }
    return quilty;
}
