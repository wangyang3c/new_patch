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
#define SENSOR_DATA_BUFFER_SIZE 256
#define NUM_REFERENCE 3
#define AMUX_1_INPUT_CHANNEL_REFERENCE0 16
#define AMUX_1_INPUT_CHANNEL_REFERENCE1 17
#define AMUX_1_INPUT_CHANNEL_REFERENCE2 18
#define AMUX_2_INPUT_INDEX_GND 16
#define RESISTANCE_REFERENCE0 10e3  //Ohm
#define RESISTANCE_REFERENCE1 100e3
#define RESISTANCE_REFERENCE2 1000e3
float resistance_reference[NUM_REFERENCE] = {RESISTANCE_REFERENCE0, RESISTANCE_REFERENCE1, RESISTANCE_REFERENCE2};
float channel_reference[NUM_REFERENCE] = {AMUX_1_INPUT_CHANNEL_REFERENCE0, AMUX_1_INPUT_CHANNEL_REFERENCE1, AMUX_1_INPUT_CHANNEL_REFERENCE2};

#define VOLTAGE_VCC 3.3


// *************************************** Protocol ***************************************
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
#define REQUEST_BYTE_INDEX_GET_DATA_SWITCH_HIGH 2
#define REQUEST_BYTE_INDEX_GET_DATA_SWITCH_LOW 3
#define REQUEST_BYTE_INDEX_GET_DATA_MEAS_HIGH 4
#define REQUEST_BYTE_INDEX_GET_DATA_MEAS_LOW 5
#define REQUEST_BYTE_INDEX_GET_DATA_SENSOR 6
// Response
#define RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART1 2
#define RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART2 3
#define RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART3 4
#define RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART4 5
#define RESPONSE_BYTE_INDEX_GET_DATA_RESISTANCE_START 6


// Set offset
#define TELEGRAM_TYPE_SET_OFFSET 0x05
// Request
#define REQUEST_LENGTH_SET_OFFSET 3


// Set resistance data conversion factor
#define TELEGRAM_TYPE_SET_RESISTANCE_CODING_FACTOR 0x06
// Request
#define REQUEST_LENGTH_SET_RESISTANCE_CODING_FACTOR 4
#define REQUEST_BYTE_INDEX_SET_RESISTANCE_CODING_FACTOR_FACTOR 2


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
unsigned int getTelegramSize(uint8_t* buffer, size_t size);
void write(uint8_t* buffer, size_t size);

void sendDataDebug(void);


/***************************************
 * Global variables
 ****************************************/
struct {
    uint8_t ledStatus;
    uint8_t num_sensor;
    uint16_t delay_switch;   //us
    uint16_t delay_meas;     //ms
    uint8_t get_data_mode;
    uint8_t resistance_coding_factor;
    uint8_t index_reference[SENSOR_DATA_BUFFER_SIZE];
    uint8_t input_channel[SENSOR_DATA_BUFFER_SIZE];
    uint8_t output_channel[SENSOR_DATA_BUFFER_SIZE];
} measurementConfig = 
{
    .ledStatus = LED_ON,
    .num_sensor = 0,
    .delay_switch = 300,
    .delay_meas = 50,
    .get_data_mode = GET_DATA_MODE_STOP,
    .resistance_coding_factor = 10,
    .index_reference = {[0 ... SENSOR_DATA_BUFFER_SIZE-1] = 0},
    .input_channel = {[0 ... SENSOR_DATA_BUFFER_SIZE-1] = 0},
    .output_channel = {[0 ... SENSOR_DATA_BUFFER_SIZE-1] = 0}
};


struct {
    uint32_t timestamp;
    float voltage[SENSOR_DATA_BUFFER_SIZE];
    float resistance [SENSOR_DATA_BUFFER_SIZE];
    float offset[SENSOR_DATA_BUFFER_SIZE];
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

    //readADC();
    //setOffset();
    
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
                //sendDataDebug();

                break;
        }
        

        setReferenceAuto();
        CyDelay(measurementConfig.delay_meas);
    }
}


/***************************************
 *  measurement implementation
 ****************************************/
