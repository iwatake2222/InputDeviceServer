/* 
 * File:   port.h
 * Author: takeshi
 *
 * Created on April 8, 2016, 7:23 PM
 */

#ifndef PORT_H
#define PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_LCD_ST7735R_RS 16
#define GPIO_INPUT_KEY_0 17    
#define GPIO_INPUT_KEY_1 27
#define GPIO_INPUT_ROTARY_ENCODER_A 20
#define GPIO_INPUT_ROTARY_ENCODER_B 21
#define GPIO_LED_0 22
#define GPIO_LED_1 23

#define SPI_LCD_ST7735R 0
    
#define I2C_ADDR_ADXL345 0x53
#define I2C_ADDR_OLED_SSD1306 0x3c
    
#ifdef __cplusplus
}
#endif

#endif /* PORT_H */

/*
# Port map

## IO
GPIO00 = N/A
GPIO01 = N/A
GPIO02 = I2C_SDA
GPIO03 = I2C_SCL
GPIO04 = 
GPIO05 = 
GPIO06 = 
GPIO07 = SPI_CE1
GPIO08 = SPI_CE0
GPIO09 = SPI_MISO
GPIO10 = SPI_MOSI
GPIO11 = SPI_SCLK
GPIO12 = 
GPIO13 = 
GPIO14 = UART_TXD
GPIO15 = UART_RXD
GPIO16 = LcdST7735R_RS
GPIO17 = INPUT_KEY0
GPIO18 = 
GPIO19 = 
GPIO20 = INPUT_RotaryEncoder_B
GPIO21 = INPUT_RotaryEncoder_A
GPIO22 = LED_0
GPIO23 = LED_1
GPIO24 = 
GPIO25 = N/A
GPIO26 = 
GPIO27 = INPUT_KEY1
GPIO28 = N/A
GPIO29 = N/A

## Function
USART = System Console
SPI0.0 = LcdST7735R
SPI0.1 = 
I2C = Accelerometer (ADXL345), OLED_SSD1306

## Note
*/