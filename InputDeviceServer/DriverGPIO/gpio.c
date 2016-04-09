
#include <stdint.h>
#include <wiringPi.h>

#ifdef __cplusplus
extern "C" {
#endif

static int isInitialized = 0;

int  gpioSetup       (void) {
    if(isInitialized == 0) {
        isInitialized = 1;
        return wiringPiSetup();
    }        
    return 0;
}

int  gpioSetupSys    (void) {
    if(isInitialized == 0) {
        isInitialized = 1;
        return wiringPiSetupSys();
    }
    return 0;
}
int  gpioSetupGpio   (void) {
    if(isInitialized == 0) {
        isInitialized = 1;
        return wiringPiSetupGpio();
    }
    return 0;
}
int  gpioSetupPhys   (void) {
    if(isInitialized == 0) {
        isInitialized = 1;
        return wiringPiSetupPhys();
    }
    return 0;
}

void gpioPinModeAlt          (int pin, int mode) {
    pinModeAlt(pin, mode);
}

void gpioPinMode             (int pin, int mode) {
    pinMode(pin, mode);
}

void gpioPullUpDnControl     (int pin, int pud)  {
    pullUpDnControl(pin, pud);
}

int  gpioDigitalRead         (int pin) {
    return digitalRead(pin);
}

void gpioDigitalWrite        (int pin, int value) {
    digitalWrite(pin, value);
}

void gpioPwmWrite            (int pin, int value) {
    pwmWrite(pin, value);
}

int  gpioAnalogRead          (int pin) {
    return analogRead(pin);
}

void gpioAnalogWrite         (int pin, int value) {
    analogWrite(pin, value);
}

#ifdef __cplusplus
}
#endif
