/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
 */
#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>





/***************************************
 *            Constants
 ****************************************/
#define NUM_PATCH_X 8
#define NUM_PATCH_Y 8

#define AMUX_1_INPUT_INDEX_REFERENCE0 8
#define AMUX_1_INPUT_INDEX_REFERENCE1 9
#define AMUX_1_INPUT_INDEX_REFERENCE2 10
#define AMUX_2_INPUT_INDEX_GND 8
#define RESISTANCE_REFERENCE0 10e3  //Ohm
#define RESISTANCE_REFERENCE1 100e3
#define RESISTANCE_REFERENCE2 1000e3

// Telegram pblic
#define TELEGRAM_START_BYTE 0x81
#define TELEGRAM_END_BYTE 0x7E
#define TELEGRAM_BYTE_INDEX_START_BYTE 0
#define TELEGRAM_BYTE_INDEX_TYPE 1
#define TELEGRAM_BYTE_INDEX_ARGS_START 2

// Test
#define TELEGRAM_TYPE_TEST 0x01
// Request
#define REQUEST_LENGTH_TEST 3
// Response
#define RESPONSE_LENGTH_TEST 8
#define RESPONSE_TEST "Test\0"
#define RESPONSE_BYTE_INDEX_TEST_END_BYTE 7


// Set LED
#define TELEGRAM_TYPE_SET_LED 0x02
// Request
#define REQUEST_LENGTH_LED 4
#define REQUEST_BYTE_INDEX_LED_STATUS 2
#define LED_OFF 0x00
#define LED_ON 0x01
// Response



// Single/Continuous data
#define TELEGRAM_TYPE_GET_SINGLE_DATA 0x03
#define TELEGRAM_TYPE_GET_CONTINUOUS_DATA 0x04
// Request
#define REQUEST_LENGTH_GET_DATA 12
#define REQUEST_BYTE_INDEX_GET_DATA_XMIN 2
#define REQUEST_BYTE_INDEX_GET_DATA_XMAX 3
#define REQUEST_BYTE_INDEX_GET_DATA_YMIN 4
#define REQUEST_BYTE_INDEX_GET_DATA_YMAX 5
#define REQUEST_BYTE_INDEX_GET_DATA_MVALUES 6
#define REQUEST_BYTE_INDEX_GET_DATA_SWITCH_HIGH 7
#define REQUEST_BYTE_INDEX_GET_DATA_SWITCH_LOW 8
#define REQUEST_BYTE_INDEX_GET_DATA_MEAS_HIGH 9
#define REQUEST_BYTE_INDEX_GET_DATA_MEAS_LOW 10
// Response
#define REPONSE_LENGTH_GET_DATA 131 // 1+1+8*8*2+1 = 131
#define RESPONSE_BYTE_INDEX_GET_DATA_END_BYTE 130



// Set offset auto
#define TELEGRAM_TYPE_SET_OFFSET_AUTO 0x05
// Request
#define REQUEST_LENGTH_SET_OFFSET_AUTO 3
// Response
#define RESPONSE_LENGTH_SET_OFFSET_AUTO 131
#define RESPONSE_BYTE_INDEX_SET_OFFSET_AUTO_END_BYTE 130


// Set offset manual
#define TELEGRAM_TYPE_SET_OFFSET_MANUAL 0x06
// Request
#define REQUEST_LENGTH_SET_OFFSET_MANUAL 131
// Response


// Set gain
#define TELEGRAM_TYPE_SET_GAIN '\x07'
// Request
#define REQUEST_LENGTH_SET_GAIN
// Response



// Stop data
#define TELEGRAM_TYPE_STOP_SENSOR_DATA 0x08
// Request
#define REQUEST_LENGTH_STOP_SENSOR_DATA 3
// Response


// Get sampling frequency
#define TELEGRAM_TYPE_GET_SAMPLING_FREQUENCY 0x09
// Request
#define REQUEST_LENGTH_GET_SAMPLING_FREQUENCY   3
// Response
#define RESPONSE_LENGTH_GET_SAMPLING_FREQUENCY  5
#define RESPONSE_BYTE_INDEX_GET_SAMPLING_FREQUENCY_SFREQ_COUNT_HIGH 2
#define RESPONSE_BYTE_INDEX_GET_SAMPLING_FREQUENCY_SFREQ_COUNT_LOW  3
#define RESPONSE_BYTE_INDEX_GET_SAMPLING_FREQUENCY_END_BYTE 4

