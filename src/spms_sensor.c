// Includes
#include "spms_libs.h"
#include "spms_sensor.h"
#include <stdio.h>
#include <math.h>


// Compiler macros
#define ADC_LABEL DT_LABEL(DT_INST(0, nordic_nrf_saadc))
#define ADC_RESOLUTION 10
#define ADC_GAIN ADC_GAIN_1_5
#define ADC_REFERENCE ADC_REF_INTERNAL
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 10)
#define ADC_1ST_CHANNEL_ID 0
#define ADC_1ST_CHANNEL_INPUT NRF_SAADC_INPUT_AIN1
#define ADC_2ND_CHANNEL_ID 2
#define ADC_2ND_CHANNEL_INPUT NRF_SAADC_INPUT_AIN2

#define BME280_NAME DT_INST(0, bosch_bme280)
#if DT_NODE_HAS_STATUS(BME280_NAME, okay)
#define BME280_LABEL DT_LABEL(BME280_NAME)
#else
#define BME280_LABEL "<none>"
#endif

// Globals
static struct device* devBME = NULL;
static struct device* devADC = NULL;

static const struct adc_channel_cfg m_1st_channel_cfg = {
	.gain             = ADC_GAIN,
	.reference        = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id       = ADC_1ST_CHANNEL_ID,
	.input_positive   = ADC_1ST_CHANNEL_INPUT,
};

static const struct adc_channel_cfg m_2nd_channel_cfg = {
	.gain             = ADC_GAIN,
	.reference        = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id       = ADC_2ND_CHANNEL_ID,
	.input_positive   = ADC_2ND_CHANNEL_INPUT,
};

// Initialize the sensors
int sensor_init()
{
    int status = 0;

    devADC = (struct device*) device_get_binding(ADC_LABEL);
    if (devADC == NULL) {
       printk("[Warning] No ADC's found\n");
       status = status ^ 0x1;
    }
    else { 
       printk("[Log] ADC found\n");
       
       if (!adc_channel_setup(devADC, &m_1st_channel_cfg)) printk("[Log] ADC channel 1 configured\n");
       else status = status ^ 0x1;
    
       if (!adc_channel_setup(devADC, &m_2nd_channel_cfg)) printk("[Log] ADC channel 2 configured\n");
       else status = status ^ 0x1;
    }

    devBME = (struct device*) device_get_binding(BME280_LABEL);
    if (devBME == NULL) {
        printk("[Warning] No BME 280 device found\n");
        status = status ^ 0x1;
    }
    else printk("[Log] BME 280 device found\n");

    return status;
}

// Read sensor data into local memory
int sensor_read(airflow_local* airflowMem) 
{
    int16_t tmp;
    double Vout, Vout_debug; 
    double offset = 1.210; 
    struct sensor_value temp, humi, pres, airf;

    struct adc_sequence sequence1 = {
		.channels    = BIT(ADC_1ST_CHANNEL_ID),
		.buffer      = &tmp,
		.buffer_size = sizeof(tmp),
		.resolution  = ADC_RESOLUTION,
		.oversampling = 0,	// don't oversample
		.calibrate = 0		
	};
    
    adc_read(devADC, &sequence1);

    // calculate V airflow sensor
    Vout = 3.3 * tmp * offset / 1023; 
    Vout_debug = 3.3 * tmp * offset / 1023 * 1000;  
    int Vout_int = (int)Vout_debug; 

    printk("[Sensor] Vout :%d\n", Vout_int); 

    // calculate speed from V
    airflowMem->airf = pow((((Vout - 1.30)/(3.038517*pow(25,0.115157)))/0.087288),3.009364)*0.44704;


    printf("[Sensor] adc value :%d\n", tmp);
    printf("[Sensor] airflow speed :%d\n", airflowMem->airf);

    if (devBME != NULL) {

        printk("[Log] Fetching BME 280 data\n");


		sensor_sample_fetch(devBME);
		sensor_channel_get(devBME, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		sensor_channel_get(devBME, SENSOR_CHAN_HUMIDITY, &humi);
		sensor_channel_get(devBME, SENSOR_CHAN_PRESS, &pres);

        printk("[Log] Storing BME 280 data in local memory\n");

		airflowMem->temp = (uint16_t) (((temp.val1 * 100) + (temp.val2 / 10000)));
		airflowMem->humi = (uint16_t) (((humi.val1 * 100) + (humi.val2 / 10000)));
		airflowMem->pres = (uint16_t) pres.val1;

		printk("[Sensor] temp: %d.%06d; press: %d.%06d; humidity: %d.%06d\n",
			temp.val1, temp.val2, pres.val1, pres.val2,	humi.val1, humi.val2);
	}
    else {
        return 1;
    }

    return 0;
}