/* 
 * File:   InputDevices.cpp
 * Author: takeshi
 * 
 * Created on April 4, 2016, 8:06 PM
 */
#include <stdint.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "../DriverGPIO/gpio.h"
#include "../DriverI2C/i2c.h"
#include "InputDevices.h"

using namespace std;

InputDevices::InputDevices() {  
    m_isExit = false;
    memset(&m_inputStatus, 0, sizeof(m_inputStatus));
    memset(m_funcKeyChangedCallback, 0, sizeof(m_funcKeyChangedCallback));
    memset(m_funcRotaryChangedCallback, 0, sizeof(m_funcRotaryChangedCallback));
    memset(m_funcTappedCallback, 0, sizeof(m_funcTappedCallback));
}

InputDevices::InputDevices(const InputDevices& orig) {
}

InputDevices::~InputDevices() {
}

InputDevices* InputDevices::getInstance() {
    static InputDevices s_inputDevices;
    return &s_inputDevices;
}

RET InputDevices::start() {
    m_isExit = false;
    if( gpioSetupGpio() == -1 ) return RET_ERR;    
    pthread_mutex_init(&m_mutex, NULL);    
    pthread_create(&m_tidInputKeys, NULL, threadInputKeysWrapper, this);
    pthread_create(&m_tidInputRotary, NULL, threadInputRotaryWrapper, this);
    pthread_create(&m_tidInputAccel, NULL, threadInputAccelWrapper, this);
    // wait until status fixed
    usleep(SLEEP_TIME_KEY_MS*(CHATTERING_CHECK_TIMES+1)*1000);
    return RET_OK;
}

RET InputDevices::stopRendezvous() {
    m_isExit = true;    
    pthread_join(m_tidInputKeys,NULL);
    pthread_join(m_tidInputRotary,NULL);
    pthread_join(m_tidInputAccel,NULL);
    pthread_mutex_destroy(&m_mutex);    
    return RET_OK;
}

RET InputDevices::getInputStatus(INPUT_STATUS* inputStatus){
    //pthread_mutex_lock(&m_mutex);
    memcpy( inputStatus, &m_inputStatus, sizeof(m_inputStatus));
    //pthread_mutex_unlock(&m_mutex);
    return RET_OK;
}

RET InputDevices::registerKeyChangedCallback(void (*func)(INPUT_STATUS *inputStatus)) {
    uint32_t i;
    for( i = 0; i < MAX_NUM_CALLBACK; i++ ){
        if( m_funcKeyChangedCallback[i] == NULL) {
            m_funcKeyChangedCallback[i] = func;
            break;
        }
    }
    if( i == MAX_NUM_CALLBACK )
        return RET_ERR_REGISTER;
    return RET_OK;
}

RET InputDevices::registerRotaryEncoderChangedCallback(void (*func)(INPUT_STATUS *inputStatus)) {
    uint32_t i;
    for( i = 0; i < MAX_NUM_CALLBACK; i++ ){
        if( m_funcRotaryChangedCallback[i] == NULL) {
            m_funcRotaryChangedCallback[i] = func;
            break;
        }
    }
    if( i == MAX_NUM_CALLBACK )
        return RET_ERR_REGISTER;
    return RET_OK;
}

RET InputDevices::registerTappedCallback(void (*func)(INPUT_STATUS *inputStatus)) {
    uint32_t i;
    for( i = 0; i < MAX_NUM_CALLBACK; i++ ){
        if( m_funcTappedCallback[i] == NULL) {
            m_funcTappedCallback[i] = func;
            break;
        }
    }
    if( i == MAX_NUM_CALLBACK )
        return RET_ERR_REGISTER;
    return RET_OK;
}

RET InputDevices::unregisterKeyChangedCallback(void (*func)(INPUT_STATUS *inputStatus)) {
    uint32_t i;
    for( i = 0; i < MAX_NUM_CALLBACK; i++ ){
        if( m_funcKeyChangedCallback[i] == func) {
            m_funcKeyChangedCallback[i] = NULL;
            break;
        }
    }
    if( i == MAX_NUM_CALLBACK )
        return RET_ERR_PRM;
    return RET_OK;
}