// Get timestamp
#define TELEGRAM_TYPE_GET_TIMESTAMP 0x0A
// Request
#define REQUEST_LENGTH_GET_TIMESTAMP    3
// Response
#define RESPONSE_LENGTH_GET_TIMESTAMP   7
#define RESPONSE_BYTE_INDEX_GET_TIMESTAMP_TIMESTAMP_PART_1   2
#define RESPONSE_BYTE_INDEX_GET_TIMESTAMP_TIMESTAMP_PART_2   3
#define RESPONSE_BYTE_INDEX_GET_TIMESTAMP_TIMESTAMP_PART_3   4
#define RESPONSE_BYTE_INDEX_GET_TIMESTAMP_TIMESTAMP_PART_4   5
#define RESPONSE_BYTE_INDEX_GET_TIMESTAMP_END_BYTE 6

//Error
#define TELEGRAM_TYPE_ERROR 0xFF
#define RESPONSE_LENGTH_ERROR 3
#define RESPONSE_BYTE_INDEX_ERROR_END_BYTE 2




// Setting
#define GET_DATA_MODE_STOP 0
#define GET_DATA_MODE_SINGLE 1
#define GET_DATA_MODE_CONTINUNOUS 2




/***************************************
 *         Function Prototypes
 ****************************************/
void sendData(void);
void readADC(void);
void setLED(void);
void setOffsetAuto(void);
void setOffsetManual(uint16_t offset, uint8_t y, uint8_t x);

void receiveRequest(void);
void sendResponse(char type);

void readUntil(uint8_t* buffer, size_t size, const uint8_t expected);
void write(uint8_t* buffer, size_t size);
void pushStringToBuffer(uint8_t* buffer, size_t size, unsigned int position, char* str);


/***************************************
 * Global variables
 ****************************************/
float offset[8][8];
float v_patch[8][8];
int referenceChannel = 1;

struct {
    uint8_t ledStatus;
    uint8_t xmin;
    uint8_t xmax;
    uint8_t ymin;
    uint8_t ymax;
    uint8_t m_values;
    uint16_t delay_switch;   //us
    uint16_t delay_meas;     //ms
    uint8_t channel_reference;
    uint8_t get_data_mode;
} measurementConfig = 
{
    .ledStatus = LED_ON,
    .xmin = 0,
    .xmax = 7,
    .ymin = 0,
    .ymax = 7,
    .m_values = 16,
    .delay_switch = 300,
    .delay_meas = 50,
    .channel_reference = AMUX_1_INPUT_INDEX_REFERENCE0,
    .get_data_mode = GET_DATA_MODE_STOP
};


struct {
    int timestamp;
    uint16_t voltage_count[NUM_PATCH_Y][NUM_PATCH_X];
    uint16_t offset_count[NUM_PATCH_Y][NUM_PATCH_X];
} measurementData;



/*******************************************************************************
 * Function Name: main
 ****************************************************************************/
/**
 *
 *  The main function performs the following actions:
 *   1. Sets up I2C component to acts as master.
 *   2. If initialization of I2C component system will be in infinite loop.
 *   3. I2C master sends commands packets every two seconds to EzI2C slave using high level PDL function
 *   to change to color of RGB LED on the slave.
 *   4. Master reads the reply from the slave to know whether commands are received properly.
 *
 *******************************************************************************/
int main(void)
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();
    setvbuf(stdin, NULL, _IONBF, 0);

    AMux_1_Start();
    AMux_1_Connect(measurementConfig.channel_reference);

    AMux_2_Start();
    AMux_2_Connect(AMUX_2_INPUT_INDEX_GND);


    ADC_Start();


    /* Initialize and enable the opamp. */
    (void)Cy_CTB_OpampInit(Opamp_1_CTB_HW, Opamp_1_OPAMP_NUM, &Opamp_1_opampConfig);
    Cy_CTB_Enable(Opamp_1_CTB_HW);

    (void)Cy_CTB_OpampInit(Opamp_2_CTB_HW, Opamp_2_OPAMP_NUM, &Opamp_2_opampConfig);
    Cy_CTB_Enable(Opamp_2_CTB_HW);


    for (;;)
    {
        /* Place your application code here. */
        if(Cy_SCB_UART_GetNumInRxFifo(UART_HW)>0) {
            receiveRequest();
        }
        readADC();
        switch(measurementConfig.get_data_mode) {
            case GET_DATA_MODE_STOP:
                break;
            case GET_DATA_MODE_SINGLE:
                sendResponse(TELEGRAM_TYPE_GET_SINGLE_DATA);
                measurementConfig.get_data_mode = GET_DATA_MODE_STOP;
                break;
            case GET_DATA_MODE_CONTINUNOUS:
                sendResponse(TELEGRAM_TYPE_GET_CONTINUOUS_DATA);
                break;
        }
        CyDelay(measurementConfig.delay_meas);
    }
}



