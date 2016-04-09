/* 
 * File:   Application.cpp
 * Author: takeshi
 * 
 * Created on April 7, 2016, 2:17 AM
 */
#include <stdint.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "Application.h"

using namespace std;

Application* Application::getInstance() {
    static Application s_application;
    return &s_application;
}

Application::Application() {
    m_isExit = false;
}

Application::Application(const Application& orig) {
}

Application::~Application() {
}

RET Application::start() {
    // Initialize Mutex
    pthread_mutex_init(&m_mutexPrint, NULL);      

    // Initialize Displays
    initDriverLcdST7735R();
    initDriverOledSSD1306();
    printLcd("Hello", 0, 0);
    usleep(1*1000*1000);
    
    // Initialize and start Input
    InputDevices *inputDevices = InputDevices::getInstance();    
    inputDevices->registerKeyChangedCallback(inputKeyCBWrapper);
    inputDevices->registerRotaryEncoderChangedCallback(inputRotaryCBWrapper);
    inputDevices->registerTappedCallback(inputTapCBWrapper);
    inputDevices->start();
    
    // Start main loop
    pthread_create(&m_tidMainLoop, NULL, threadMainLoopWrapper, this);
}

RET Application::stopRendezvous() {
    InputDevices::getInstance()->stopRendezvous();
    
    m_isExit = true;
    pthread_join(m_tidMainLoop,NULL);
    pthread_mutex_destroy(&m_mutexPrint);    
    
    delete m_driverLcdST7735R;
    delete m_driverLcdST7735RText;
    delete m_driverOledSSD1306;
    delete m_driverOledSSD1306Text;
    pthread_mutex_destroy(&m_mutexPrint);
    return RET_OK;
}

void* Application::threadMainLoopWrapper(void* pParam) {
    Application *p = (Application *)pParam;
    p->threadMainLoop();
}

void* Application::threadMainLoop() {
    InputDevices *inputDevices = InputDevices::getInstance();    
    InputDevices::INPUT_STATUS inputStatus;
    while(1) {
        // Loop for check Accel (should create thread)
        inputDevices->getInputStatus(&inputStatus);
        char buffer[30];
        //snprintf(buffer, sizeof(buffer), "X: %03d, Y: %03d, Z: %03d", inputStatus.accelX, inputStatus.accelY, inputStatus.accelZ);
        snprintf(buffer, sizeof(buffer), "XY: %03d %03d", inputStatus.accelX, inputStatus.accelY);
        printLcd(buffer , 0, 3);
        usleep(20*1000);
        
        usleep(SLEEP_TIME_MS*1000);
        if(m_isExit) break;    
    }
    
}


void Application::printLcd(const char *str, uint32_t x, uint32_t y)
{
    pthread_mutex_lock(&m_mutexPrint);
    m_driverLcdST7735RText->print(str , x, y);
    m_driverOledSSD1306Text->print(str , x, y);    
    pthread_mutex_unlock(&m_mutexPrint);
}

void Application::inputKeyCBWrapper(InputDevices::INPUT_STATUS* inputStatus) {
    Application::getInstance()->inputKeyCB(inputStatus);
}

void Application::inputRotaryCBWrapper(InputDevices::INPUT_STATUS* inputStatus) {
    Application::getInstance()->inputRotaryCB(inputStatus);
}

void Application::inputTapCBWrapper(InputDevices::INPUT_STATUS* inputStatus) {
    Application::getInstance()->inputTapCB(inputStatus);
}


void Application::inputKeyCB (InputDevices::INPUT_STATUS *inputStatus){
    cout << "Keys: " << inputStatus->key[0] << ", " << inputStatus->key[1] << endl;
    
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "Keys : %d %d", inputStatus->key[0], inputStatus->key[1]);
    printLcd(buffer , 0, 0);
}

void Application::inputRotaryCB (InputDevices::INPUT_STATUS *inputStatus){
    cout << "Rotate: " << inputStatus->rotaryTickSpeed << endl;
    
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "Rot  : %04d", inputStatus->rotaryTickSum);
    printLcd(buffer , 0, 1);
}

void Application::inputTapCB (InputDevices::INPUT_STATUS *inputStatus){
    cout << "Tap: " << inputStatus->accelTapNum << endl;
            
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "Tap  : %d", inputStatus->accelTapNum);
    printLcd(buffer , 0, 2);
}


void Application::initDriverLcdST7735R() {
    const uint32_t spiChannel = SPI_LCD_ST7735R;
    const uint32_t gpioRS     = GPIO_LCD_ST7735R_RS;
    const uint32_t width      = DriverLcdST7735R::DEFAULT_WIDTH;
    const uint32_t height     = DriverLcdST7735R::DEFAULT_HEIGHT;
    m_driverLcdST7735R = new DriverLcdST7735R(spiChannel, gpioRS, width, height, DriverLcdST7735R::LCD_ST7735R_ROTATE_180);    
    m_driverLcdST7735R->init();    
    m_driverLcdST7735R->fillRect(0x0000, 0, 0, width, height);    
    m_driverLcdST7735RText = new DriverLcdST7735RText(m_driverLcdST7735R);
    m_driverLcdST7735RText->setColor(DriverLcdST7735R::COLOR_RED, DriverLcdST7735R::COLOR_BLUE);
    m_driverLcdST7735RText->setTextArea(0, 0, width, height);   
}

void Application::initDriverOledSSD1306() {
    const uint8_t  i2cAddress = I2C_ADDR_OLED_SSD1306;
    const uint32_t width      = DriverOledSSD1306::DEFAULT_WIDTH;
    const uint32_t height     = DriverOledSSD1306::DEFAULT_HEIGHT;
    m_driverOledSSD1306 = new DriverOledSSD1306(i2cAddress, width, height);    
    m_driverOledSSD1306->init();    
    m_driverOledSSD1306->fillRect(0, 0, 0, width, height);
    m_driverOledSSD1306Text = new DriverOledSSD1306Text(m_driverOledSSD1306);
    m_driverOledSSD1306Text->setColor(0, 1);
    m_driverOledSSD1306Text->setTextArea(0, 0, width, height);
}
