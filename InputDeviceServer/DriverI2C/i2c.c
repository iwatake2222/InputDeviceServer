#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "i2c.h"

#ifdef __cplusplus
extern "C" {
#endif
    
int i2cRead           (int fd) {
    return wiringPiI2CRead(fd);
}

int i2cReadReg8       (int fd, int reg) {
    return wiringPiI2CReadReg8(fd, reg);
}

int i2cReadReg16      (int fd, int reg) {
    return wiringPiI2CReadReg16(fd, reg);
}

int i2cWrite          (int fd, int data) {
    return wiringPiI2CWrite(fd, data);
}

int i2cWriteReg8      (int fd, int reg, int data) {
    return wiringPiI2CWriteReg8(fd, reg, data);
}

int i2cWriteReg16     (int fd, int reg, int data) {
    return wiringPiI2CWriteReg16(fd, reg, data);
}

int i2cWriteRegBuffer (int fd, int reg, uint8_t *buffer, int size) {
    uint8_t *realBuffer = (uint8_t *)malloc(size+1);
    realBuffer[0] = reg;
    memcpy(&realBuffer[1], buffer, size);
    return write(fd, realBuffer, size+1);
}

int i2cWriteBuffer    (int fd, uint8_t *buffer, int size) {
    return write(fd, buffer, size);
}


int i2cSetupInterface (const char *device, int devId) {
    return wiringPiI2CSetupInterface(device, devId);
}

int i2cSetup          (const int devId) {
    return wiringPiI2CSetup(devId);
}

#ifdef __cplusplus
}
#endif