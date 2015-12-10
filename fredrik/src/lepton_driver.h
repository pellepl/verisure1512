#ifndef _LEPTON_DRIVER_H_
#define _LEPTON_DRIVER_H_

#include <stdint.h>

#define LEPTON_GET_ID(p)  (uint16_t)(((uint16_t)(p)->id[0]  << 8) | (uint16_t)(p)->id[1]);
#define LEPTON_GET_CRC(p) (uint16_t)(((uint16_t)(p)->crc[0] << 8) | (uint16_t)(p)->crc[1]);

struct lepton_video_packet_s
{
  uint8_t id[2];
  uint8_t crc[2];
  uint8_t payload[160];
} __attribute__((packed));

void lepton_get_video_packet(struct lepton_video_packet_s *packet);
void lepton_init(void);

#endif
