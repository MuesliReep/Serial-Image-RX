#include "Output.h"

Output::Output()
{
}


int Output::initialise() {

  //Open window, set size etc.
  cvNamedWindow( "OSD", CV_WINDOW_AUTOSIZE );

  return 0;
}

int Output::destroy() {

  cvDestroyWindow( "OSD" );
}