RET InputDevices::unregisterRotaryEncoderChangedCallback(void (*func)(INPUT_STATUS *inputStatus)) {
    uint32_t i;
    for( i = 0; i < MAX_NUM_CALLBACK; i++ ){
        if( m_funcRotaryChangedCallback[i] == func) {
            m_funcRotaryChangedCallback[i] = NULL;
            break;
        }
    }
    if( i == MAX_NUM_CALLBACK )
        return RET_ERR_PRM;
    return RET_OK;
}

RET InputDevices::unregisterTappedCallback(void (*func)(INPUT_STATUS *inputStatus)) {
    uint32_t i;
    for( i = 0; i < MAX_NUM_CALLBACK; i++ ){
        if( m_funcTappedCallback[i] == func) {
            m_funcTappedCallback[i] = NULL;
            break;
        }
    }
    if( i == MAX_NUM_CALLBACK )
        return RET_ERR_PRM;
    return RET_OK;
}

void* InputDevices::threadInputKeysWrapper(void* pParam) {
    InputDevices *p = (InputDevices *)pParam;
    p->threadInputKeys();
}

void* InputDevices::threadInputRotaryWrapper(void* pParam) {
    InputDevices *p = (InputDevices *)pParam;
    p->threadInputRotary(); 
}

void* InputDevices::threadInputAccelWrapper(void* pParam) {
    InputDevices *p = (InputDevices *)pParam;
    p->threadInputAccel(); 
}


void* InputDevices::threadInputKeys() {
    uint32_t keys[] = {
        GPIO_KEY0,
        GPIO_KEY1,
    };

    uint32_t chattering[MAX_KEY_NUM] = {};
    
    /* Init GPIO for input keys */
    for( uint32_t i = 0; i < MAX_KEY_NUM; i++ ){
        gpioPinMode( keys[i], INPUT);
        gpioPullUpDnControl(keys[i], PUD_UP);        
    }

    while(1){
        for( uint32_t i = 0; i < MAX_KEY_NUM; i++ ){
            bool current = false;
            if(gpioDigitalRead(keys[i]) != 0) current = true;
            if(m_inputStatus.key[i] != current) {
                chattering[i]++;
                if(chattering[i] > CHATTERING_CHECK_TIMES) {
                    // reach here when key is really pushed
                    m_inputStatus.key[i] = current;
                    chattering[i] = 0;
                    // call registered callback functions
                    for( uint32_t i = 0; i < MAX_NUM_CALLBACK; i++ ){
                        if( m_funcKeyChangedCallback[i] != NULL) {
                            m_funcKeyChangedCallback[i](&m_inputStatus);
                        }                        
                    }
                }                 
            } else {
                chattering[i] = 0;
            }
        }
        
        usleep(SLEEP_TIME_KEY_MS*1000);
        if(m_isExit) break;
    }
}


