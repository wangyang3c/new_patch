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

#define NUM_REFERENCE 3
#define AMUX_1_INPUT_CHANNEL_REFERENCE0 8
#define AMUX_1_INPUT_CHANNEL_REFERENCE1 9
#define AMUX_1_INPUT_CHANNEL_REFERENCE2 10
#define AMUX_2_INPUT_INDEX_GND 8
#define RESISTANCE_REFERENCE0 10e3  //Ohm
#define RESISTANCE_REFERENCE1 100e3
#define RESISTANCE_REFERENCE2 1000e3
float resistance_reference[NUM_REFERENCE] = {RESISTANCE_REFERENCE0, RESISTANCE_REFERENCE1, RESISTANCE_REFERENCE2};
float channel_reference[NUM_REFERENCE] = {AMUX_1_INPUT_CHANNEL_REFERENCE0, AMUX_1_INPUT_CHANNEL_REFERENCE1, AMUX_1_INPUT_CHANNEL_REFERENCE2};

#define VOLTAGE_VCC 3.3

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
#define REQUEST_LENGTH_SET_LED 4
#define REQUEST_BYTE_INDEX_SET_LED_STATUS 2
#define LED_OFF 0x00
#define LED_ON 0x01
// Response



// Single/Continuous data
#define TELEGRAM_TYPE_GET_SINGLE_DATA 0x03
#define TELEGRAM_TYPE_GET_CONTINUOUS_DATA 0x04
// Request
#define REQUEST_LENGTH_GET_DATA 11
#define REQUEST_BYTE_INDEX_GET_DATA_XMIN 2
#define REQUEST_BYTE_INDEX_GET_DATA_XMAX 3
#define REQUEST_BYTE_INDEX_GET_DATA_YMIN 4
#define REQUEST_BYTE_INDEX_GET_DATA_YMAX 5
#define REQUEST_BYTE_INDEX_GET_DATA_SWITCH_HIGH 6
#define REQUEST_BYTE_INDEX_GET_DATA_SWITCH_LOW 7
#define REQUEST_BYTE_INDEX_GET_DATA_MEAS_HIGH 8
#define REQUEST_BYTE_INDEX_GET_DATA_MEAS_LOW 9
// Response
#define RESPONSE_LENGTH_GET_DATA 135 // 1+1+4+128+1 = 135
#define RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART1 2
#define RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART2 3
#define RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART3 4
#define RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART4 5
#define RESPONSE_BYTE_INDEX_GET_DATA_RESISTANCE_START 6
#define RESPONSE_BYTE_INDEX_GET_DATA_END_BYTE 134


// Set offset
#define TELEGRAM_TYPE_SET_OFFSET 0x05
// Request
#define REQUEST_LENGTH_SET_OFFSET 3



// Stop data
#define TELEGRAM_TYPE_STOP_SENSOR_DATA 0x08
// Request
#define REQUEST_LENGTH_STOP_SENSOR_DATA 3



// Get sampling frequency
#define TELEGRAM_TYPE_GET_SAMPLING_FREQUENCY 0x09
// Request
#define REQUEST_LENGTH_GET_SAMPLING_FREQUENCY   3
// Response
#define RESPONSE_LENGTH_GET_SAMPLING_FREQUENCY  5
#define RESPONSE_BYTE_INDEX_GET_SAMPLING_FREQUENCY_SFREQ_COUNT_HIGH 2
#define RESPONSE_BYTE_INDEX_GET_SAMPLING_FREQUENCY_SFREQ_COUNT_LOW  3
#define RESPONSE_BYTE_INDEX_GET_SAMPLING_FREQUENCY_END_BYTE 4


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
void setReferenceAuto(void);
void setOffset(void);
uint16_t encodeResistance(float offset, float resistance, float factor);
void SysTick_Callback();

void receiveRequest(void);
void sendResponse(char type);
void handleTelegramError(void);

void readUntilSize(uint8_t* buffer, size_t size, size_t expectedSize);
void readUntilByte(uint8_t* buffer, size_t size, const uint8_t expectedByte);
void write(uint8_t* buffer, size_t size);


/***************************************
 * Global variables
 ****************************************/
