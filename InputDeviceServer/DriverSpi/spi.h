/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   spi.h
 * Author: takeshi
 *
 * Created on April 6, 2016, 9:32 PM
 */

#ifndef SPI_H
#define SPI_H

#ifdef __cplusplus
extern "C" {
#endif

int spiGetFd     (int channel) ;
int spiDataRW    (int channel, unsigned char *data, int len) ;
int spiDataW     (int channel, unsigned char *data, int len) ;
int spiSetupMode (int channel, int speed, int mode) ;
int spiSetup     (int channel, int speed) ;


#ifdef __cplusplus
}
#endif

#endif /* SPI_H */

