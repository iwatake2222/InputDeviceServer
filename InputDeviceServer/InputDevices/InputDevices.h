/* 
 * File:   InputDevices.h
 * Author: takeshi
 *
 * Created on April 4, 2016, 8:06 PM
 */
#include <stdint.h>
#include "../myCommon.h"
#include "../port.h"

#ifndef INPUTDEVICES_H
#define INPUTDEVICES_H

class InputDevices {
    /*** Config and interface ***/
public:
    const static uint32_t MAX_KEY_NUM = 2;
    typedef struct {
        bool key[MAX_KEY_NUM];
        int32_t rotaryDegreeSpeed;
        int32_t rotaryDegreeSum;
        int32_t rotaryTickSpeed;
        int32_t rotaryTickSum;       
        int32_t accelX;
        int32_t accelY;
        int32_t accelZ;
        uint32_t accelTapNum;
    } INPUT_STATUS;
private:
    /* General */
    const static uint32_t MAX_NUM_CALLBACK = 4;
    /* Key */
    const static uint32_t GPIO_KEY0 = GPIO_INPUT_KEY_0;
    const static uint32_t GPIO_KEY1 = GPIO_INPUT_KEY_1;
    const static uint32_t SLEEP_TIME_KEY_MS = 20;
    const static uint32_t CHATTERING_CHECK_TIMES = 1;
    /* Rotary Encoder */
    const static uint32_t ROTARY_TICK_DEGREE = 20;
    const static uint32_t GPIO_ROTARY_A = GPIO_INPUT_ROTARY_ENCODER_A;
    const static uint32_t GPIO_ROTARY_B = GPIO_INPUT_ROTARY_ENCODER_B;
    const static uint32_t SLEEP_TIME_ROTARY_MS = 3;
    const static uint32_t CHATTERING_ROTARY_DELAY_MS = 1;
    const static uint32_t ROTARY_SPEED_MEASURE_INTERVAL_TIMES = 10;
     /* Accelerometer (ADXL345) */
    const static uint32_t ACCEL_I2C_ADDR = I2C_ADDR_ADXL345;
    const static uint32_t SLEEP_TIME_ACCEL_MS = 50;
    
    /*** Methods ***/
public:
    static InputDevices* getInstance();
    RET start();
    RET stopRendezvous();
    RET getInputStatus(INPUT_STATUS* inputStatus);
    RET registerKeyChangedCallback(void (*func)(INPUT_STATUS *inputStatus));
    RET registerRotaryEncoderChangedCallback(void (*func)(INPUT_STATUS *inputStatus));
    RET registerTappedCallback(void (*func)(INPUT_STATUS *inputStatus));
    RET unregisterKeyChangedCallback(void (*func)(INPUT_STATUS *inputStatus));
    RET unregisterRotaryEncoderChangedCallback(void (*func)(INPUT_STATUS *inputStatus));
    RET unregisterTappedCallback(void (*func)(INPUT_STATUS *inputStatus));
    
private:
    InputDevices();
    InputDevices(const InputDevices& orig);
    virtual ~InputDevices();
    void* threadInputKeys();
    void* threadInputRotary();   
    void* threadInputAccel();  
    static void* threadInputKeysWrapper(void* pParam);
    static void* threadInputRotaryWrapper(void* pParam);
    static void* threadInputAccelWrapper(void* pParam);
    
    /*** Attributes ***/
private:
    INPUT_STATUS m_inputStatus;
    bool m_isExit;    
    pthread_t m_tidInputKeys;
    pthread_t m_tidInputRotary;
    pthread_t m_tidInputAccel;
    pthread_mutex_t m_mutex;
    void (*m_funcKeyChangedCallback[MAX_NUM_CALLBACK])(INPUT_STATUS *inputStatus);
    void (*m_funcRotaryChangedCallback[MAX_NUM_CALLBACK])(INPUT_STATUS *inputStatus);
    void (*m_funcTappedCallback[MAX_NUM_CALLBACK])(INPUT_STATUS *inputStatus);
};

#endif /* INPUTDEVICES_H */
