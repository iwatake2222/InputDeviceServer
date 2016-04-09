/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Application.h
 * Author: takeshi
 *
 * Created on April 7, 2016, 2:17 AM
 */
#include <stdint.h>
#include <pthread.h>
#include "./DriverLcdST7735R/DriverLcdST7735R.h"
#include "./DriverLcdST7735R/DriverLcdST7735RText.h"
#include "./DriverOledSSD1306/DriverOledSSD1306.h"
#include "./DriverOledSSD1306/DriverOledSSD1306Text.h"
#include "./InputDevices/InputDevices.h"

#ifndef APPLICATION_H
#define APPLICATION_H

class Application {
    /*** Config and interface ***/
private:
    const static uint32_t SLEEP_TIME_MS = 20;
    /*** Methods ***/
public:
    static Application* getInstance();
    RET start();
    RET stopRendezvous();
private:
    Application();
    Application(const Application& orig);
    virtual ~Application();
    static void* threadMainLoopWrapper(void* pParam);
    void* threadMainLoop();  
    void initDriverLcdST7735R();
    void initDriverOledSSD1306();
    void inputKeyCB (InputDevices::INPUT_STATUS *inputStatus);
    void inputRotaryCB (InputDevices::INPUT_STATUS *inputStatus);
    void inputTapCB (InputDevices::INPUT_STATUS *inputStatus);
    static void inputKeyCBWrapper (InputDevices::INPUT_STATUS *inputStatus);
    static void inputRotaryCBWrapper (InputDevices::INPUT_STATUS *inputStatus);
    static void inputTapCBWrapper (InputDevices::INPUT_STATUS *inputStatus);
    void printLcd(const char *str, uint32_t x, uint32_t y);
    /*** Attributes ***/
private:
    bool m_isExit;
    pthread_t m_tidMainLoop;
    pthread_mutex_t m_mutexPrint;
    DriverLcdST7735R* m_driverLcdST7735R;
    DriverLcdST7735RText* m_driverLcdST7735RText;
    DriverOledSSD1306* m_driverOledSSD1306;
    DriverOledSSD1306Text* m_driverOledSSD1306Text;

};

#endif /* APPLICATION_H */


// Callback functions for Input
