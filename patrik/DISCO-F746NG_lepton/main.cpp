#include "mbed.h"
#include "palettes.h"
#include "LCD_DISCO_F746NG.h"

#define BYTES_PER_PACKET (164)
#define DATA_PER_PACKET (BYTES_PER_PACKET/2)
#define PACKETS_PER_FRAME (60)

Serial pc(USBTX, USBRX);
SPI lepton_spi(SPI_MOSI, SPI_MISO, SPI_SCK);
DigitalOut spi_cs(SPI_CS);
LCD_DISCO_F746NG lcd;

static uint8_t lepton_buffer[BYTES_PER_PACKET * PACKETS_PER_FRAME];
static uint32_t lepton_image[60][80];

void setup(void)
{
    lepton_spi.format(8,3);
    lepton_spi.frequency(20000000);
    spi_cs = 1;
    spi_cs = 0;
    spi_cs = 1;
    wait_ms(185);
    lcd.Clear(LCD_COLOR_BLACK);
}

void get_frame(void)
{
    int p;
    int i;
    int packet_num;

    spi_cs = 0;

    // No error handling. Assume we don't loose any packets
    for(p=0; p<PACKETS_PER_FRAME; p++) {
        for(i=0; i<BYTES_PER_PACKET; i++) {
            lepton_buffer[p*BYTES_PER_PACKET + i] = lepton_spi.write(0x00);
        }
        packet_num = lepton_buffer[p*BYTES_PER_PACKET + 1];
        if(packet_num != p) {
            //pc.printf("-");
            i = -1;
            p = -1;
            wait_ms(1);
        } else {
            //pc.printf("+");
        }
    }

    spi_cs = 1;
}

void create_image(void)
{
    uint16_t min_val = 65535;
    uint16_t max_val = 0;
    uint16_t value;
    int x;
    int y;
    int i;
    int p;

    for(p=0; p<PACKETS_PER_FRAME; p++) {
        for(i=2; i<DATA_PER_PACKET; i++) {
            value = (lepton_buffer[p*BYTES_PER_PACKET+i*2+0] << 8) + lepton_buffer[p*BYTES_PER_PACKET+i*2+1];
            //pc.printf("val = %d\n",value);
            if(value > max_val) {
                max_val = value;
            }
            if(value < min_val) {
                min_val = value;
            }
        }
    }
    
    //pc.printf("max %d, min %d\n", max_val, min_val);

    float diff = max_val - min_val;
    float scale = 255/diff;
    uint8_t r;
    uint8_t g;
    uint8_t b;

    for(p=0; p<PACKETS_PER_FRAME; p++) {
        for(i=2; i<DATA_PER_PACKET; i++) {

            value = (lepton_buffer[p*BYTES_PER_PACKET+i*2+0] << 8) + lepton_buffer[p*BYTES_PER_PACKET+i*2+1];

            x = i - 2;
            y = p;
#if 1
            value = (value - min_val) * scale;
            //pc.printf("val = %d\n", value);
            r = colormap_ironblack[3*value];
            g = colormap_ironblack[3*value+1];
            b = colormap_ironblack[3*value+2];

            lepton_image[y][x] = (0xFF << 24) | (r << 16) | (g << 8) | b;
#else
            lepton_image[y][x] = (0xFF << 24) | (value % 255) << 16 | (value % 255) << 8 | (value % 255);
#endif
        }
    }
}

void dummy_image(void)
{
    int x;
    int y;

    for(y=0; y<60; y++) {
        for(x=0; x<80; x++) {
            lepton_image[y][x] = (0xFF << 24) | (x << 16) | (y << 8);
        }
    }
}

void show_image(void)
{
    int x;
    int y;

    for(y=0; y<60; y++) {
        for(x=0; x<80; x++) {
            lcd.SetTextColor(lepton_image[y][x]);
            lcd.FillRect(x*5, y*4, 5, 4);
        }
    }
}

void dump_frame(void)
{
    int i;
    int p;

    for(p=0; p<PACKETS_PER_FRAME; p++) {
        pc.printf("\n\nPacket %d:\n", p);
        for(i=0; i<BYTES_PER_PACKET; i++) {
        	pc.printf("%x ", lepton_buffer[p*BYTES_PER_PACKET+i]);
        }
    }
}

int main()
{
    pc.printf("Alive\n");
    setup();
    pc.printf("Up...\n");

    while(1) {
        get_frame();
        //dump_frame();
        create_image();
        //dummy_image();
        show_image();
    }
}