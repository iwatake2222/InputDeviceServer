#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "wiringPi.h"
#include "wiringPiI2C.h"
#include "wiringPiSPI.h"

#ifdef __cplusplus
extern "C" {
#endif

const static uint8_t     spiBPW   = 8 ;
const static uint16_t    spiDelay = 0 ;
static uint32_t    spiSpeeds [2] ;


int spiGetFd     (int channel)
{
    return wiringPiSPIGetFd(channel);
}

int spiDataRW    (int channel, unsigned char *data, int len)
{
    return wiringPiSPIDataRW(channel, data, len);
}

int spiDataW    (int channel, unsigned char *data, int len)
{
  struct spi_ioc_transfer spi ;

  channel &= 1 ;

// Mentioned in spidev.h but not used in the original kernel documentation
//	test program )-:
  
  
  memset (&spi, 0, sizeof (spi)) ;
  
  uint8_t *dummy = (uint8_t *)malloc(len);

  spi.tx_buf        = (unsigned long)data ;
  spi.rx_buf        = (unsigned long)dummy ;
  spi.len           = len ;
  spi.delay_usecs   = spiDelay ;
  spi.speed_hz      = spiSpeeds [channel] ;
  spi.bits_per_word = spiBPW ;
  
  int ret = ioctl (spiGetFd(channel), SPI_IOC_MESSAGE(1), &spi) ;
  
  free(dummy);

  return ret;
}

int spiSetupMode (int channel, int speed, int mode)
{
    spiSpeeds [channel] = speed ;
    return wiringPiSPISetupMode(channel, speed, mode);
}

int spiSetup     (int channel, int speed)
{
    return spiSetupMode(channel, speed, 0);
}

#ifdef __cplusplus
}
#endif
