/* 
 * File:   DriverLcdST7735RText.cpp
 * Author: takeshi
 * 
 * Created on April 6, 2016, 7:19 PM
 */

#include "stdlib.h"
#include "DriverLcdST7735RText.h"
#include "../font//font.h"

DriverLcdST7735RText::DriverLcdST7735RText(DriverLcdST7735R * driverLcdST7735R) {
    m_driverLcdST7735R = driverLcdST7735R;
}

DriverLcdST7735RText::DriverLcdST7735RText(const DriverLcdST7735RText& orig) {
}

DriverLcdST7735RText::~DriverLcdST7735RText() {
}

void DriverLcdST7735RText::setColor(uint16_t foreground, uint16_t background) {
    m_foregroundColor = foreground;
    m_backgroundColor = background;
}

void DriverLcdST7735RText::setTextArea(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) {
    m_areaX0 = x0;
    m_areaY0 = y0;
    m_areaX1 = x1;
    m_areaY1 = y1;
    m_currentPosX = m_areaX0;
    m_currentPosY = m_areaY0;
    m_savedPosX = m_areaX0;
    m_savedPosY = m_areaY0;
}

void DriverLcdST7735RText::loadCurPos() {
    m_savedPosX = m_currentPosX;
    m_savedPosY = m_currentPosY;
}

void DriverLcdST7735RText::saveCurPos() {
    m_currentPosX = m_savedPosX;
    m_currentPosY = m_savedPosY;
}



void DriverLcdST7735RText::print(const char* str, uint32_t textX, uint32_t textY) {
    setCurPos(textX, textY);
    print(str);
}

void DriverLcdST7735RText::print(const char* str) {
    while(*str != '\0') {
        putchar(*str);
        str++;
    }
}

void DriverLcdST7735RText::putchar(char c, uint32_t textX, uint32_t textY) {
    setCurPos(textX, textY);
    putchar(c);
}

void DriverLcdST7735RText::putchar(char c) {
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
        m_driverLcdST7735R->fillRect(m_backgroundColor, m_areaX0, m_currentPosY, m_areaX1, endY);
    }
#ifdef IS_FONT_ROTATE
    m_driverLcdST7735R->setRasterVertical();
    m_driverLcdST7735R->setAddrWindow(m_currentPosY, m_currentPosX, endY, endX);
    drawChar(c);
    m_driverLcdST7735R->setRasterHorizontal();
#else
    m_driverLcdST7735R->setAddrWindow(m_currentPosX, m_currentPosY, endX, endY);
    drawChar(c);
#endif
    m_currentPosX = endX;	
}



void DriverLcdST7735RText::setCurPos(uint32_t textX, uint32_t textY) {
    m_currentPosX = m_areaX0 + textX * (FONT_WIDTH * FONT_DISPLAY_SIZE);
    m_currentPosY = m_areaY0 + textY * (FONT_HEIGHT * FONT_DISPLAY_SIZE);
    if( m_currentPosX > m_areaX1 ) {
            m_currentPosX = m_areaX0;
    }
    if( m_currentPosY > m_areaY1 ) {
            m_currentPosY = m_areaY0;
    }
}

void DriverLcdST7735RText::drawChar(char c) {
    uint8_t buffer[FONT_WIDTH*FONT_DISPLAY_SIZE*FONT_HEIGHT*FONT_DISPLAY_SIZE*2];
    uint32_t cnt=0;
    for (uint32_t i =0; i<FONT_WIDTH; i++ ) {
        for(uint32_t size = 0; size < FONT_DISPLAY_SIZE; size++){
            uint32_t line = *(GET_FONT()+(c*FONT_WIDTH)+i);
            for (uint32_t j = 0; j<8; j++) {
                for(uint32_t size = 0; size < FONT_DISPLAY_SIZE; size++){
                    if (line & 0x1) {
                        buffer[cnt++] = m_foregroundColor >> 8;
                        buffer[cnt++] = m_foregroundColor;
                    } else {
                        buffer[cnt++] = m_backgroundColor >> 8;
                        buffer[cnt++] = m_backgroundColor;
                    }
                }
                line >>= 1;
            }
        }
    }
    m_driverLcdST7735R->drawBuffer(buffer, FONT_WIDTH*FONT_DISPLAY_SIZE*FONT_HEIGHT*FONT_DISPLAY_SIZE*2);
}
