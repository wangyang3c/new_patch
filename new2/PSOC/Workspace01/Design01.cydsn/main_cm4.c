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

/***************************************
 *            Constants
 ****************************************/
#define VOLTAGE_VCC 3.3
#define RESISTANCE_REFERENCE0 10e3  //kOhm
#define RESISTANCE_REFERENCE1 100e3
#define RESISTANCE_REFERENCE2 1000e3

/***************************************
 * Global variables
 ****************************************/
float v_patch_single[8];
float R_patch_single[8];
float R_patch_double[7][8];
int referenceChannel = 1;

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
    AMux_1_Connect(9);
    

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
        readADC();
        sendData();
        CyDelay(100);
    }
}



void sendData(void)
{
        int idx_R = 1;
        for(uint8 i = 0;i < 8;i++) {
            printf("%d:%.0f,", idx_R, R_patch_single[i]/1000);
            idx_R += 2;
        }

        int idx_R1 = 2;
        for(uint8 i = 0;i < 7;i++) {
            int idx_R2 = 1;
            for(uint8 j = 0;j < 8;j++) { 
                printf("%d-%d:%.0f,", idx_R1, idx_R2, R_patch_double[i][j]/1000);
                idx_R2 += 2;
            }
            idx_R1 += 2;
        }

        printf("\r\n");
 
}

void readADC(void)
{
    // Read single
    AMux_1_Connect(0);
    for(uint8 i = 0;i < 8;i++)
    {
            AMux_2_FastSelect(i); 
            CyDelayUs(500);
            ADC_StartConvert();
            while (!ADC_IsEndConversion(CY_SAR_WAIT_FOR_RESULT));
            ADC_StopConvert();
            v_patch_single[i] = ADC_CountsTo_Volts(0, ADC_GetResult16(0)); 
            R_patch_single[i] = v_patch_single[i] / (VOLTAGE_VCC - v_patch_single[i]) * RESISTANCE_REFERENCE1;
    }
    // Read double
    AMux_1_Disconnect(0);
    for(uint8 i = 0;i < 7;i++) {
        for(uint8 j = 0;j < 8;j++) {
            AMux_1_FastSelect(i+1); 
            AMux_2_FastSelect(j); 
            CyDelayUs(500);
            ADC_StartConvert();
            while (!ADC_IsEndConversion(CY_SAR_WAIT_FOR_RESULT));
            ADC_StopConvert();
            
            float v_patch_double = ADC_CountsTo_Volts(0, ADC_GetResult16(0)); 
            R_patch_double[i][j] = v_patch_double / (VOLTAGE_VCC - v_patch_double) * RESISTANCE_REFERENCE1;
        }
    
    }
    

}



/* [] END OF FILE */
