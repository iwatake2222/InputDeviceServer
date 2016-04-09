/* 
 * File:   DriverOledSSD1306.h
 * Author: takeshi
 *
 * Created on April 6, 2016, 10:34 PM
 */

#include <stdint.h>
#include "../myCommon.h"

#ifndef DRIVEROLEDSSD1306_H
#define DRIVEROLEDSSD1306_H

class DriverOledSSD1306 {
    /*** Config and interface ***/
public:
    const static uint32_t DEFAULT_WIDTH  = 128;
    const static uint32_t DEFAULT_HEIGHT = 64;
    static const uint8_t  CONTROL_COMMAND = 0x00;
    static const uint8_t  CONTROL_DATA    = 0x40;
private:
    static const uint32_t I2C_SPEED = 400*1000;
    static const uint32_t PAGE_SIZE = 8;
    
    /*** Methods ***/
public:
    DriverOledSSD1306(uint8_t i2cAddress, uint32_t width, uint32_t height);
    DriverOledSSD1306(const DriverOledSSD1306& orig);
    virtual ~DriverOledSSD1306(); 
    RET init();
    void setAddrWindowFull();
    void setAddrWindow(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
    void fillRect(uint16_t color, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
    void drawBuffer(uint8_t *buffer, uint32_t size);        // buffer[0] need to contain CONTROL_XXX
    void drawBufferWithControl(uint8_t* buffer, uint32_t size); // memcpy occurs
public:
    void sendCommand(uint8_t cmd);
    void sendData(uint8_t data);
    
    /*** Attributes ***/
private:
    uint8_t  m_i2cAddress;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_pageNum;
    uint32_t m_device;


};

#endif /* DRIVEROLEDSSD1306_H */

