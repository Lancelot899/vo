#include "ImgIO.h"
#include "Frame.h"


ImgIO::ImgIO(int width, int height, SrcType srcType, char* )
{
    if(srcType == SRC_VIDEO) {
        cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    }
}