struct {
    uint8_t ledStatus;
    uint8_t xmin;
    uint8_t xmax;
    uint8_t ymin;
    uint8_t ymax;
    uint16_t delay_switch;   //us
    uint16_t delay_meas;     //ms
    uint8_t index_reference[NUM_PATCH_Y][NUM_PATCH_X];
    uint8_t get_data_mode;
} measurementConfig = 
{
    .ledStatus = LED_ON,
    .xmin = 0,
    .xmax = 3,
    .ymin = 0,
    .ymax = 3,
    .delay_switch = 300,
    .delay_meas = 50,
    .index_reference = {[0 ... NUM_PATCH_Y-1] = {[0 ... NUM_PATCH_X-1] = 0}},
    .get_data_mode = GET_DATA_MODE_STOP
};


struct {
    uint32_t timestamp;
    float voltage[NUM_PATCH_Y][NUM_PATCH_X];
    float resistance [NUM_PATCH_Y][NUM_PATCH_X];
    float offset_resistance[NUM_PATCH_Y][NUM_PATCH_X];
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

    Cy_SysTick_Init(CY_SYSTICK_CLOCK_SOURCE_CLK_CPU , 100000); //Clock frequency 100MHz (1/100us per tick)
    Cy_SysTick_SetCallback(0,SysTick_Callback);
    
    UART_Start();
    setvbuf(stdin, NULL, _IONBF, 0);

    AMux_1_Start();
    
    AMux_2_Start();
    AMux_2_Connect(AMUX_2_INPUT_INDEX_GND);


    ADC_Start();


    /* Initialize and enable the opamp. */
    (void)Cy_CTB_OpampInit(Opamp_1_CTB_HW, Opamp_1_OPAMP_NUM, &Opamp_1_opampConfig);
    Cy_CTB_Enable(Opamp_1_CTB_HW);

    (void)Cy_CTB_OpampInit(Opamp_2_CTB_HW, Opamp_2_OPAMP_NUM, &Opamp_2_opampConfig);
    Cy_CTB_Enable(Opamp_2_CTB_HW);

    readADC();
    setOffset();
    
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
        
        /*
        for(uint8 y = 0;y < 8;y++) {
            for(uint8 x = 0;x < 8;x++) {
                //printf("%.2f,", measurementData.resistance[y][x]);
                printf("%d,", convertResistanceToCode(measurementData.offset_resistance[y][x], measurementData.resistance[y][x], 1.0));
            }
        }
        printf("\r\n");
        */
        setReferenceAuto();
        CyDelay(measurementConfig.delay_meas);
    }
}


/***************************************
 *  measurement implementation
 ****************************************/
void readADC(void)
{

    for(int y = measurementConfig.ymin ;y < measurementConfig.ymax + 1;y++)
    {
        for(int x = measurementConfig.xmin; x < measurementConfig.xmax + 1; x++)
        {
            
            AMux_1_Select(channel_reference[measurementConfig.index_reference[y][x]]);
            AMux_1_Connect(y);
            AMux_2_FastSelect(x); 
            CyDelayUs(measurementConfig.delay_switch);
            ADC_StartConvert();
            while (!ADC_IsEndConversion(CY_SAR_WAIT_FOR_RESULT));
            ADC_StopConvert();
            measurementData.voltage[y][x] = ADC_CountsTo_Volts(0, ADC_GetResult16(0));
            measurementData.resistance[y][x] = measurementData.voltage[y][x]/
                                                (VOLTAGE_VCC - measurementData.voltage[y][x])*
                                                resistance_reference[measurementConfig.index_reference[y][x]];
        }  
    }

}

void SysTick_Callback() {
    measurementData.timestamp++;
}


uint16_t encodeResistance(float offset, float resistance, float factor) {
    
    float minimum = -factor;
    float maximum = factor;
    float lsb = 2 * factor /65536;
    float ratio = (resistance - offset)/offset;
    uint16_t code;
    if(ratio <= minimum) {
        code = 0x8000;
    
    } else if(ratio >= maximum) {
        code = 0x7FFF;
    } else {
        code  = (uint16_t)((ratio - minimum) / lsb) - 0x8000;
    }
    return code;
}


