/* 
 * File:   gpio.h
 * Author: takeshi
 *
 * Created on April 7, 2016, 1:06 AM
 */

#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
extern "C" {
#endif
    
#define	INPUT			 0
#define	OUTPUT			 1

#define	PUD_OFF			 0
#define	PUD_DOWN		 1
#define	PUD_UP			 2
    
int  gpioSetup       (void) ;
int  gpioSetupSys    (void) ;
int  gpioSetupGpio   (void) ;
int  gpioSetupPhys   (void) ;

void gpioPinModeAlt          (int pin, int mode) ;
void gpioPinMode             (int pin, int mode) ;
void gpioPullUpDnControl     (int pin, int pud) ;
int  gpioDigitalRead         (int pin) ;
void gpioDigitalWrite        (int pin, int value) ;
void gpioPwmWrite            (int pin, int value) ;
int  gpioAnalogRead          (int pin) ;
void gpioAnalogWrite         (int pin, int value) ;


#ifdef __cplusplus
}
#endif

#endif /* GPIO_H */

