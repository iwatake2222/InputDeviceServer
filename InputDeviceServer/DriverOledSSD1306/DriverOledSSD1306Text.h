/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DriverOledSSD1306Text.h
 * Author: takeshi
 *
 * Created on April 6, 2016, 11:40 PM
 */

#include <stdint.h>
#include "../myCommon.h"
#include "DriverOledSSD1306.h"

#ifndef DRIVEROLEDSSD1306TEXT_H
#define DRIVEROLEDSSD1306TEXT_H

class DriverOledSSD1306Text {
    /*** Config and interface ***/
public:
private:
    #define IS_FONT_ROTATE    // if font bitmap table is rotated
    const static uint32_t FONT_WIDTH = 5;
    const static uint32_t FONT_HEIGHT = 8;
    const static uint32_t FONT_DISPLAY_SIZE = 2;
    
    /*** Methods ***/
public:
    DriverOledSSD1306Text(DriverOledSSD1306 * driverOledSSD1306);
    //DriverOledSSD1306Text(const DriverOledSSD1306& orig);
    virtual ~DriverOledSSD1306Text();
    void setColor(uint16_t foreground, uint16_t background);
    void setTextArea(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
    void putchar(char c);
    void print(const char *str);
    void putchar(char c, uint32_t textX, uint32_t textY);
    void print(const char *str, uint32_t textX, uint32_t textY);
    void setCurPos(uint32_t textX, uint32_t textY);

private:
    void saveCurPos();
    void loadCurPos();
    void drawChar(char c);
    
    /*** Attributes ***/
private:
    DriverOledSSD1306* m_driverOledSSD1306;
    uint16_t m_foregroundColor;
    uint16_t m_backgroundColor;
    uint32_t m_areaX0;  // start from 0
    uint32_t m_areaY0;  // start from 0
    uint32_t m_areaX1;    // real end point + 1. e.g. 128
    uint32_t m_areaY1;    // real end point + 1. e.g. 160
    
    uint32_t m_currentPosX;
    uint32_t m_currentPosY;
    uint32_t m_savedPosX;
    uint32_t m_savedPosY;

};

#endif /* DRIVEROLEDSSD1306TEXT_H */