void readADC(void)
{

    for(int y = measurementConfig.ymin ;y < measurementConfig.ymax + 1;y++)
    {
        AMux_1_FastSelect(y);
        for(int x = measurementConfig.xmin; x < measurementConfig.xmax + 1; x++)
        {
            AMux_2_FastSelect(x); 
            CyDelayUs(measurementConfig.delay_switch);
            ADC_StartConvert();
            while (!ADC_IsEndConversion(CY_SAR_WAIT_FOR_RESULT));
            ADC_StopConvert();
            measurementData.voltage_count[y][x] = ADC_GetResult16(0); 
        }  
    }

}



/***************************************
 *  Command implementation
 ****************************************/
void setLED(void) {
    switch(measurementConfig.ledStatus) {
        case LED_OFF:
            Cy_GPIO_Write(LED_PORT, LED_NUM, 1);
            break;
        case LED_ON:
            Cy_GPIO_Write(LED_PORT, LED_NUM, 0);
            break;
    }
}

void setOffsetAuto(void) {
    for(int y = measurementConfig.ymin ;y < measurementConfig.ymax + 1;y++)
    {
        for(int x = measurementConfig.xmin; x < measurementConfig.xmax + 1; x++)
        {
            measurementData.offset_count[y][x] = measurementData.voltage_count[y][x]; 
        }  
    }
}

void setOffsetManual(uint16_t offset, uint8_t y, uint8_t x) {
    measurementData.offset_count[y][x] = offset;
}



/***************************************
 *  Communication implementatio 
 ****************************************/
void receiveRequest() {
    
    uint8_t rxBuffer[256];
    readUntil(rxBuffer, sizeof(rxBuffer),TELEGRAM_END_BYTE);
    
    char requestStart = rxBuffer[TELEGRAM_BYTE_INDEX_START_BYTE];
    char requestType = rxBuffer[TELEGRAM_BYTE_INDEX_TYPE];
    
    if(requestStart == TELEGRAM_START_BYTE) {
        switch(requestType) {
            case TELEGRAM_TYPE_TEST:
            {
                sendResponse(TELEGRAM_TYPE_TEST);
                break;
            }
                
            case TELEGRAM_TYPE_SET_LED:
            {
                measurementConfig.ledStatus = rxBuffer[REQUEST_BYTE_INDEX_LED_STATUS];
                setLED();
                break;
            }
                
            case TELEGRAM_TYPE_GET_SINGLE_DATA:
            {
                measurementConfig.get_data_mode = GET_DATA_MODE_SINGLE;
                measurementConfig.xmin = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_XMIN];
                measurementConfig.xmax = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_XMAX];
                measurementConfig.ymin = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_YMIN];
                measurementConfig.ymax = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_YMAX];
                measurementConfig.m_values = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MVALUES];
                measurementConfig.delay_switch = ((uint16_t)rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_SWITCH_HIGH] << 8) | rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_SWITCH_LOW];
                measurementConfig.delay_meas = ((uint16_t)rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MEAS_HIGH] << 8) | rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MEAS_LOW];
                break;
            }
                
                
            case TELEGRAM_TYPE_GET_CONTINUOUS_DATA:
            {
                measurementConfig.get_data_mode = GET_DATA_MODE_CONTINUNOUS;
                measurementConfig.xmin = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_XMIN];
                measurementConfig.xmax = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_XMAX];
                measurementConfig.ymin = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_YMIN];
                measurementConfig.ymax = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_YMAX];
                measurementConfig.m_values = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MVALUES];
                measurementConfig.delay_switch = ((uint16_t)rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_SWITCH_HIGH] << 8) | rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_SWITCH_LOW];
                measurementConfig.delay_meas = ((uint16_t)rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MEAS_HIGH] << 8) | rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MEAS_LOW];
                break;
            }
                
            case TELEGRAM_TYPE_SET_OFFSET_AUTO:
            {
                setOffsetAuto();
                sendResponse(TELEGRAM_TYPE_SET_OFFSET_AUTO);
                break;
            }
                
            case TELEGRAM_TYPE_SET_OFFSET_MANUAL:
            {
                for(int i = 0; i < NUM_PATCH_X*NUM_PATCH_Y; ++i) 
                {
                    uint8_t offset_high = rxBuffer[TELEGRAM_BYTE_INDEX_ARGS_START + 2 * i];
                    uint8_t offset_low = rxBuffer[TELEGRAM_BYTE_INDEX_ARGS_START + 2 * i + 1];
                    uint16_t offset = ((uint16_t)offset_high << 8) | offset_low;
                    setOffsetManual(offset, i/NUM_PATCH_Y, i%NUM_PATCH_Y);
                }
                break;
            }

            case TELEGRAM_TYPE_STOP_SENSOR_DATA:
            {
                measurementConfig.get_data_mode = GET_DATA_MODE_STOP;
                break;
            }
               
            case TELEGRAM_TYPE_GET_SAMPLING_FREQUENCY:
            {
                break;
            }
            default:
            {
                sendResponse(TELEGRAM_TYPE_ERROR);
            }
                
        }
    }
}


