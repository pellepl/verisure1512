
#include <target.h>
#include <uart_driver.h>
#include <miniutils.h>

#include "lepton_driver.h"

//------------------------------------------------
// calculate crc16-ccitt very fast
// Idea from: http://www.ccsinfo.com/forum/viewtopic.php?t=24977
static uint16_t lepton_crc16(const uint8_t *buf, uint16_t len) 
{
  uint16_t x;
  uint16_t crc = 0;
  while (len--) {
    x = (crc >> 8) ^ *buf++;
    x ^= x >> 4;
    crc = (crc << 8) ^ (x << 12) ^ (x << 5) ^ x;
  }
  return crc;
}

//---------------------------------------------------------
int lepton_packet_check_crc(struct lepton_packet_s *packet)
{
  int crc_ok = 0;

  struct lepton_packet_s p;
  memcpy(&p, packet, sizeof(p));

  // The 4 most sig bits not relevant for CRC
  p.id[0] = packet->id[0] & 0x0F;
  uint16_t crc_pkt = LEPTON_GET_CRC(packet);
  // clear CRC when do calc
  p.crc[0] = 0;
  p.crc[1] = 0;
  uint16_t crc_test = lepton_crc16((uint8_t*)&p,
                                   sizeof(p));
  if (crc_pkt == crc_test) {
    crc_ok = 1;
  }
  return crc_ok;
}