void setReferenceAuto(void)
{
  
     for(int y = measurementConfig.ymin ;y < measurementConfig.ymax + 1;y++)
    {
        for(int x = measurementConfig.xmin; x < measurementConfig.xmax + 1; x++)
        {
            if (measurementData.voltage[y][x] < 1.65)
            {
                measurementConfig.index_reference[y][x] = (measurementConfig.index_reference[y][x] - 1) % NUM_REFERENCE;
            }
            else if (measurementData.voltage[y][x] > 3) {
                measurementConfig.index_reference[y][x] = (measurementConfig.index_reference[y][x] + 1) % NUM_REFERENCE;
            }
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





void setOffset(void) {
    for(int y = measurementConfig.ymin ;y < measurementConfig.ymax + 1;y++)
    {
        for(int x = measurementConfig.xmin; x < measurementConfig.xmax + 1; x++)
        { 
            measurementData.offset_resistance[y][x] = measurementData.resistance[y][x]; 
        }
    
    }    



}




/***************************************
 *  Communication implementatio 
 ****************************************/
void receiveRequest() {
    
    uint8_t rxBuffer[256];
    
    readUntilSize(rxBuffer, sizeof(rxBuffer), TELEGRAM_BYTE_INDEX_ARGS_START);
    uint8_t requestStart = rxBuffer[TELEGRAM_BYTE_INDEX_START_BYTE];
    if(requestStart != TELEGRAM_START_BYTE) 
    {
        handleTelegramError();
        return;
    }
    
    uint8_t requestType = rxBuffer[TELEGRAM_BYTE_INDEX_TYPE];
    switch(requestType) {
        case TELEGRAM_TYPE_TEST:
        {
            readUntilSize(&rxBuffer[TELEGRAM_BYTE_INDEX_ARGS_START], sizeof(rxBuffer), REQUEST_LENGTH_TEST - 2);
            if(rxBuffer[REQUEST_LENGTH_TEST-1] != TELEGRAM_END_BYTE) {
                handleTelegramError();
                return;
            } 
            sendResponse(TELEGRAM_TYPE_TEST);
            break;
        }
            
        case TELEGRAM_TYPE_SET_LED:
        {
            readUntilSize(&rxBuffer[TELEGRAM_BYTE_INDEX_ARGS_START], sizeof(rxBuffer), REQUEST_LENGTH_SET_LED - 2);
            if(rxBuffer[REQUEST_LENGTH_SET_LED - 1] != TELEGRAM_END_BYTE) {
                handleTelegramError();
                return;
            } 
            measurementConfig.ledStatus = rxBuffer[REQUEST_BYTE_INDEX_SET_LED_STATUS];
            setLED();
            break;
        }
            
        case TELEGRAM_TYPE_GET_SINGLE_DATA:
        {
            readUntilSize(&rxBuffer[TELEGRAM_BYTE_INDEX_ARGS_START], sizeof(rxBuffer), REQUEST_LENGTH_GET_DATA - 2);
            if(rxBuffer[REQUEST_LENGTH_GET_DATA - 1] != TELEGRAM_END_BYTE) {
                handleTelegramError();
                return;
            } 
            measurementConfig.get_data_mode = GET_DATA_MODE_SINGLE;
            measurementConfig.xmin = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_XMIN];
            measurementConfig.xmax = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_XMAX];
            measurementConfig.ymin = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_YMIN];
            measurementConfig.ymax = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_YMAX];
            measurementConfig.delay_switch = ((uint16_t)rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_SWITCH_HIGH] << 8) | rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_SWITCH_LOW];
            measurementConfig.delay_meas = ((uint16_t)rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MEAS_HIGH] << 8) | rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MEAS_LOW];
            break;
        }
            
            
        case TELEGRAM_TYPE_GET_CONTINUOUS_DATA:
        {
            readUntilSize(&rxBuffer[TELEGRAM_BYTE_INDEX_ARGS_START], sizeof(rxBuffer), REQUEST_LENGTH_GET_DATA - 2);
            if(rxBuffer[REQUEST_LENGTH_GET_DATA - 1] != TELEGRAM_END_BYTE) {
                handleTelegramError();
                return;
            } 
            measurementConfig.get_data_mode = GET_DATA_MODE_CONTINUNOUS;
            measurementConfig.xmin = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_XMIN];
            measurementConfig.xmax = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_XMAX];
            measurementConfig.ymin = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_YMIN];
            measurementConfig.ymax = rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_YMAX];
            measurementConfig.delay_switch = ((uint16_t)rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_SWITCH_HIGH] << 8) | rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_SWITCH_LOW];
            measurementConfig.delay_meas = ((uint16_t)rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MEAS_HIGH] << 8) | rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MEAS_LOW];
            break;
        }
            
        case TELEGRAM_TYPE_SET_OFFSET:
        {
            readUntilSize(&rxBuffer[TELEGRAM_BYTE_INDEX_ARGS_START], sizeof(rxBuffer), REQUEST_LENGTH_SET_OFFSET - 2);
            if(rxBuffer[REQUEST_LENGTH_SET_OFFSET - 1] != TELEGRAM_END_BYTE) {
                handleTelegramError();
                return;
            } 
            setOffset();
            break;
        }
            

        case TELEGRAM_TYPE_STOP_SENSOR_DATA:
        {
            readUntilSize(&rxBuffer[TELEGRAM_BYTE_INDEX_ARGS_START], sizeof(rxBuffer), REQUEST_LENGTH_STOP_SENSOR_DATA - 2);
            if(rxBuffer[REQUEST_LENGTH_STOP_SENSOR_DATA - 1] != TELEGRAM_END_BYTE) {
                handleTelegramError();
                return;
            } 
            measurementConfig.get_data_mode = GET_DATA_MODE_STOP;
            break;
        }
           
        case TELEGRAM_TYPE_GET_SAMPLING_FREQUENCY:
        {
            break;
        }
        default:
        {
            handleTelegramError();
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
            for(unsigned int i = 0; i < sizeof(RESPONSE_TEST);++i) txBuffer[TELEGRAM_BYTE_INDEX_ARGS_START + i] = RESPONSE_TEST[i];
            txBuffer[RESPONSE_BYTE_INDEX_TEST_END_BYTE] = TELEGRAM_END_BYTE;
            write(txBuffer,RESPONSE_LENGTH_TEST);
            break;
        }
            
        case TELEGRAM_TYPE_GET_SINGLE_DATA:
        case TELEGRAM_TYPE_GET_CONTINUOUS_DATA:
        { 
            txBuffer[TELEGRAM_BYTE_INDEX_START_BYTE] = TELEGRAM_START_BYTE;
            txBuffer[TELEGRAM_BYTE_INDEX_TYPE] = TELEGRAM_TYPE_GET_SINGLE_DATA;
            
            txBuffer[RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART1] = measurementData.timestamp >> 24;
            txBuffer[RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART2] = measurementData.timestamp >> 16;
            txBuffer[RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART3] = measurementData.timestamp >> 8;
            txBuffer[RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART4] = measurementData.timestamp;
            int idx = RESPONSE_BYTE_INDEX_GET_DATA_RESISTANCE_START;
            for(int y = 0; y < NUM_PATCH_Y; y++) {
                for(int x = 0; x < NUM_PATCH_X; x++) {
                    uint16_t resistance_code = encodeResistance(measurementData.offset_resistance[y][x], measurementData.resistance[y][x], 10);
                    uint8_t resistance_code_low = resistance_code;
                    uint8_t resistance_code_high = resistance_code >> 8;
                    txBuffer[idx] = resistance_code_high;
                    idx++;
                    txBuffer[idx] = resistance_code_low;
                    idx++;
                }
            }
            txBuffer[RESPONSE_BYTE_INDEX_GET_DATA_END_BYTE] = TELEGRAM_END_BYTE;
            write(txBuffer, RESPONSE_LENGTH_GET_DATA);
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


void handleTelegramError(void) {
    sendResponse(TELEGRAM_TYPE_ERROR);
    Cy_SCB_UART_ClearRxFifo(UART_HW);
}







void readUntilSize(uint8_t* buffer, size_t size, size_t expectedSize) 
{
    uint8_t temp[1];
    unsigned int i = 0;
    while(true)
    {   
        Cy_SCB_UART_GetArrayBlocking(UART_HW, temp, sizeof(temp));
        buffer[i] = temp[0];
        ++i; 
        if(i == expectedSize || i == size) break;
    }   

}


void readUntilByte(uint8_t* buffer, size_t size, const uint8_t expectedByte) 
{   
    uint8_t temp[1];
    unsigned int i = 0;
    while(true)
    {   
        Cy_SCB_UART_GetArrayBlocking(UART_HW, temp, sizeof(temp));
        buffer[i] = temp[0];
        ++i; 
        if(temp[0] == expectedByte || i == size) break;
    }        
}


void write(uint8_t* buffer, size_t size) {
    Cy_SCB_UART_PutArrayBlocking(UART_HW, buffer, size);
    /* Blocking wait for transfer completion */
    while (!Cy_SCB_UART_IsTxComplete(UART_HW));
}


/* [] END OF FILE */
