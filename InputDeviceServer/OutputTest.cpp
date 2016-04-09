#include <stdint.h>
#include <iostream>
#include "./DriverGPIO/gpio.h"
#include "myCommon.h"
#include "port.h"
#include "OutputTest.h"


#define GPIO_NUM 2

const static uint32_t GPIO_LED[GPIO_NUM] = {GPIO_LED_0, GPIO_LED_1};
static bool s_isInitialized = false;

void outputTest(uint32_t gpioNum, uint32_t out)
{
    if(s_isInitialized == false){
        if( gpioSetupGpio() == -1 ){
            std::cout << "error" << std::endl;
            return;
        }
        for(uint32_t i = 0; i < GPIO_NUM; i++)
            gpioPinMode(GPIO_LED[i], OUTPUT);
        s_isInitialized = true;
    }
    gpioDigitalWrite(GPIO_LED[gpioNum], out);
}


