/* 
 * File:   DriverLcdST7735R.cpp
 * Author: takeshi
 * 
 * Created on April 6, 2016, 6:44 PM
 */

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include "../myCommon.h"
#include "../DriverSpi/spi.h"
#include "../DriverGPIO/gpio.h"
#include "lcdST7735R_CMD.h"
#include "DriverLcdST7735R.h"


#define SET_LCD_DATA   gpioDigitalWrite(m_gpioRS, 1)
#define SET_LCD_CMD    gpioDigitalWrite(m_gpioRS, 0)

DriverLcdST7735R::DriverLcdST7735R(uint32_t spiChannel, uint32_t gpioRS, uint32_t width, uint32_t height, LCD_ST7735R_ROTATE rotate) {
    m_spiChannel = spiChannel & 1;
    m_gpioRS     = gpioRS;
    m_width      = width;
    m_height     = height;
    m_rotate     = rotate;
}

DriverLcdST7735R::DriverLcdST7735R(const DriverLcdST7735R& orig) {
}

DriverLcdST7735R::~DriverLcdST7735R() {
}


void DriverLcdST7735R::setAddrWindow(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) {
    sendCmd(ST7735_CASET);  // column addr set
    sendData(0x00);
    sendData(x0+0);   // XSTART
    sendData(0x00);
    sendData(x1-1);   // XEND

    sendCmd(ST7735_RASET);  // row addr set
    sendData(0x00);
    sendData(y0+0);    // YSTART
    sendData(0x00);
    sendData(y1-1);    // YEND
}

void DriverLcdST7735R::setAddrWindowFull() {
    setAddrWindow(0, 0, m_width, m_height);
}

void DriverLcdST7735R::fillRect(uint16_t color, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) {
    uint32_t width = x1 - x0;
    uint32_t height = y1 - y0;

    setAddrWindow(x0, y0, x1, y1);
    sendCmd(ST7735_RAMWR);  // write to RAM

    SET_LCD_DATA;
    //ENABLE_LCD_CS;
    uint8_t *buffer = (uint8_t *)malloc(m_width*2);
    for(uint32_t x = 0; x < width*2;){
        buffer[x++] = (color >> 8)&0xFF;
        buffer[x++] = color&0xFF;
    }

    for (uint32_t y = 0; y < height; y++) {
        spiDataW (m_spiChannel, buffer, width*2);
    }
    free(buffer);
#if 0
    for (uint32_t x=0; x < width; x++) {
        for (uint32_t y=0; y < height; y++) {
            sendData((color >> 8)&0xFF);
            sendData(color&0xFF);
        }
    }
#endif
    //DISABLE_LCD_CS;
    setAddrWindowFull();
}

void DriverLcdST7735R::drawBuffer(uint8_t *buffer, uint32_t size) {
    sendCmd(ST7735_RAMWR);  // write to RAM
    SET_LCD_DATA;
    spiDataW (m_spiChannel, buffer, size);
}

