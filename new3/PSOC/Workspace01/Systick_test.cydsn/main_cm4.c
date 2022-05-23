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

void SysTick_Callback();

int timestamp;

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Cy_SysTick_Init(CY_SYSTICK_CLOCK_SOURCE_CLK_CPU , 100000); //100MHz
    Cy_SysTick_SetCallback(0,SysTick_Callback);
    UART_Start();
    
    for(;;)
    {
        /* Place your application code here. */
        printf("%d\n", timestamp);
        CyDelay(10);
    }
}

void SysTick_Callback() {
    timestamp++;
}
/* [] END OF FILE */
