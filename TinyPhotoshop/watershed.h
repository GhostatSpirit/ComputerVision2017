#ifndef WATERSHED_H
#define WATERSHED_H

#include <set>
namespace watershed
{
typedef unsigned char BYTE;
typedef BYTE *ByteImage;
typedef long RegionNo;


bool Watershed(BYTE *imageIn, BYTE *imageOut, unsigned int w, unsigned int h);

}

#endif // WATERSHED_H