RET DriverLcdST7735R::init() {
    
    if( gpioSetupGpio() == -1 ) {
        //cout << "error" << endl;
        return RET_ERR;
    }
      
    if( spiSetupMode (m_spiChannel, SPI_SPEED, SPI_MODE) == -1) {
        std::cout << "error" << std::endl;
        return RET_ERR;
    }
    
    gpioPinMode(m_gpioRS, OUTPUT);

    sendCmd(ST7735_SWRESET);	usleep(1000);
    sendCmd(ST7735_SLPOUT);	usleep(1000);
    sendCmd(ST7735_FRMCTR1);	usleep(1000);
    sendData(0x01);
    sendData(0x2C);
    sendData(0x2D);
    sendCmd(ST7735_FRMCTR2);
    sendData(0x01);
    sendData(0x2C);
    sendData(0x2D);

    sendCmd(ST7735_FRMCTR3);
    sendData(0x01); // dot inversion mode
    sendData(0x2C);
    sendData(0x2D);
    sendData(0x01); // line inversion mode
    sendData(0x2C);
    sendData(0x2D);

    sendCmd(ST7735_INVCTR);  // display inversion control
    sendData(0x07);  // no inversion

    sendCmd(ST7735_PWCTR1);  // power control
    sendData(0xA2);
    sendData(0x02);      // -4.6V
    sendData(0x84);      // AUTO mode

    sendCmd(ST7735_PWCTR2);  // power control
    sendData(0xC5);      // VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD

    sendCmd(ST7735_PWCTR3);  // power control
    sendData(0x0A);      // Opamp current small
    sendData(0x00);      // Boost frequency

    sendCmd(ST7735_PWCTR4);  // power control
    sendData(0x8A);      // BCLK/2, Opamp current small & Medium low
    sendData(0x2A);

    sendCmd(ST7735_PWCTR5);  // power control
    sendData(0x8A);
    sendData(0xEE);

    sendCmd(ST7735_VMCTR1);  // power control
    sendData(0x0E);

    sendCmd(ST7735_INVOFF);    // don't invert display

    sendCmd(ST7735_MADCTL);  // memory access control (directions)
    uint8_t madctl;
    switch(m_rotate){
    case LCD_ST7735R_ROTATE_0:
        madctl = (0<<7) | (0<<6) | (0<<5) | (0<<3);
        break;
    case LCD_ST7735R_ROTATE_90:
        madctl = (1<<7) | (0<<6) | (1<<5) | (0<<3);
        break;
    case LCD_ST7735R_ROTATE_270:
        madctl = (0<<7) | (1<<6) | (1<<5) | (0<<3);
        break;
    case LCD_ST7735R_ROTATE_180:
        madctl = (1<<7) | (1<<6) | (0<<5) | (0<<3);
        break;
    }
    sendData(madctl);


    sendCmd(ST7735_COLMOD);  // set color mode
    sendData(0x05);        // 16-bit color

    sendCmd(ST7735_CASET);  // column addr set
    sendData(0x00);
    sendData(0x00);   // XSTART = 0
    sendData(0x00);
    sendData(m_width);   // XEND = 127

    sendCmd(ST7735_RASET);  // row addr set
    sendData(0x00);
    sendData(0x00);    // XSTART = 0
    sendData(0x00);
    sendData(m_height);    // XEND = 159

    sendCmd(ST7735_GMCTRP1);
    sendData(0x0f);
    sendData(0x1a);
    sendData(0x0f);
    sendData(0x18);
    sendData(0x2f);
    sendData(0x28);
    sendData(0x20);
    sendData(0x22);
    sendData(0x1f);
    sendData(0x1b);
    sendData(0x23);
    sendData(0x37);
    sendData(0x00);
    sendData(0x07);
    sendData(0x02);
    sendData(0x10);
    sendCmd(ST7735_GMCTRN1);
    sendData(0x0f);
    sendData(0x1b);
    sendData(0x0f);
    sendData(0x17);
    sendData(0x33);
    sendData(0x2c);
    sendData(0x29);
    sendData(0x2e);
    sendData(0x30);
    sendData(0x30);
    sendData(0x39);
    sendData(0x3f);
    sendData(0x00);
    sendData(0x07);
    sendData(0x03);
    sendData(0x10);

    sendCmd(ST7735_DISPON);
    usleep(1000);

    sendCmd(ST7735_NORON);  // normal display on
    usleep(1000);

    sendCmd(ST7735_DISPON);
    
    return RET_OK;
}

void DriverLcdST7735R::setRasterVertical()
{
    sendCmd(ST7735_MADCTL);  // memory access control (directions)
    uint8_t madctl;
    switch(m_rotate){
    case LCD_ST7735R_ROTATE_0:
        madctl = (0<<7) | (0<<6) | (1<<5) | (0<<3);
        break;
    case LCD_ST7735R_ROTATE_90:
        madctl = (1<<7) | (0<<6) | (0<<5) | (0<<3);
        break;
    case LCD_ST7735R_ROTATE_270:
        madctl = (0<<7) | (1<<6) | (0<<5) | (0<<3);
        break;
    case LCD_ST7735R_ROTATE_180:
        madctl = (1<<7) | (1<<6) | (1<<5) | (0<<3);
        break;
    }
    sendData(madctl);
}

void DriverLcdST7735R::setRasterHorizontal()
{
    sendCmd(ST7735_MADCTL);  // memory access control (directions)
    uint8_t madctl;
    switch(m_rotate){
    case LCD_ST7735R_ROTATE_0:
        madctl = (0<<7) | (0<<6) | (0<<5) | (0<<3);
        break;
    case LCD_ST7735R_ROTATE_90:
        madctl = (1<<7) | (0<<6) | (1<<5) | (0<<3);
        break;
    case LCD_ST7735R_ROTATE_270:
        madctl = (0<<7) | (1<<6) | (1<<5) | (0<<3);
        break;
    case LCD_ST7735R_ROTATE_180:
        madctl = (1<<7) | (1<<6) | (0<<5) | (0<<3);
        break;
    }
    sendData(madctl);
}

void DriverLcdST7735R::sendCmd(uint8_t cmd) {
    SET_LCD_CMD;
    ///ENABLE_LCD_CS;
    spiDataRW (m_spiChannel, &cmd, 1);
    //DISABLE_LCD_CS;
}

void DriverLcdST7735R::sendData(uint8_t data) {
    SET_LCD_DATA;
    //ENABLE_LCD_CS;
    spiDataRW (m_spiChannel, &data, 1);
    //DISABLE_LCD_CS;
}