void readADC(void)
{

    for(int i = 0; i < measurementConfig.num_sensor; ++i)
    {
            
            AMux_1_Select(channel_reference[measurementConfig.index_reference[i]]);
            AMux_1_Connect(measurementConfig.input_channel[i]);
            AMux_2_FastSelect(measurementConfig.output_channel[i]); 
            CyDelayUs(measurementConfig.delay_switch);
            ADC_StartConvert();
            while (!ADC_IsEndConversion(CY_SAR_WAIT_FOR_RESULT));
            ADC_StopConvert();
            measurementData.voltage[i] = ADC_CountsTo_Volts(0, ADC_GetResult16(0));
            measurementData.resistance[i] = measurementData.voltage[i]/
                                                (VOLTAGE_VCC - measurementData.voltage[i])*
                                                resistance_reference[measurementConfig.index_reference[i]];
        }  


}

void SysTick_Callback() {
    measurementData.timestamp++;
}


uint16_t encodeResistance(float offset, float resistance, float factor) {
    /*
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
    */
    
    float lsb_big = (factor - 1.0) / 32768;
    float lsb_small = 1.0 / 32768;
    float ratio = resistance / offset;
    uint16_t code;
    if(ratio < 1) {
        code  = 0x8000 - (uint16_t)((1.0 - ratio) / lsb_small);
    } 
    else if(ratio >= 1 && ratio < factor) {
        code  = 0x8000 + (uint16_t)((ratio - 1.0) / lsb_big);
    }
    else  {
       code = 0xFFFF;
    }
    return code;
}


void setReferenceAuto(void)
{
  
     for(int i = 0 ;i < measurementConfig.num_sensor; ++i)
    {
            if (measurementData.voltage[i] < 1.65)
            {
                measurementConfig.index_reference[i] = (measurementConfig.index_reference[i] - 1) % NUM_REFERENCE;
            }
            else if (measurementData.voltage[i] > 3) {
                measurementConfig.index_reference[i] = (measurementConfig.index_reference[i] + 1) % NUM_REFERENCE;
            }
 
    }  
  
    
}



/***************************************
 *  Command implementation
 ****************************************/
void setLED(void) {
    switch(measurementConfig.ledStatus) {
        /*
        case LED_OFF:
            Cy_GPIO_Write(LED_PORT, LED_NUM, 1);
            break;
        case LED_ON:
            Cy_GPIO_Write(LED_PORT, LED_NUM, 0);
            break;
        */
    }
}





void setOffset(void) {
    for(int i = 0;i < measurementConfig.num_sensor; ++i)
    {
        measurementData.offset[i] = measurementData.resistance[i]; 
    
    }    
}



/***************************************
 *  Communication implementatio 
 ****************************************/
