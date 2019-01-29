/******************************************************************************
*@file    fsr402_pressure_adc.h
*@brief   read value of ADC0 and ADC1
*  ---------------------------------------------------------------------------
*******************************************************************************/
#ifndef __FSR402_PRESSURE_ADC_H__
#define __FSR402_PRESSURE_ADC_H__

typedef struct
{
    int16_t quality;
    int32 voltage;
} V_TO_Q;

int16_t to_quality(int32 m_voltage);
int fsr402_pressure_adc_read(void);


#endif /*__FSR402_PRESSURE_ADC_H__*/

