#include "mbed.h"

#define LEPTON_FRAME_SIZE (164)

Serial pc(USBTX, USBRX);
SPI lepton_spi(SPI_MOSI, SPI_MISO, SPI_SCK);
DigitalOut spi_cs(SPI_CS);

static uint8_t lepton_frame[VOSPI_FRAME_SIZE];
static uint16_t lepton_image[80][80];

int main()
{
    pc.printf("Alive\n");
 
}