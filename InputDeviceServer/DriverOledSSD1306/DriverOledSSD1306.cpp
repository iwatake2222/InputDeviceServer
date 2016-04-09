/* 
 * File:   DriverOledSSD1306.cpp
 * Author: takeshi
 * 
 * Created on April 6, 2016, 10:34 PM
 */

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include "../myCommon.h"
#include "../DriverI2C/i2c.h"
#include "oledSSD1306_CMD.h"
#include "DriverOledSSD1306.h"

DriverOledSSD1306::DriverOledSSD1306(uint8_t i2cAddress, uint32_t width, uint32_t height) {
    m_i2cAddress = i2cAddress;
    m_width = width;
    m_height = height;
    m_pageNum = height / PAGE_SIZE;
}

DriverOledSSD1306::DriverOledSSD1306(const DriverOledSSD1306& orig) {
}

DriverOledSSD1306::~DriverOledSSD1306() {
}

void DriverOledSSD1306::drawBuffer(uint8_t* buffer, uint32_t size) {
    i2cWriteBuffer(m_device, buffer, size);
}

void DriverOledSSD1306::drawBufferWithControl(uint8_t* buffer, uint32_t size) {
    i2cWriteRegBuffer(m_device, CONTROL_DATA, buffer, size);
}

void DriverOledSSD1306::fillRect(uint16_t color, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) {
    uint32_t width = x1 - x0;
    uint32_t yStart = y0/PAGE_SIZE;
    uint32_t yEnd = y1/PAGE_SIZE;

    setAddrWindow(x0, y0, x1, y1);
    uint8_t data = color==0?0:0xff;
    for(uint8_t x = x0; x < x1; x++){
        for(uint8_t y = yStart; y < yEnd; y++){			
            sendData(data);
        }
    }
    setAddrWindowFull();
}

RET DriverOledSSD1306::init() {
    
    m_device = i2cSetup(m_i2cAddress);
    if(m_device == -1) {
        //cout << "err" << endl;
        return RET_ERR;
    }

    /* init sequence is from datasheet */
    sendCommand(SSD1306_DISPLAYOFF);
    sendCommand(SSD1306_SETMULTIPLEX);
    sendCommand(0x3F);
    sendCommand(SSD1306_SETDISPLAYOFFSET);
    sendCommand(0x00);
    sendCommand(SSD1306_SETSTARTLINE | 0x00);
    sendCommand(SSD1306_SEGREMAP | 0x01);
    sendCommand(SSD1306_COMSCANDEC);
    sendCommand(SSD1306_SETCOMPINS);
    sendCommand(0x12);
    sendCommand(SSD1306_SETCONTRAST);
    sendCommand(0xCF);
    sendCommand(SSD1306_DISPLAYALLON_RESUME);
    sendCommand(SSD1306_NORMALDISPLAY);
    sendCommand(SSD1306_SETDISPLAYCLOCKDIV);
    sendCommand(0x80);
    sendCommand(SSD1306_CHARGEPUMP);
    sendCommand(0x14);
    sendCommand(SSD1306_MEMORYMODE);
    sendCommand(0x01);	// vertical ddressing mode
    sendCommand(SSD1306_DISPLAYON);

    setAddrWindowFull();
    fillRect(0, 0, 0, m_width, m_height);

}

void DriverOledSSD1306::sendData(uint8_t data) {
    //i2cWrite(m_device, CONTROL_DATA);
    //i2cWrite(m_device, data);
    i2cWriteReg8(m_device, CONTROL_DATA, data);
}

void DriverOledSSD1306::sendCommand(uint8_t cmd) {
    //i2cWrite(m_device, CONTROL_COMMAND);
    //i2cWrite(m_device, cmd);
    i2cWriteReg8(m_device, CONTROL_COMMAND, cmd);
}


void DriverOledSSD1306::setAddrWindow(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) {
    sendCommand(SSD1306_COLUMNADDR);
    sendCommand(x0);
    sendCommand(x1 - 1);
    sendCommand(SSD1306_PAGEADDR);
    sendCommand(y0/PAGE_SIZE);
    sendCommand(y1/PAGE_SIZE - 1);
}

void DriverOledSSD1306::setAddrWindowFull() {
    setAddrWindow(0, 0, m_width, m_height);
}

