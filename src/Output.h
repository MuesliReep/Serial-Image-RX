#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "opencv/cv.h"
#include "opencv2/highgui/highgui_c.h"

class Output
{
public:
    Output();
    int initialise();
    int destroy();
};

#endif // OUTPUT_H
