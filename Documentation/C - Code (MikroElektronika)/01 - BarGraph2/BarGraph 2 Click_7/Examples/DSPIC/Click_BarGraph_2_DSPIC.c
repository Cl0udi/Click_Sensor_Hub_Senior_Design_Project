/*
Example for BarGraph_2 Click

    Date          : mar 2018.
    Author        : MikroE Team

Test configuration dsPIC :
    
    MCU                : P33FJ256GP710A
    Dev. Board         : EasyPIC Fusion v7
    dsPIC Compiler ver : v7.0.1.0

---
Description :

The application is composed of three sections :

- System Initialization - GPIO and SPI module initialization    
- Application Initialization - SPI driver initialization
- Application Task - Turning all 10 LED ON sequentially on the bargraph 

*/

#include "Click_BarGraph_2_types.h"
#include "Click_BarGraph_2_config.h"

 
uint8_t i = 0;

void systemInit()
{
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_RST_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_PWM_PIN, _GPIO_OUTPUT );

    mikrobus_spiInit( _MIKROBUS1, &_BARGRAPH2_SPI_CFG[0] );


    Delay_ms( 100 );
}

void applicationInit()
{
    bargraph2_spiDriverInit( (T_BARGRAPH2_P)&_MIKROBUS1_GPIO, (T_BARGRAPH2_P)&_MIKROBUS1_SPI );
}

void applicationTask()
{

    for (i = 0; i < 11; i++)
    {
        bargraph2_set(i);
        Delay_ms(100);
    }

}

void main()
{
    systemInit();
    applicationInit();

    while (1)
    {
            applicationTask();
    }
}