#ifndef DECODER_H
#define DECODER_H

#include <QByteArray>
#include <QDebug>

#include <stdint.h>

class Decoder
{
public:
  Decoder();

  uint8_t *currentFrame;
  uint8_t *nextFrame;

  uint8_t stepX=0;
  uint8_t stepY=0;

  bool decodeGREY(int xres, int yres, QByteArray data, bool *syncReceived);
  bool decodeRGB(int xres, int yres, uint8_t *data);

  bool inSync;

  uint8_t *getCurrentFrame();

  void copyFrame();

private:
  int syncBits;
};

#endif // DECODER_H
