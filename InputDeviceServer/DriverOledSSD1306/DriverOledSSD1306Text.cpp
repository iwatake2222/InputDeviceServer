/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DriverOledSSD1306Text.cpp
 * Author: takeshi
 * 
 * Created on April 6, 2016, 11:40 PM
 */
#include <stdlib.h>
#include "../font//font.h"
#include "DriverOledSSD1306Text.h"

DriverOledSSD1306Text::DriverOledSSD1306Text(DriverOledSSD1306 * driverOledSSD1306) {
    m_driverOledSSD1306 = driverOledSSD1306;
}

//DriverOledSSD1306Text::DriverOledSSD1306Text(const DriverOledSSD1306Text& orig) {
//}

DriverOledSSD1306Text::~DriverOledSSD1306Text() {
}

void DriverOledSSD1306Text::putchar(char c) {
    uint32_t endX = m_currentPosX + FONT_WIDTH * FONT_DISPLAY_SIZE;
    uint32_t endY = m_currentPosY + FONT_HEIGHT * FONT_DISPLAY_SIZE;
    if( endX > m_areaX1 ) {
        m_currentPosX = m_areaX0;
        endX = m_currentPosX + FONT_WIDTH * FONT_DISPLAY_SIZE;
        m_currentPosY += FONT_HEIGHT * FONT_DISPLAY_SIZE;
        endY = m_currentPosY + FONT_HEIGHT * FONT_DISPLAY_SIZE;
        if( endY > m_areaY1 ) {
            m_currentPosY = m_areaY0;
            endY = m_currentPosY + FONT_HEIGHT * FONT_DISPLAY_SIZE;
        }
        m_driverOledSSD1306->fillRect(m_backgroundColor, m_areaX0, m_currentPosY, m_areaX1, endY);
    }

    m_driverOledSSD1306->setAddrWindow(m_currentPosX, m_currentPosY, endX, endY);
    drawChar(c);

    m_currentPosX = endX;
}

void DriverOledSSD1306Text::drawChar(char c) {
    uint8_t* buffer = (uint8_t*)malloc(FONT_WIDTH*FONT_DISPLAY_SIZE*FONT_HEIGHT*FONT_DISPLAY_SIZE/8 + 1);
    uint8_t* bufferOrg = buffer;
    *buffer++ = DriverOledSSD1306::CONTROL_DATA;
    for (uint8_t i = 0; i < FONT_WIDTH; i++) {
        for (uint8_t sizeX = 0; sizeX < FONT_DISPLAY_SIZE; sizeX++) {
            uint8_t line = *(GET_FONT() + (c * FONT_WIDTH) + i);
            uint8_t data = 0;
            uint8_t dataDigit = 0;
            for (uint8_t lineDigit = 0; lineDigit < 8;) {
                for (uint8_t sizeY = 0; sizeY < FONT_DISPLAY_SIZE; sizeY++) {
                    dataDigit++;
                    data >>= 1;
                    if (line & 0x01) data += 0x80;
                }
                line >>= 1;
                lineDigit++;
                if (dataDigit == 8) {
                    //m_driverOledSSD1306->sendData(data);
                    //m_driverOledSSD1306->drawBuffer(&data, 1);
                    *buffer++ = data;
                    data = 0;
                    dataDigit = 0;
                }
            }
        }
    }
    m_driverOledSSD1306->drawBuffer(bufferOrg, FONT_WIDTH*FONT_DISPLAY_SIZE*FONT_HEIGHT*FONT_DISPLAY_SIZE/8 + 1);
    free(bufferOrg);
}

void DriverOledSSD1306Text::loadCurPos() {
    m_savedPosX = m_currentPosX;
    m_savedPosY = m_currentPosY;
}

void DriverOledSSD1306Text::print(const char* str) {
    while(*str != '\0') {
        putchar(*str);
        str++;
    }
}

void DriverOledSSD1306Text::print(const char* str, uint32_t textX, uint32_t textY) {
    setCurPos(textX, textY);
    print(str);
}


void DriverOledSSD1306Text::putchar(char c, uint32_t textX, uint32_t textY) {
    setCurPos(textX, textY);
    putchar(c);
}

void DriverOledSSD1306Text::saveCurPos() {
    m_currentPosX = m_savedPosX;
    m_currentPosY = m_savedPosY;
}

void DriverOledSSD1306Text::setColor(uint16_t foreground, uint16_t background) {
    m_foregroundColor = foreground;
    m_backgroundColor = background;
}

void DriverOledSSD1306Text::setCurPos(uint32_t textX, uint32_t textY) {
    m_currentPosX = m_areaX0 + textX * (FONT_WIDTH * FONT_DISPLAY_SIZE);
    m_currentPosY = m_areaY0 + textY * (FONT_HEIGHT * FONT_DISPLAY_SIZE);
    if( m_currentPosX > m_areaX1 ) {
            m_currentPosX = m_areaX0;
    }
    if( m_currentPosY > m_areaY1 ) {
            m_currentPosY = m_areaY0;
    }
}

void DriverOledSSD1306Text::setTextArea(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) {
    m_areaX0 = x0;
    m_areaY0 = y0;
    m_areaX1 = x1;
    m_areaY1 = y1;
    m_currentPosX = m_areaX0;
    m_currentPosY = m_areaY0;
    m_savedPosX = m_areaX0;
    m_savedPosY = m_areaY0;
}