void sendResponse(char type) {
    uint8_t txBuffer[256];
    switch(type) {
        case TELEGRAM_TYPE_TEST: 
        {
            txBuffer[TELEGRAM_BYTE_INDEX_START_BYTE] = TELEGRAM_START_BYTE;
            txBuffer[TELEGRAM_BYTE_INDEX_TYPE] = TELEGRAM_TYPE_TEST;
            pushStringToBuffer(txBuffer, sizeof(txBuffer), TELEGRAM_BYTE_INDEX_ARGS_START, RESPONSE_TEST);
            txBuffer[RESPONSE_BYTE_INDEX_TEST_END_BYTE] = TELEGRAM_END_BYTE;
            write(txBuffer,RESPONSE_LENGTH_TEST);
            break;
        }
            
        case TELEGRAM_TYPE_GET_SINGLE_DATA:
        case TELEGRAM_TYPE_GET_CONTINUOUS_DATA:
        {
            txBuffer[TELEGRAM_BYTE_INDEX_START_BYTE] = TELEGRAM_START_BYTE;
            txBuffer[TELEGRAM_BYTE_INDEX_TYPE] = TELEGRAM_TYPE_GET_SINGLE_DATA;
            int idx = TELEGRAM_BYTE_INDEX_ARGS_START;
            for(int y = 0; y < NUM_PATCH_Y; y++) {
                for(int x = 0; x < NUM_PATCH_X; x++) {
                    uint16_t voltage_count_after_offset = measurementData.voltage_count[y][x] - measurementData.offset_count[y][x];
                    uint8_t voltage_count_low = voltage_count_after_offset & 0xFF;
                    uint8_t voltage_count_high = (voltage_count_after_offset >> 8) & 0xFF;
                    txBuffer[idx] = voltage_count_high;
                    idx++;
                    txBuffer[idx] = voltage_count_low;
                    idx++;
                }
            }
            txBuffer[RESPONSE_BYTE_INDEX_GET_DATA_END_BYTE] = TELEGRAM_END_BYTE;
            write(txBuffer, REPONSE_LENGTH_GET_DATA);
            break;
        }
        case TELEGRAM_TYPE_SET_OFFSET_AUTO:
        {
            txBuffer[TELEGRAM_BYTE_INDEX_START_BYTE] = TELEGRAM_START_BYTE;
            txBuffer[TELEGRAM_BYTE_INDEX_TYPE] = TELEGRAM_TYPE_SET_OFFSET_AUTO;
            int idx = TELEGRAM_BYTE_INDEX_ARGS_START;
            for(int y = 0; y < NUM_PATCH_Y; y++) {
                for(int x = 0; x < NUM_PATCH_X; x++) {
                    uint16 offset = measurementData.offset_count[y][x];
                    uint8_t offset_count_low =  offset & 0xFF;
                    uint8_t offse_count_high = (offset >> 8) & 0xFF;
                    txBuffer[idx] = offse_count_high;
                    idx++;
                    txBuffer[idx] = offset_count_low;
                    idx++;
                }
            }
            txBuffer[RESPONSE_BYTE_INDEX_SET_OFFSET_AUTO_END_BYTE] = TELEGRAM_END_BYTE;
            write(txBuffer, RESPONSE_LENGTH_SET_OFFSET_AUTO);
            break;
        }
        case TELEGRAM_TYPE_ERROR: {
            txBuffer[TELEGRAM_BYTE_INDEX_START_BYTE] = TELEGRAM_START_BYTE;
            txBuffer[TELEGRAM_BYTE_INDEX_TYPE] = TELEGRAM_TYPE_ERROR;
            txBuffer[RESPONSE_BYTE_INDEX_ERROR_END_BYTE] = TELEGRAM_END_BYTE;
            write(txBuffer, RESPONSE_LENGTH_ERROR);
            break;
        }
        
    }
}

void read(uint8_t* buffer, size_t size) {
    


}

void readUntil(uint8_t* buffer, size_t size, const uint8_t expected) 
{   
    uint8_t temp[1];
    unsigned int i = 0;
    while(true)
    {   
        Cy_SCB_UART_GetArrayBlocking(UART_HW, temp, sizeof(temp));
        buffer[i] = temp[0];
        ++i; 
        if(temp[0] == expected || i == size) break;
    }        
}


void write(uint8_t* buffer, size_t size) {
    Cy_SCB_UART_PutArrayBlocking(UART_HW, buffer, size);
    /* Blocking wait for transfer completion */
    while (!Cy_SCB_UART_IsTxComplete(UART_HW));
}


void pushStringToBuffer(uint8_t* buffer, size_t size, unsigned int position, char* str) {
    for(unsigned int i = 0; i < strlen(str); ++i) {
        if(position + i == size) return;
        buffer[position + i] = str[i];
    }
}

/* [] END OF FILE */
