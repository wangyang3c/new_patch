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
 *         Function Prototypes
 ****************************************/
void Timer_Config(void);
void TimerInterruptHandler(void);
void sendData(void);
void readADC(void);
void SysTick_Callback();
void readUntil(char* buffer, size_t size, const char expected);
void readline(char* buffer, size_t size);
void write(char* buffer);
void receiveRequest(void);
void sendResponse(char* buffer, size_t size, char type);
void insertString(char* buffer, size_t size, unsigned int position, char* str);


/***************************************
 *            Constants
 ****************************************/
#define TELEGRAM_START '\x81'
#define TELEGRAM_END '\x7E'

#define TELEGRAM_TYPE_TEST '\x01'
#define TELEGRAM_TYPE_SET_LED '\x02'
#define TELEGRAM_TYPE_GET_SINGLE_SENSOR_DATA '\x03'
#define TELEGRAM_TYPE_GET_CONTINUOUS_SENSOR_DATA '\x04'
#define TELEGRAM_TYPE_SET_OFFSET_AUTO '\x05'
#define TELEGRAM_TYPE_SET_OFFSET_MANUAL '\x06'
#define TELEGRAM_TYPE_SET_GAIN '\x07'
#define TELEGRAM_TYPE_STOP_SENSOR_DATA '\x08'
#define TELEGRAM_TYPE_GET_SAMPLING_FREQUENCY '\x09'
#define TELEGRAM_TYPE_GET_TIMESTAMP '\x0A'

#define RESPONSE_TEST "Test\0"

/***************************************
 * Global variables
 ****************************************/
char command;
int timestamp;
float offset[8][8];
float v_patch[8][8];
int referenceChannel = 1;

struct {
    bool ledStatus;
    float offset;
    int reference;
    int samplingFrequency;
} setting;

struct {
    int timestamp;
    float voltages[8][8];
} data;



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
    AMux_1_Connect(8);

    AMux_2_Start();
    AMux_2_Connect(8);

    Timer_Start();
    Timer_Config();

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
        CyDelay(100);
    }
}

void Timer_Config(void)
{
    Cy_SysInt_Init(&isrTimer_cfg, TimerInterruptHandler);
    NVIC_ClearPendingIRQ(isrTimer_cfg.intrSrc);/* Clears the interrupt */
    NVIC_EnableIRQ(isrTimer_cfg.intrSrc); /* Enable the core interrupt */
    __enable_irq(); /* Enable global interrupts. */
    
    /* Start the TCPWM component in timer/counter mode. The return value of the
     * function indicates whether the arguments are valid or not. It is not used
     * here for simplicity. */
    (void)Cy_TCPWM_Counter_Init(Timer_HW, Timer_CNT_NUM, &Timer_config);
    Cy_TCPWM_Enable_Multiple(Timer_HW, Timer_CNT_MASK); /* Enable the counter instance */
    
    /* Set the timer period in milliseconds. To count N cycles, period should be
     * set to N-1. */
    Cy_TCPWM_Counter_SetPeriod(Timer_HW, Timer_CNT_NUM, 100000 - 1); //1000-1=1ms
    
    /* Trigger a software reload on the counter instance. This is required when 
     * no other hardware input signal is connected to the component to act as
     * a trigger source. */
    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK); 
}


void TimerInterruptHandler(void)
{
   
    /* Clear the terminal count interrupt */
    Cy_TCPWM_ClearInterrupt(Timer_HW, Timer_CNT_NUM, CY_TCPWM_INT_ON_TC);
    
    
    /*ADC task*/
    readADC();
    sendData();
    
}

void sendData(void)
{

        for(uint8 i = 0;i < 8;i++) {
            for(uint8 j = 0;j < 8;j++) {
                printf("%.3f,", v_patch[i][j]);
            }
        }
        printf("\r\n");
 
}

void readADC(void)
{

    for(uint8 i = 0;i < 8;i++)
    {
        AMux_1_FastSelect(i);
        for(uint8 j = 0; j < 8; j++)
        {
            AMux_2_FastSelect(j); 
            CyDelayUs(300);
            ADC_StartConvert();
            while (!ADC_IsEndConversion(CY_SAR_WAIT_FOR_RESULT));
            ADC_StopConvert();
            v_patch[i][j] = ADC_CountsTo_Volts(0, ADC_GetResult16(0)); 
        }  
    }

}

void SysTick_Callback() {
    timestamp++;
}


void receiveRequest() {
    
    char rxBuffer[64] = "";
    readUntil(rxBuffer, sizeof(rxBuffer),TELEGRAM_END);
    
    char requestStart = rxBuffer[0];
    char requestType = rxBuffer[1];
    
    if(requestStart == TELEGRAM_START) {
        char txBuffer[64] = "";
        switch(requestType) {
            case TELEGRAM_TYPE_TEST:
                sendResponse(txBuffer, sizeof(txBuffer), TELEGRAM_TYPE_TEST);
                break;
                
            case TELEGRAM_TYPE_SET_LED:
                break;
                
            case TELEGRAM_TYPE_GET_SINGLE_SENSOR_DATA:
                break;
                
            case TELEGRAM_TYPE_GET_CONTINUOUS_SENSOR_DATA:
                break;
                
            case TELEGRAM_TYPE_SET_OFFSET_AUTO:
                break;
                
            case TELEGRAM_TYPE_SET_OFFSET_MANUAL:
                break;

            case TELEGRAM_TYPE_STOP_SENSOR_DATA:
                break;
               
            case TELEGRAM_TYPE_GET_SAMPLING_FREQUENCY:
                break;
        }
    }
}


void sendResponse(char* buffer, size_t size, char type) {
    switch(type) {
        case TELEGRAM_TYPE_TEST: 
            buffer[0] = TELEGRAM_START;
            insertString(buffer, size, 1, RESPONSE_TEST);
            buffer[strlen(buffer)] = TELEGRAM_END;
            write(buffer);
            break;
    }
}




void readUntil(char* buffer, size_t size, const char expected) 
{   
    char temp[1];
    unsigned int i = 0;
    while(true)
    {   
        Cy_SCB_UART_GetArrayBlocking(UART_HW, temp, sizeof(temp));
        buffer[i] = temp[0];
        ++i; 
        if(temp[0] == expected || i == size) break;
    }        
}

void readline(char* buffer, size_t size) 
{   
    readUntil(buffer, size, '\n');
}

void write(char* buffer) {
    Cy_SCB_UART_PutArrayBlocking(UART_HW, buffer, strlen(buffer));
    /* Blocking wait for transfer completion */
    while (!Cy_SCB_UART_IsTxComplete(UART_HW));
}




void insertString(char* buffer, size_t size, unsigned int position, char* str) {
    for(unsigned int i = 0; i < strlen(str); ++i) {
        if(position + i == size) return;
        buffer[position + i] = str[i];
    }
}

/* [] END OF FILE */
