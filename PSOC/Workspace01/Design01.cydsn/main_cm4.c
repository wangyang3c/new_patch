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

/***************************************
 *         Function Prototypes
 ****************************************/
void sendData(void);
void readADC(void);
void SysTick_Callback();
void readline(char* buffer);
void receiveCommand(void);
void setOptimalReference(void);

/***************************************
 *            Constants
 ****************************************/
#define TELEGRAM_HEADER 0x81
#define TELEGRAM_FOOTER 0x7E

#define TELEGRAM_TYPE_TEST 0x01
#define TELEGRAM_TYPE_SET_LED 0x02
#define TELEGRAM_TYPE_GET_SINGLE_SENSOR_DATA 0x03
#define TELEGRAM_TYPE_GET_CONTINUOUS_SENSOR_DATA 0x04
#define TELEGRAM_TYPE_SET_OFFSET_AUTO 0x05
#define TELEGRAM_TYPE_SET_OFFSET_MANUAL 0x06
#define TELEGRAM_TYPE_SET_GAIN 0x07
#define TELEGRAM_TYPE_STOP_SENSOR_DATA 0x08
#define TELEGRAM_TYPE_GET_SAMPLING_FREQUENCY 0x09
#define TELEGRAM_TYPE_GET_TIMESTAMP 0x0A



/***************************************
 * Global variables
 ****************************************/
char command;
int timestamp;
float offset = 0.0;
float v_patch[8][8];
int referenceChannel = 1;

bool send = false;

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
            receiveCommand();
        }    
        if(send) {
            readADC();
            //sendData();
        }

        CyDelay(100);
    }
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

void readline(char* buffer) 
{   
    char temp[1];
    int i = 0;
    while(true)
    {   
        Cy_SCB_UART_GetArrayBlocking(UART_HW, temp, 1);
        buffer[i] = temp[0];
        ++i; 
        
        
        if(temp[0] == '\n')  {

            break;
        }
        //CyDelayUs(1000);
        // byte duration of 115200 is 86.806 µs https://lucidar.me/en/serialib/most-used-baud-rates-table/
    }
              
}

void receiveCommand() {
    
    char buffer[64];
    readline(buffer);
    if(buffer[0]=='0') printf("666");
}


/* [] END OF FILE */
