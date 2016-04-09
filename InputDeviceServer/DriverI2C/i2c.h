/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   i2c.h
 * Author: takeshi
 *
 * Created on April 6, 2016, 10:44 PM
 */

#include <stdint.h>

#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
extern "C" {
#endif

int i2cRead           (int fd) ;
int i2cReadReg8       (int fd, int reg) ;
int i2cReadReg16      (int fd, int reg) ;

int i2cWrite          (int fd, int data) ;
int i2cWriteReg8      (int fd, int reg, int data) ;
int i2cWriteReg16     (int fd, int reg, int data) ;
int i2cWriteRegBuffer (int fd, int reg, uint8_t *buffer, int size) ;    // memcpy occurs
int i2cWriteBuffer    (int fd, uint8_t *buffer, int size) ;             // buffer[0] need to contain CONTROL_XXX

int i2cSetupInterface (const char *device, int devId) ;
int i2cSetup          (const int devId) ;


#ifdef __cplusplus
}
#endif

#endif /* I2C_H */

