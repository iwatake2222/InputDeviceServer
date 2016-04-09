/* 
 * File:   DriverLcdST7735R.h
 * Author: takeshi
 *
 * Created on April 6, 2016, 6:44 PM
 */

#include <stdint.h>
#include "../myCommon.h"

#ifndef DRIVERLCDST7735R_H
#define DRIVERLCDST7735R_H

class DriverLcdST7735R {
    /*** Config and interface ***/
public:
    const static uint16_t COLOR_RED   = 0xF800;
    const static uint16_t COLOR_GREEN = 0x07E0;
    const static uint16_t COLOR_BLUE  = 0x001F;
    typedef enum {
        LCD_ST7735R_ROTATE_0   = 0,
        LCD_ST7735R_ROTATE_90  = 1,
        LCD_ST7735R_ROTATE_180 = 2,
        LCD_ST7735R_ROTATE_270 = 3,
    } LCD_ST7735R_ROTATE;
    const static uint32_t DEFAULT_WIDTH  = 128;
    const static uint32_t DEFAULT_HEIGHT = 160;
private:
    static const uint32_t SPI_SPEED = 8*1000*1000;
    static const uint32_t SPI_MODE = 0;
    
    /*** Methods ***/
public:
    DriverLcdST7735R(uint32_t spiChannel, uint32_t gpioRS, uint32_t width, uint32_t height, LCD_ST7735R_ROTATE rotate);
    DriverLcdST7735R(const DriverLcdST7735R& orig);
    virtual ~DriverLcdST7735R(); 
    RET init();
    void setAddrWindowFull();
    void setAddrWindow(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
    void fillRect(uint16_t color, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);
    void drawBuffer(uint8_t *buffer, uint32_t size);
    void setRasterVertical();
    void setRasterHorizontal();
private:
    void sendCmd(uint8_t cmd);
    void sendData(uint8_t data);
    
    /*** Attributes ***/
private:
    uint32_t           m_spiChannel;
    uint32_t           m_gpioRS;
    LCD_ST7735R_ROTATE m_rotate;
    uint32_t           m_width;
    uint32_t           m_height;
};

#endif /* DRIVERLCDST7735R_H */

