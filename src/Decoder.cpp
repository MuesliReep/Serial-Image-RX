#include "Decoder.h"

Decoder::Decoder()
{
  inSync = false;
  currentFrame = new uint8_t[120*96];
  nextFrame    = new uint8_t[120*96];
  syncBits = 0;
}

bool Decoder::decodeGREY(int xres, int yres, QByteArray data, bool *syncReceived) {

    for(int i=0; i < data.size(); i++) {

        char curByte = data.at(i);

        if(curByte == 0) {
          syncBits++;
          if(syncBits >=3) {
            inSync=true;
            *syncReceived = true;
            syncBits = 0;
          }
        }

        if(inSync && curByte > 0) {

            *(nextFrame + (stepX + stepY*yres)) = curByte;

            stepX++;

            if(stepX > xres) {
                stepX = 0;
                stepY++;
                if(stepY > yres) { // Is this frame complete?
                    stepY = 0;
                    // currentFrame = nextFrame;
                    //std::array<uint8_t,120*96> currentFrame = nextFrame;
                    copyFrame();
                    qDebug() << "Full frame decoded";
                    return true;
                }
            }
        }
    }
    return false;
}


bool Decoder::decodeRGB(int xres, int yres, uint8_t *data) {
    return false;
}

uint8_t* Decoder::getCurrentFrame() {

    return currentFrame;
}

void Decoder::copyFrame() {

  for(int i=0; i < 120*96; i++) {
    *(currentFrame + i) = *(nextFrame + i);
  }
}