void* InputDevices::threadInputRotary() {   
    // code(FW) = 0 1 3 2 0 1 3 2 0 1 3 2
    /* Init GPIO for input keys */
    gpioPinMode( GPIO_ROTARY_A, INPUT );
    gpioPinMode( GPIO_ROTARY_B, INPUT );
    gpioPullUpDnControl( GPIO_ROTARY_A, PUD_UP );
    gpioPullUpDnControl( GPIO_ROTARY_B, PUD_UP );

    uint32_t cntMeasureSpeed = 0;
    int32_t previousRotaryTickSum = 0;    // for rotation speed measurement
    
    uint32_t previousCode = (gpioDigitalRead(GPIO_ROTARY_A)==0?0:1<<1) | (gpioDigitalRead(GPIO_ROTARY_B)==0?0:1<<0);    
    while(1){
        uint32_t currentCode;
        currentCode = (gpioDigitalRead(GPIO_ROTARY_A)==0?0:1<<1) | (gpioDigitalRead(GPIO_ROTARY_B)==0?0:1<<0);
                
	if( currentCode != previousCode  ){
            usleep(CHATTERING_ROTARY_DELAY_MS*1000);    // for chattering
            currentCode = (gpioDigitalRead(GPIO_ROTARY_A)==0?0:1<<1) | (gpioDigitalRead(GPIO_ROTARY_B)==0?0:1<<0);
            if( currentCode != previousCode  ){                
                uint32_t code = previousCode << 2 | currentCode;
                previousCode = currentCode;
                static const int rotationTable[] = { 0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0 };
                int8_t rotation = rotationTable[code & 0x0f];
                m_inputStatus.rotaryTickSum += rotation;
                m_inputStatus.rotaryDegreeSum += rotation*ROTARY_TICK_DEGREE;
            }
	}
        
        // for speed measurement
        if( cntMeasureSpeed++ >= ROTARY_SPEED_MEASURE_INTERVAL_TIMES ) {
            cntMeasureSpeed = 0;
            int32_t diff = m_inputStatus.rotaryTickSum - previousRotaryTickSum;
            previousRotaryTickSum = m_inputStatus.rotaryTickSum;
            m_inputStatus.rotaryTickSpeed = diff * (1000 / (ROTARY_SPEED_MEASURE_INTERVAL_TIMES*SLEEP_TIME_ROTARY_MS));
            m_inputStatus.rotaryDegreeSpeed = diff * (ROTARY_TICK_DEGREE * 1000 / (ROTARY_SPEED_MEASURE_INTERVAL_TIMES*SLEEP_TIME_ROTARY_MS));
            if( diff != 0 ){                    
                //cout << m_inputStatus.rotaryTickSpeed << endl;
                // call registered callback functions
                for( uint32_t i = 0; i < MAX_NUM_CALLBACK; i++ ){
                    if( m_funcRotaryChangedCallback[i] != NULL) {
                        m_funcRotaryChangedCallback[i](&m_inputStatus);
                    }
                }
            }            
        }

        usleep(SLEEP_TIME_ROTARY_MS*1000);
        if(m_isExit) break;
    }
}


void* InputDevices::threadInputAccel() {   
    int32_t fd = i2cSetup(ACCEL_I2C_ADDR);
    if(fd == -1) {
        cerr << "err err" << endl;
        return 0;
    }
    
    //uint32_t version = i2cReadReg8(fd, 0x00);
    //cout << version << endl;
    
    /* init */
    i2cWriteReg8(fd, 0x2d, 0x00);
    // range = +-2g, Left Justify, 10-bit mode(but will ignore tha right 2-bit)
    i2cWriteReg8(fd, 0x31, 0x04);
    // setings for tap (values are from data sheet)
    i2cWriteReg8(fd, 0x1d, 0x60);
    i2cWriteReg8(fd, 0x21, 0x20);
    i2cWriteReg8(fd, 0x22, 0x20);
    i2cWriteReg8(fd, 0x23, 0x80);
    // enable tap detection
    i2cWriteReg8(fd, 0x2a, 0x07);
    i2cWriteReg8(fd, 0x2e, 0x60);		
    // start measurement
    i2cWriteReg8(fd, 0x2d, 0x08);

    while(1){
        uint8_t data;
        data = i2cReadReg8(fd, 0x30);
	if(data&0x20) {
            m_inputStatus.accelTapNum = 2;
	} else if(data&0x40) {		
            m_inputStatus.accelTapNum = 1;
	} else {
            // clear the status when user read
            //m_inputStatus.accelTapNum = 0;
	}
        // call registered callback functions
        if(m_inputStatus.accelTapNum != 0){
            for( uint32_t i = 0; i < MAX_NUM_CALLBACK; i++ ){
                if( m_funcTappedCallback[i] != NULL) {
                    m_funcTappedCallback[i](&m_inputStatus);
                }
            }
            m_inputStatus.accelTapNum = 0;
        }
        // +-64
        m_inputStatus.accelX = (int8_t)i2cReadReg8(fd, 0x33);
        m_inputStatus.accelY = (int8_t)i2cReadReg8(fd, 0x35);
        m_inputStatus.accelZ = (int8_t)i2cReadReg8(fd, 0x37);
        
        usleep(SLEEP_TIME_ACCEL_MS*1000);
        if(m_isExit) break;
    }
}