void receiveRequest() {
    
    uint8_t rxBuffer[256] = {0};
    
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
            readUntilByte(&rxBuffer[TELEGRAM_BYTE_INDEX_ARGS_START], sizeof(rxBuffer), TELEGRAM_END_BYTE);
            measurementConfig.get_data_mode = GET_DATA_MODE_SINGLE;
            measurementConfig.delay_switch = ((uint16_t)rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_SWITCH_HIGH] << 8) | rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_SWITCH_LOW];
            measurementConfig.delay_meas = ((uint16_t)rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MEAS_HIGH] << 8) | rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MEAS_LOW];
            
            unsigned int telegramSize = getTelegramSize(rxBuffer, sizeof(rxBuffer));
            unsigned int idx = 0;
            for(unsigned int i = REQUEST_BYTE_INDEX_GET_DATA_SENSOR; i < telegramSize - 1; ++i) {             
                measurementConfig.input_channel[idx] = rxBuffer[i] >> 4;
                measurementConfig.output_channel[idx] = rxBuffer[i] & 0x0F;
                idx++;

            }
            measurementConfig.num_sensor = idx;
            

            break;
        }
            
            
        case TELEGRAM_TYPE_GET_CONTINUOUS_DATA:
        {
            readUntilByte(&rxBuffer[TELEGRAM_BYTE_INDEX_ARGS_START], sizeof(rxBuffer), TELEGRAM_END_BYTE);
            
            measurementConfig.get_data_mode = GET_DATA_MODE_CONTINUNOUS;
            measurementConfig.delay_switch = ((uint16_t)rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_SWITCH_HIGH] << 8) | rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_SWITCH_LOW];
            measurementConfig.delay_meas = ((uint16_t)rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MEAS_HIGH] << 8) | rxBuffer[REQUEST_BYTE_INDEX_GET_DATA_MEAS_LOW];
            
            unsigned int telegramSize = getTelegramSize(rxBuffer, sizeof(rxBuffer));
            unsigned int idx = 0;
            for(unsigned int i = REQUEST_BYTE_INDEX_GET_DATA_SENSOR; i < telegramSize - 1; ++i) {             
                measurementConfig.input_channel[idx] = rxBuffer[i] >> 4;
                measurementConfig.output_channel[idx] = rxBuffer[i] & 0x0F;
                idx++;

            }
            measurementConfig.num_sensor = idx;
            
            
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
        
        
        
        case TELEGRAM_TYPE_SET_RESISTANCE_CODING_FACTOR:
        {
            readUntilSize(&rxBuffer[TELEGRAM_BYTE_INDEX_ARGS_START], sizeof(rxBuffer), REQUEST_LENGTH_SET_RESISTANCE_CODING_FACTOR - 2);
            if(rxBuffer[REQUEST_LENGTH_SET_RESISTANCE_CODING_FACTOR - 1] != TELEGRAM_END_BYTE) {
                handleTelegramError();
                return;
            } 
            measurementConfig.resistance_coding_factor = rxBuffer[REQUEST_BYTE_INDEX_SET_RESISTANCE_CODING_FACTOR_FACTOR];
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
    uint8_t txBuffer[256] = {0};
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
            
            // For company
            txBuffer[TELEGRAM_BYTE_INDEX_START_BYTE] = TELEGRAM_START_BYTE;
            txBuffer[TELEGRAM_BYTE_INDEX_TYPE] = TELEGRAM_TYPE_GET_SINGLE_DATA;
            
            txBuffer[RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART1] = measurementData.timestamp >> 24;
            txBuffer[RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART2] = measurementData.timestamp >> 16;
            txBuffer[RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART3] = measurementData.timestamp >>  8;
            txBuffer[RESPONSE_BYTE_INDEX_GET_DATA_TIMESTAMP_PART4] = measurementData.timestamp      ;
            
            
            int idx = RESPONSE_BYTE_INDEX_GET_DATA_RESISTANCE_START;
            for(int i = 0; i < measurementConfig.num_sensor; ++i) {
                    uint16_t resistance_code = encodeResistance(measurementData.offset[i], measurementData.resistance[i], 10);
                    uint8_t resistance_code_low = resistance_code;
                    uint8_t resistance_code_high = resistance_code >> 8;
                    txBuffer[idx] = resistance_code_high;
                    idx++;
                    txBuffer[idx] = resistance_code_low;
                    idx++;
            }
            txBuffer[idx] = TELEGRAM_END_BYTE;
            write(txBuffer, getTelegramSize(txBuffer, sizeof(txBuffer)));
            break;
            
            
            
            /*
            // For Roman
            printf("%lu", (unsigned long)measurementData.timestamp);
            for(int i = 0; i < measurementConfig.num_sensor; ++i) {
                printf(",%.f", measurementData.resistance[i]);
            }
            printf("\r\n");
            break;
            */
            
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

unsigned int getTelegramSize(uint8_t* buffer, size_t size) {
    unsigned int telegram_size = size;
    for(unsigned int i = 0; i < size; ++i) {
        if(buffer[i] == 0 && telegram_size == size) telegram_size = i;
        else if(buffer[i] == 0 && telegram_size != size) continue;
        else telegram_size = size;
    }
    return telegram_size;
}


void write(uint8_t* buffer, size_t size) {
    Cy_SCB_UART_PutArrayBlocking(UART_HW, buffer, size);
    /* Blocking wait for transfer completion */
    while (!Cy_SCB_UART_IsTxComplete(UART_HW));
}


void sendDataDebug(void) {
    printf("%d,", measurementConfig.resistance_coding_factor);
    for(int i = 0; i < measurementConfig.num_sensor; ++i) {
        printf("%.f,", measurementData.resistance[i]);
        printf("%.d,", measurementConfig.index_reference[i]);
    }
    printf("\r\n");
}

/* [] END OF FILE */
